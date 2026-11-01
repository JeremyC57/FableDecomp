// CTCFishingRod::~CTCFishingRod() -- implicit component destructor: release the def pointer PDef (+0xc), then CTCBase::~CTCBase (tail jump)
// Leaf: ??1CTCFishingRod@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E24D0, 35 bytes: push esi ; mov esi,ecx ; mov ecx,[esi+0Ch] ; test ecx,ecx ; je ;
//   dec dword ptr [ecx+4] ; jne ; mov eax,[ecx] ; call [eax+4] ; mov dword ptr [esi+0Ch],0 ; mov ecx,esi ; pop esi ;
//   jmp 0x00686830.
// Identity (proven): the only caller is 0x007E24B0 (call at 0x007E24B3), slot 0 (scalar deleting destructor) of vtable
//   0x01270104, whose RTTI is .?AVCTCFishingRod@@ (rtti-vtables.tsv); RTTI base CTCFishingRod : CTCBase. Ego_r.pdb lists
//   CTCFishingRod's Alloc, ctor ??0CTCFishingRod@@QAE@AAVCThing@@@Z, GetInterfaceType and GetClassTypeName and NO destructor
//   (FableWin.pdb, debug, has the generated ??1CTCFishingRod@@UAE@XZ): the destructor is compiler-generated, hence no vtable
//   store (VC7.1 drops it in an implicit destructor) and the straight tail jump to CTCBase::~CTCBase 0x00686830.
//   The manifest label "C3DAnimationBlendState::~C3DAnimationBlendState" is propagated from the byte-identical twin family.
// Callees: the def's virtual slot 1 (CIVCountedPointeeBase::Delete, `call [eax+4]`) when the count drops to zero,
//   CTCBase::~CTCBase 0x00686830 (tail jump).
// Layout: Ego_r.pdb CTCFishingRod (16 bytes) : CTCBase (0xC) with PDef (CDefPointer<const CFishingRodDef>) +0xC; retail
//   identical. CDefPointer is the retail engine model (engine/CDefPointer.h: Release() holds the pointee in a local, so
//   `dec [ecx+4] ; jne`). engine/CTCBase.h flattens the vptr into a data member, so the polymorphic base chain is the landed
//   CTCHeroPodium model (0x00771F30): CBaseClass -> CBaseClassNonCopyable -> CTCBase.
// Shape: the class declares no destructor; the constructor is defined here (as the CTCFishingRod TU does) so the vftable,
//   the scalar deleting destructor and with them the implicit destructor are emitted.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CBaseClassNonCopyable.h"
class CThing;
class CFishingRodDef;

class CTCBase : public CBaseClassNonCopyable {
public:
    CTCBase(CThing& parent);                                // 0x00686800
    virtual ~CTCBase();                                     // 0x00686830
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCFishingRod : public CTCBase {                      // vtable 0x01270104
public:
    CTCFishingRod(CThing& parent);
    CDefPointer<const CFishingRodDef> PDef;                 // +0x0C
};

CTCFishingRod::CTCFishingRod(CThing& parent)
    : CTCBase(parent)
{
}