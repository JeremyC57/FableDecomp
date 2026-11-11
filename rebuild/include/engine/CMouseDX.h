#ifndef FABLE_ENGINE_CMOUSEDX_H
#define FABLE_ENGINE_CMOUSEDX_H
#include "stlport/fable_containers.h"
#include "engine/CMaintainedEvent.h"
#include "engine/CMouse.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/DirectInputTypes.h"
class CInputManagerDX;
// Layout: Ego_r full CMouseDX fields, native base0x3414 (STLport list4), total0x4860.
// Native ResetNonExclusiveMouse and landed destructor/PeekScreenPos prove all accessed offsets.
class CMouseDX:public CMouse {
protected:
 C2DVector Pos; IDirectInputDevice8W* DMouse; CInputManagerDX& InputManagerDX;
 bool Acquired; float MouseStep;
 bool NonExclusiveOldButton0Pressed,NonExclusiveOldButton1Pressed,NonExclusiveOldButton2Pressed,NonExclusiveOldButton3Pressed,NonExclusiveOldButton4Pressed;
 float NonExclusiveOldMouseX,NonExclusiveOldMouseY;
 bool ExclusiveMode,InputEnabled,ForceRecentre,DrawingCursor;
 DIDEVICEOBJECTDATA ExclusiveModeData[256];
 long ExclusiveModeEventsCount,NoExclusiveModeEvents;
 bool LMBOn,MMBOn,RMBOn,Filtering;
 C3DVector LastUpdateMovement; CCountedPointer<CSurface> CursorGraphic;
 void ResetNonExclusiveMouse();
public:
 CMouseDX(CInputManagerDX&,bool);
 virtual ~CMouseDX();
 virtual void GetPos(C2DVector*)const;
 virtual const C2DVector& PeekPos()const;
 virtual C2DVector PeekScreenPos()const;
 virtual void SetPos(const C2DVector*);
 virtual void Update();
 virtual bool SetExclusiveMode(bool);
 virtual void SetInputOn(bool);
 virtual void SetRecentringOn(bool);
 virtual bool IsRecentringOn()const;
 virtual void SetDrawCursor(bool);
 virtual bool IsDrawingCursor()const;
 virtual void SetMouseFiltering(bool);
 virtual void SetMouseCursor(const CSurface&);
 virtual void SetHasInputFocus(bool);
 virtual bool IsLMBDown()const;
 virtual bool IsMMBDown()const;
 virtual bool IsRMBDown()const;
};
FABLE_STATIC_ASSERT(sizeof(CMouseDX)==0x4860);
#endif
