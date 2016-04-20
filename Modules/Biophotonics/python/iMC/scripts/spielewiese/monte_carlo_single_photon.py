from collections import OrderedDict
import numpy as np

import theano
import theano.tensor as T
#from theano.sandbox.rng_mrg import MRG_RandomStreams as RandomStreams
from theano.tensor.shared_randomstreams import RandomStreams
import time


#theano.config.compute_test_value = 'warn'
#theano.config.exception_verbosity='high'
#theano.config.profile=True
#theano.config.mode = "FAST_RUN"
#theano.config.mode = "FAST_COMPILE"
#theano.config.scan.allow_gc =True
#theano.config.scan.allow_output_prealloc =False
#theano.optimizer_excluding="more_mem"

# initializing
rng = RandomStreams(seed=234)
photons = 10**6
SHELL_MAX = 101

i = T.iscalar('i')
i.tag.test_value = 3

mu_a = T.scalar('mu_a')
# provide Theano with a default test-value
mu_a.tag.test_value = 2.
mu_s = T.scalar('mu_s')
mu_s.tag.test_value = 20.
microns_per_shell = T.scalar('microns_per_shell')
microns_per_shell.tag.test_value = 50.

albedo = mu_s / (mu_s + mu_a)
shells_per_mfp = 1e4/microns_per_shell/(mu_a+mu_s)

x = theano.shared(np.zeros(photons, dtype=theano.config.floatX))
y = theano.shared(np.zeros(photons, dtype=theano.config.floatX))
z = theano.shared(np.zeros(photons, dtype=theano.config.floatX))

u = theano.shared(np.zeros(photons, dtype=theano.config.floatX))
v = theano.shared(np.zeros(photons, dtype=theano.config.floatX))
w = theano.shared(np.ones(photons, dtype=theano.config.floatX))

weight = theano.shared(np.ones(photons, dtype=theano.config.floatX))

heat = theano.shared(np.zeros(SHELL_MAX, dtype=theano.config.floatX))


x_i = x[i]
y_i = x[i]
z_i = z[i]

u_i = u[i]
v_i = v[i]
w_i = w[i]

weight_i = weight[i]

# move
random = rng.uniform(low=0.00003, high=1.)
t = -T.log(random)

x_moved = x_i + u_i*t
y_moved = y_i + v_i*t
z_moved = z_i + w_i*t

# absorb
shell = T.cast(T.sqrt(T.sqr(x_moved) + T.sqr(y_moved) + T.sqr(z_moved))
               * microns_per_shell, 'int32')
shell = T.clip(shell, 0, SHELL_MAX-1)

new_weight = weight_i * albedo

# new direction
xi1 = rng.uniform(low=-1., high=1.)
xi2 = rng.uniform(low=-1., high=1.)
xi_norm = T.sqrt(T.sqr(xi1) + T.sqr(xi2))

t_xi = rng.uniform(low=0.000000001, high=1.)

# rescale xi12 to fit t_xi as norm
xi1 = xi1/xi_norm * T.sqr(t_xi)
xi2 = xi2/xi_norm * T.sqr(t_xi)

u_new_direction = 2. * t_xi - 1.
v_new_direction = xi1 * T.sqrt((1. - T.sqr(u_new_direction)) / t_xi)
w_new_direction = xi2 * T.sqrt((1. - T.sqr(u_new_direction)) / t_xi)

# roulette
weight_for_starting_roulette = 0.001
CHANCE = 0.1
partakes_roulette = T.switch(T.lt(new_weight, weight_for_starting_roulette),
                             1,
                             0)
roulette = rng.uniform(low=0., high=1.)
loses_roulette = T.gt(roulette, CHANCE)
# if roulette decides to terminate the photon: set weight to 0
weight_after_roulette = T.switch(T.and_(partakes_roulette, loses_roulette),
                                 0.,
                                 new_weight)
# if partakes in roulette but does not get terminated
weight_after_roulette = T.switch(T.and_(partakes_roulette, T.invert(loses_roulette)),
                                 weight_after_roulette / CHANCE,
                                 weight_after_roulette)

new_heat = (1.0 - albedo) * weight_i
heat_i = heat[shell]


def one_run(i, mu_a, mu_s, microns_per_shell,
            heat, x, y, z, u, v, w, weight):
    return OrderedDict({heat: T.inc_subtensor(heat_i, new_heat),
                        x: T.set_subtensor(x_i, x_moved),
                        y: T.set_subtensor(y_i, y_moved),
                        z: T.set_subtensor(z_i, z_moved),
                        u: T.set_subtensor(u_i, u_new_direction),
                        v: T.set_subtensor(v_i, v_new_direction),
                        w: T.set_subtensor(w_i, w_new_direction),
                        weight: T.set_subtensor(weight_i, weight_after_roulette)})


one_photon_results, one_photon_updates = theano.scan(fn=one_run,
                                                     outputs_info=None,
                                                     non_sequences=[i, mu_a, mu_s, microns_per_shell,
                                                                    heat, x, y, z, u, v, w, weight],
                                                     n_steps=100,
                                                     strict=True)

final_heat_result = one_photon_updates[heat]


initial_heat = np.zeros(SHELL_MAX, dtype=theano.config.floatX)

heat_for_one_photon = theano.function(inputs=[i, mu_a, mu_s, microns_per_shell],
                                      outputs=final_heat_result,
                                      updates=one_photon_updates)

start = time.time()

print("start simulation")

print(heat_for_one_photon(0, 2., 20., 50.))

end = time.time()
print("end simulation after: " + str(end - start) + " seconds")


