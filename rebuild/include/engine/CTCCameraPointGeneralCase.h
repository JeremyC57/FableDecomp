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
#ifdef FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS
 // agentFAM03 fix14 (2026-11-11): retail virtuals of CTCCameraPointDefinitionBase, declaration only (needs the polymorphic
 // FABLE_RETAIL_CTCBASE; no layout change). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order
 // on TLC-named slots; spellings from the Ego_r / FableWin name sets.
public:
 virtual bool HasFrameUpdate()const;                                  //  8 0x004D3A1F
 virtual bool IsComponentSerialisable()const;                         // 23 0x004D3A26
 virtual bool IsHeroTheSubject()const;                                // 30 0x004D3A22
 virtual void SetLeafDataInWindow(class CGuiVarTransferStruct&)const; // 31 0x004D3A29
 virtual void GetLeafDataFromWindow(const class CGuiVarTransferStruct&);   // 32 0x004D3A2C
 virtual void OnSerialseLeaf(CPersistContext&);                       // 33 0x004D3A2F
protected:
 virtual void UpdateActiveCameraPosition();                           // 34 0x004D3A32
private:
#endif
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
