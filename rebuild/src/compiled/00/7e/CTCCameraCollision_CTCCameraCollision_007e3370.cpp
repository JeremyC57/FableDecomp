// CTCCameraCollision::~CTCCameraCollision() -- implicit component destructor: release the def pointer CameraCollisionDef (+0xc), then CTCBase::~CTCBase (tail jump)
// Leaf: ??1CTCCameraCollision@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E3370, 35 bytes: push esi ; mov esi,ecx ; mov ecx,[esi+0Ch] ; test ecx,ecx ; je ;
//   dec dword ptr [ecx+4] ; jne ; mov eax,[ecx] ; call [eax+4] ; mov dword ptr [esi+0Ch],0 ; mov ecx,esi ; pop esi ;
//   jmp 0x00686830.
// Identity (proven): the only caller is 0x007E3350 (call at 0x007E3353), slot 0 (scalar deleting destructor) of vtable
//   0x01270394, whose RTTI is .?AVCTCCameraCollision@@ (rtti-vtables.tsv); RTTI bases CTCCameraCollision : CTCBase :
//   CBaseClassNonCopyable : CBaseClass (ghidra_out/rtti_bases.tsv). Ego_r.pdb lists CTCCameraCollision's Alloc, ctor
//   ??0CTCCameraCollision@@QAE@AAVCThing@@@Z, OnCreate, GetInterfaceType, GetClassTypeName and NO destructor; FableWin.pdb
//   (debug) has the generated ??1CTCCameraCollision@@UAE@XZ: the destructor is compiler-generated, hence no vtable store and
//   the straight tail jump to CTCBase::~CTCBase 0x00686830. The manifest label "C3DAnimationBlendState::~C3DAnimationBlendState"
//   is propagated from the byte-identical family (landed twins 0x005DAE40, 0x007E24D0 ~CTCFishingRod).
// Callees: the def's virtual slot 1 (CIVCountedPointeeBase::Delete, `call [eax+4]`) when its count drops to zero,
//   CTCBase::~CTCBase 0x00686830 (tail jump).
// Layout: Ego_r.pdb CTCCameraCollision (16 bytes) : CTCBase (0xC) with CameraCollisionDef
//   (CDefPointer<const CCameraCollisionDef>) +0xC; retail identical. CDefPointer is the retail engine model
//   (engine/CDefPointer.h). engine/CTCBase.h flattens the vptr into a data member, so the polymorphic base chain is the landed
//   CTCFishingRod model (0x007E24D0): CBaseClass -> CBaseClassNonCopyable -> CTCBase.
// Shape: the class declares no destructor; its constructor is defined here (as the CTCCameraCollision TU does) so the
//   vftable, the scalar deleting destructor and with them the implicit destructor are emitted.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CBaseClassNonCopyable.h"
class CThing;
class CCameraCollisionDef;

class CTCBase : public CBaseClassNonCopyable {
public:
    CTCBase(CThing& parent);                                // 0x00686800
    virtual ~CTCBase();                                     // 0x00686830
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCCameraCollision : public CTCBase {                 // vtable 0x01270394
public:
    CTCCameraCollision(CThing& parent);
    CDefPointer<const CCameraCollisionDef> CameraCollisionDef;   // +0x0C
};

CTCCameraCollision::CTCCameraCollision(CThing& parent)
    : CTCBase(parent)
{
}