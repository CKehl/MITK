#include "mitkDisplayVectorInteractor.h"
#include "Operation.h"
#include "mitkDisplayCoordinateOperation.h"
#include "PositionEvent.h"
#include "mitkInteractionConst.h"

//##ModelId=3EF222410127
void mitk::DisplayVectorInteractor::ExecuteOperation(Operation* operation)
{
    DisplayCoordinateOperation* dcOperation = static_cast<DisplayCoordinateOperation*>(operation);
    if(dcOperation==NULL) return;

    switch(operation->GetOperationType())
    {
    case OpTEST:
        m_Sender=dcOperation->GetRenderer();
        m_StartDisplayCoordinate=dcOperation->GetStartDisplayCoordinate();
        m_LastDisplayCoordinate=dcOperation->GetLastDisplayCoordinate();
        m_CurrentDisplayCoordinate=dcOperation->GetCurrentDisplayCoordinate();
        std::cout << m_CurrentDisplayCoordinate << std::endl;

		std::cout<<"Message from DisplayVectorInteractor.cpp::ExecuteOperation() : "
            << "StartDisplayCoordinate:" <<     m_StartDisplayCoordinate 
            << "LastDisplayCoordinate:" <<      m_LastDisplayCoordinate 
            << "CurrentDisplayCoordinate:" <<   m_CurrentDisplayCoordinate 
            << std::endl;

        break;
    }

}

//##ModelId=3EF2224401CB
bool mitk::DisplayVectorInteractor::ExecuteSideEffect(int sideEffectId, mitk::StateEvent const* stateEvent, int groupEventId, int objectEventId)
{
    bool ok=false;

    const PositionEvent* posEvent=dynamic_cast<const PositionEvent*>(stateEvent->GetEvent());
    if(posEvent==NULL) return false;
    switch(sideEffectId%100)
    {
        case 1:
        {
			DisplayCoordinateOperation* doOp = new mitk::DisplayCoordinateOperation(OpTEST,  posEvent->GetSender(), posEvent->GetDisplayPosition(), posEvent->GetDisplayPosition(), posEvent->GetDisplayPosition());
            if (m_UndoEnabled)	//write to UndoMechanism
            {
                DisplayCoordinateOperation* undoOp = new DisplayCoordinateOperation(OpTEST, m_Sender, m_StartDisplayCoordinate, m_LastDisplayCoordinate, m_CurrentDisplayCoordinate);
                

                OperationEvent *operationEvent = new OperationEvent(this, doOp, undoOp,
																    groupEventId, 
																    objectEventId);
                m_UndoController->SetOperationEvent(operationEvent);
            }

            //execute the Operation
            this->ExecuteOperation(doOp);
            ok = true;
            break;
        }
        case 2:
        {
            int opId;
            if(sideEffectId<1200)
                opId=OpMOVE;
            else
                opId=OpZOOM;
            DisplayCoordinateOperation* doOp = new DisplayCoordinateOperation(opId,  m_Sender, m_StartDisplayCoordinate, m_CurrentDisplayCoordinate, posEvent->GetDisplayPosition());
			if (m_UndoEnabled)	//write to UndoMechanism
            {
                DisplayCoordinateOperation* undoOp = new mitk::DisplayCoordinateOperation(opId,  posEvent->GetSender(), m_StartDisplayCoordinate, m_LastDisplayCoordinate, m_CurrentDisplayCoordinate);


                OperationEvent *operationEvent = new OperationEvent(m_Destination, doOp, undoOp,
																    groupEventId, 
																    objectEventId);
                m_UndoController->SetOperationEvent(operationEvent);
            }

            //make Operation
            m_LastDisplayCoordinate=m_CurrentDisplayCoordinate;
            m_CurrentDisplayCoordinate=posEvent->GetDisplayPosition();
            std::cout << m_CurrentDisplayCoordinate << std::endl;
            
            //execute the Operation
            m_Destination->ExecuteOperation(doOp);
            ok = true;
            break;
        }
        default:
            ok = false;
            break;
    }
    return ok;
}

//##ModelId=3EF2229F00F0
mitk::DisplayVectorInteractor::DisplayVectorInteractor(std::string type, mitk::OperationActor* destination)
: mitk::StateMachine(type), m_Destination(destination),
    m_Sender(NULL),
    m_StartDisplayCoordinate(0,0),
    m_LastDisplayCoordinate(0,0),
    m_CurrentDisplayCoordinate(0,0)
{
	if(m_Destination==NULL)
         m_Destination=this;
}


//##ModelId=3EF2229F010E
mitk::DisplayVectorInteractor::~DisplayVectorInteractor()
{
}


