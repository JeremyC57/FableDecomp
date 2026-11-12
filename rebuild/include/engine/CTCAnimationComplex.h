#ifndef FABLE_ENGINE_CTCANIMATIONCOMPLEX_H
#define FABLE_ENGINE_CTCANIMATIONCOMPLEX_H
// CTCAnimationComplex (new shared header, chunk 1006, row 0x005B3E4E CTCInventoryClothing::GlobalUpdate): the animation component
// (RTTI .?AVCTCAnimationComplex@@ : CTCAnimationBase : CTCBase; Ego_r.pdb sizeof 0x10, PAnimation +0x0C; members not modelled).
// StatGetInterfaceType: TCI_ANIMATION (90; the `push 5Ah ; pop edi` of the caller's inline GetTC, 0x005B3F95). Out of line:
// ?GetAnimation@CTCAnimationComplex@@QAEAAVCAnimateThing@@XZ (Ego_r.pdb / FableWin.pdb, retail 0x0070B460). Declarations only.
#ifdef FABLE_RETAIL_CTCANIMATIONCOMPLEX_FULL
// Full-class opt-in (Codex pass 17, helper header of row 0x004C1060 GetFocalPos): the retail 16-byte class on its real base
// CTCAnimationBase with its virtuals (native ctor 0x0070B3F0) and the inline PeekAnimation reading PAnimation +0x0C.
#include "engine/CTCAnimationBase.h"
#include "engine/CAnimateThing.h"
// Original16-byte class; native ctor70B3F0 and original PeekAnimation read PAnimation+Ch.
class CTCAnimationComplex : public CTCAnimationBase {
public:
 CTCAnimationComplex(CThing&);
 virtual ~CTCAnimationComplex();
 virtual CCharString GetClassTypeName()const;
 virtual void GetInterpolatedMeshAnim(C3DAnimationBlendState&,float)const;
 virtual void GetMeshAnims(C3DAnimationBlendState&,C3DAnimationBlendState&)const;
 const CAnimateThing& PeekAnimation()const;
#ifdef FABLE_RETAIL_CTCANIMATIONCOMPLEX_ONCREATE
 // Nested opt-in (agentNF11, row 0x0070B4D0): Ego_r.pdb / FableWin.pdb ?OnCreate@CTCAnimationComplex@@UAEXXZ, the CTCBase slot 4
 // override (vtable 0x012650A4 slot 4 = 0x0070B4D0). No vtable change (an override).
 virtual void OnCreate();
#endif
protected:
 CAnimateThing* PAnimation;
#ifdef FABLE_RETAIL_CTCANIMATIONCOMPLEX_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x0070B600
public:
#endif
#ifdef FABLE_RETAIL_CTCANIMATIONCOMPLEX_VSLOTS3
// agentFAM05 (2026-11-11): retail virtual(s) this declaration lacks, declaration only (no layout change; the vtable exists). Slots from the TLC
// vtable (agentFAM03 proof, re-verified by agentFAM05); spellings from the Ego_r / FableWin name sets.
public:
    virtual void OnFrameUpdate();  // slot 31, 0x0070B480
public:
#endif // FABLE_RETAIL_CTCANIMATIONCOMPLEX_VSLOTS3
};
FABLE_STATIC_ASSERT(sizeof(CTCAnimationComplex)==16);
#else
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CAnimateThing;
class CTCAnimationComplex : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_ANIMATION; }
    CAnimateThing& GetAnimation();                           // 0x0070B460
#ifdef FABLE_RETAIL_CTCANIMATIONCOMPLEX_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x0070B600
public:
#endif
};
#endif // FABLE_RETAIL_CTCANIMATIONCOMPLEX_FULL
#endif // FABLE_ENGINE_CTCANIMATIONCOMPLEX_H
