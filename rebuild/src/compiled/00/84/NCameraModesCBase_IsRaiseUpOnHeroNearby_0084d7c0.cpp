// NCameraModes::CBase::IsRaiseUpOnHeroNearby() const -- returns the camera mode def's RaiseUpWhenClose
// Leaf: ?IsRaiseUpOnHeroNearby@CBase@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084D7C0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 7 of NCameraModes::CPCMouseLook's vtable 0x01277784 (the slot holds this body in 14 vtable(s), owner NCameraModes::CBase); the Xbox 360 Test PDB virtual order (third source) aligned on 9 TLC-named slots of that vtable puts IsRaiseUpOnHeroNearby(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsRaiseUpOnHeroNearby@CBase@NCameraModes@@UBE_NXZ; `ret` matches the signature.
// Was: _unattributed::vfunc_7 (rebuild/src/compiled/00/84/unattributed_vfunc7_0084d7c0.cpp).
// No shared header for CCameraModeDef: minimal local definition, the Ego_r.pdb members 4 lower in retail (the packed 0x39-byte
// CParentDefClassBase, as engine/NUISystem_CUIStateDef.h), up to the one the body reads.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CPARENTDEFCLASSBASE
#define FABLE_PDCB_TYPED_MEMBERS
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/CParentDefClassBase.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#include "engine/NCameraModes_CBase.h"
class CCameraModeDef : public CParentDefClassBase {
public:
    unsigned char _pad_39[3];
    long  CameraMode;                                               // +0x3c
    float StringLengthXY;                                           // +0x40
    float CageRadius;                                               // +0x44
    float HeightOffsetZ;                                            // +0x48
    float LookOffsetZ;                                              // +0x4c
    float FOV;                                                      // +0x50
    float RollAngle;                                                // +0x54
    long  TransitionInFrames;                                       // +0x58
    float LazyLookAtPosWeight;                                      // +0x5c
    bool  RaiseUpWhenClose;                                         // +0x60
    bool  StretchAroundHero;                                        // +0x61
};
namespace NCameraModes {
bool CBase::IsRaiseUpOnHeroNearby() const
{
    return GetCameraModeDef()->RaiseUpWhenClose;
}
}
