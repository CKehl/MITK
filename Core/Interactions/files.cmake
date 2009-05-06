SET(CPP_FILES
  mitkAction.cpp
  mitkAffineInteractor.cpp
  mitkAutoSegmentationTool.cpp
  mitkConferenceEventMapper.cpp
  mitkConnectPointsInteractor.cpp
  mitkContourInteractor.cpp
  mitkContourTool.cpp
  mitkCoordinateSupplier.cpp
  mitkDisplayCoordinateOperation.cpp
  mitkDisplayInteractor.cpp
  mitkDisplayPointSetInteractor.cpp
  mitkDisplayPositionEvent.cpp
  mitkDisplayVectorInteractor.cpp
  mitkEvent.cpp
  mitkEventDescription.cpp
  mitkEventMapper.cpp
  mitkExtrudedContourInteractor.cpp
  mitkFeedbackContourTool.cpp
  mitkGlobalInteraction.cpp
  # mitkInteractionDebug.cpp
  mitkInteractionDebugger.cpp
  mitkInteractor.cpp
  mitkPointInteractor.cpp
  mitkPointSelectorInteractor.cpp
  mitkPointSetInteractor.cpp
  mitkPositionEvent.cpp
  mitkPositionTracker.cpp
  mitkSeedsInteractor.cpp
  mitkSegmentationsProcessingTool.cpp
  mitkSegTool2D.cpp
  mitkSetRegionTool.cpp
  mitkSocketClient.cpp
  mitkState.cpp
  mitkStateEvent.cpp
  mitkStateMachine.cpp
  mitkStateMachineFactory.cpp
  mitkSurfaceInteractor.cpp
  mitkTool.cpp
  mitkTransition.cpp
)

# Segmentation (or related) tools for QmitkSlicebasedSegmentation
#
# Tool classes, which do not appear in this list but in the CPP_FILES
# list, do not need a factory. The list below contains only "end-user"
# tools, which should be instantiated during runtime
SET( TOOL_FILES
  mitkAddContourTool.cpp
  mitkAutoCropTool.cpp
  mitkBinaryThresholdTool.cpp
  mitkCalculateGrayValueStatisticsTool.cpp
  mitkCalculateVolumetryTool.cpp
  mitkCorrectorTool2D.cpp
  mitkCreateSurfaceTool.cpp
  mitkEraseRegionTool.cpp
  mitkFillRegionTool.cpp
  mitkRegionGrowingTool.cpp
  mitkSubtractContourTool.cpp
  mitkPaintbrushTool.cpp
  mitkDrawPaintbrushTool.cpp
  mitkErasePaintbrushTool.cpp
)

IF(MITK_USE_TD_MOUSE)
  SET(CPP_FILES ${CPP_FILES} mitkTDMouseEvent.cpp mitkSpaceNavigatorDriver.cpp mitkTDMouseEventThrower.cpp)
ENDIF(MITK_USE_TD_MOUSE)

