#ifndef FABLE_ENGINE_CTCCAMERAPOINTGENERALCASE_H
#define FABLE_ENGINE_CTCCAMERAPOINTGENERALCASE_H
#include "engine/CTCBase.h"
class CTCCameraPointDefinitionBase:public CTCBase {
public:
 float GetFOV()const;
protected:
 bool CutInto,CutOutOf,TestAngleBeforeActivation;
 float FOV;
 bool SelfTerminate,HeroIsSubject;
 unsigned char CoordBase[12]; // Original C3DVector at22, retains native unaligned field storage.
 unsigned char CoordAxis[24]; // Original CRightHandedSet at34.
 bool UsingRelativeCoords,UsingRelativeOrientation;
private:
 bool Activated;
};
class CTCCameraPointShapeActivationBase:public CTCCameraPointDefinitionBase{};
class CTCCameraPointGeneralCase:public CTCCameraPointShapeActivationBase {
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return (ETCInterfaceType)63;}
 float GetStringLength()const;
 float GetCageRadius()const;
 float GetHeightOffset()const;
 bool IsAutoGoBehind()const;
 float GetAutoGoBehindTime()const;
private:
 float CameraStringLength,CameraCageRadius,CameraHeightOffset;
 bool AllowRightStickZoom,AllowRightStickRotation,AllowZTarget,AutoGoBehind;
 float AutoGoBehindTime;
};
FABLE_STATIC_ASSERT(sizeof(CTCCameraPointDefinitionBase)==64);
FABLE_STATIC_ASSERT(sizeof(CTCCameraPointGeneralCase)==84);
#endif
