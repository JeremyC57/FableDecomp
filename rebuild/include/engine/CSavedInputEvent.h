#ifndef FABLE_ENGINE_CSAVEDINPUTEVENT_H
#define FABLE_ENGINE_CSAVEDINPUTEVENT_H
#include "engine/CInputEvent.h"
#include "engine/C2DVector.h"
#include "engine/CCamera.h"
// Layout: complete original Ego19ee7/FW1a8d7,size120.
class CSavedInputEvent{
public:
 unsigned long Frame;
 CInputEvent InputEvent;
 C2DVector MousePos;
 CCamera Camera;
 CSavedInputEvent(const CSavedInputEvent & p0);
 CSavedInputEvent();
 ~CSavedInputEvent();
};
FABLE_STATIC_ASSERT(sizeof(CSavedInputEvent)==120);
#endif
