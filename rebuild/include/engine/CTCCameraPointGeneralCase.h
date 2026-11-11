#ifndef FABLE_ENGINE_CTCCAMERAPOINTGENERALCASE_H
#define FABLE_ENGINE_CTCCAMERAPOINTGENERALCASE_H
#include "engine/CTCBase.h"
#ifdef FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS3
// agentFAM05 (2026-11-11): the camera-point type every leaf's GetCameraPointType returns (Ego_r.pdb CIsCameraPointOfType::Type, mangled
// W4ECameraPointType@@; the enumerators are not in the name sets, so the leaves write the retail values as casts).
enum ECameraPointType { };
#endif // FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS3
#ifdef FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VTABLE
// agentFAM05 (2026-11-11): the camera-point type enum (as in the _VSLOTS3 block) for the ordered vtable block below, when that macro is not defined.
#ifndef FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS3
enum ECameraPointType { };
#endif
#endif // FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VTABLE
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
#ifdef FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS3
// agentFAM05 (2026-11-11): retail virtuals of CTCCameraPointDefinitionBase that FAM03's block lacks, declaration only (needs the polymorphic
// FABLE_RETAIL_CTCBASE; no layout change). Slots from the TLC vtable 0x01269B04 (agentFAM03 proof, re-verified by agentFAM05):
// slot 21 0x004D3A1B is shared by every camera-point vtable, slot 28 is _purecall here and holds each leaf's GetCameraPointType.
public:
 virtual ETCInterfaceType GetInterfaceType()const;                     // 21 0x004D3A1B
 virtual void FrameUpdate();                                           //  9 0x0075C5F0
 virtual ECameraPointType GetCameraPointType()const=0;                 // 28 _purecall
private:
#endif // FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS3
#ifdef FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VTABLE
// agentFAM05 (2026-11-11): the virtuals CTCCameraPointDefinitionBase declares, in TLC slot order (vtable 0x01269B04 aligned with the Xbox 360 Test PDB
// order on TLC-named slots, agentFAM03; spellings from the Ego_r / FableWin name sets), for bodies that call them through the vtable
// (FrameUpdate 0x0075C5F0 calls slot 29 `[eax+74h]` and slot 34 `[edx+88h]`). Use it instead of _VSLOTS / _VSLOTS3 (those declare the
// same functions out of slot order). Also the private IsAngleValid (Ego_r.pdb ?IsAngleValid@CTCCameraPointDefinitionBase@@ABE_NXZ,
// 0x0075C530). Declarations only (needs FABLE_RETAIL_CTCBASE; no layout change).
public:
 virtual bool HasFrameUpdate()const;                                  //  8 0x004D3A1F
 virtual void FrameUpdate();                                          //  9 0x0075C5F0
 virtual ETCInterfaceType GetInterfaceType()const;                     // 21 0x004D3A1B
 virtual bool IsComponentSerialisable()const;                         // 23 0x004D3A26
 virtual ECameraPointType GetCameraPointType()const=0;                 // 28 _purecall
 virtual bool IsLeafCriteriaMet();                                    // 29
 virtual bool IsHeroTheSubject()const;                                // 30 0x004D3A22
 virtual void SetLeafDataInWindow(class CGuiVarTransferStruct&)const; // 31 0x004D3A29
 virtual void GetLeafDataFromWindow(const class CGuiVarTransferStruct&);   // 32 0x004D3A2C
 virtual void OnSerialseLeaf(CPersistContext&);                       // 33 0x004D3A2F
protected:
 virtual void UpdateActiveCameraPosition();                           // 34 0x004D3A32
private:
 bool IsAngleValid()const;                                            // 0x0075C530
#endif // FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VTABLE
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
#ifdef FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS3
// agentFAM05 (2026-11-11): the leaf override of slot 28 (TLC vtable of CTCCameraPointGeneralCase; agentFAM03 proof, re-verified by agentFAM05). Declaration only.
 virtual ECameraPointType GetCameraPointType()const;                   // 28 0x004D3B88
#endif // FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS3
private:
 float CameraStringLength,CameraCageRadius,CameraHeightOffset;
 bool AllowRightStickZoom,AllowRightStickRotation,AllowZTarget,AutoGoBehind;
 float AutoGoBehindTime;
};
FABLE_STATIC_ASSERT(sizeof(CTCCameraPointDefinitionBase)==64);
FABLE_STATIC_ASSERT(sizeof(CTCCameraPointGeneralCase)==84);
#endif
