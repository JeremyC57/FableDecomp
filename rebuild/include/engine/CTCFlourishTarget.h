#ifndef FABLE_ENGINE_CTCFLOURISHTARGET_H
#define FABLE_ENGINE_CTCFLOURISHTARGET_H
// New shared header (egod e13, row 0x007E8A40 CTCFlourishTarget::ProcessActionMovementDelta): the flourish-target component.
// RTTI .?AVCTCFlourishTarget@@ : CTCBase. Members: the Ego_d.pdb class record (types.txt 0x367C3/0x367C4: private PAttacker
// CIntelligentPointer<const CThing> +12, CullNegativeXAxisMovement bool +20, CullAllYAxisMovement bool +21; sizeof 24), which Ego_r.pdb
// agrees with (struct_layouts_egor.tsv: same names / offsets, size 24). Methods: the Ego_r.pdb / FableWin.pdb member set (ctor 0x004F43C1
// donor, OnInitialActivate / OnRemove virtuals, ?ProcessActionMovementDelta@CTCFlourishTarget@@QBEXAAVC3DVector@@@Z).
// Needs FABLE_RETAIL_CTCBASE and FABLE_RETAIL_CINTELLIGENTPOINTER (defined here when missing, as engine/CTCCreatedEntity.h does).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#ifndef FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ETCInterfaceType.h"
class CCharString;
class CThing;
class C3DVector;

class CTCFlourishTarget : public CTCBase {
public:
    CTCFlourishTarget(CThing&);                                          // ??0CTCFlourishTarget@@QAE@AAVCThing@@@Z
    static CTCBase* __fastcall Alloc(CThing&);                           // ?Alloc@CTCFlourishTarget@@SIPAVCTCBase@@AAVCThing@@@Z
    static CCharString __fastcall StatGetClassTypeName();                // ?StatGetClassTypeName@CTCFlourishTarget@@SI?AVCCharString@@XZ
    virtual ETCInterfaceType GetInterfaceType() const;                   // ?GetInterfaceType@CTCFlourishTarget@@UBE?AW4ETCInterfaceType@@XZ
    virtual CCharString GetClassTypeName() const;                        // ?GetClassTypeName@CTCFlourishTarget@@UBE?AVCCharString@@XZ
    virtual void OnInitialActivate();                                    // ?OnInitialActivate@CTCFlourishTarget@@UAEXXZ 0x007E8A00
    virtual void OnRemove();                                             // ?OnRemove@CTCFlourishTarget@@UAEXXZ 0x007E8A20
    void ProcessActionMovementDelta(C3DVector& movement_delta) const;    // ?ProcessActionMovementDelta@CTCFlourishTarget@@QBEXAAVC3DVector@@@Z 0x007E8A40
#ifdef FABLE_RETAIL_CTCFLOURISHTARGET_SETTERS
    // p25E opt-in (row 0x00859EF0 CCombatActionBase::OnAnimationLoaded): the public setters of FableWin.pdb / Ego_d.pdb
    // ?SetAttacker@CTCFlourishTarget@@QAEXABVCThing@@@Z (FableWin 0x02828860, 95 bytes; out of line in retail at 0x0085AB00: a temporary
    // CIntelligentPointer<const CThing> on the argument, its item stored into PAttacker +0x0C), ?SetAsCullNegativeXAxisMovement@ /
    // ?SetAsCullAllYAxisMovement@CTCFlourishTarget@@QAEXXZ (FableWin.pdb debug\combat_action_base.obj 0x028288E0 / 0x02828900, 18-byte /Od
    // bodies, right after SetAttacker in the same module; no Ego_r.pdb or Fable.exe copy: expanded, 0x0085A34C / 0x0085A350 store the bytes
    // CullAllYAxisMovement +0x15 / CullNegativeXAxisMovement +0x14 in place). No layout change.
    void SetAttacker(const CThing& attacker);                            // 0x0085AB00
    void SetAsCullNegativeXAxisMovement() { CullNegativeXAxisMovement = true; }
    void SetAsCullAllYAxisMovement() { CullAllYAxisMovement = true; }
#endif
private:
    CIntelligentPointer<const CThing> PAttacker;                         // +0x0C
    bool CullNegativeXAxisMovement;                                      // +0x14
    bool CullAllYAxisMovement;                                           // +0x15
public:
    static void CheckRetailLayout() { FABLE_STATIC_ASSERT_AT(39, offsetof(CTCFlourishTarget, PAttacker) == 0x0c); FABLE_STATIC_ASSERT_AT(39, offsetof(CTCFlourishTarget, CullAllYAxisMovement) == 0x15); }
};
FABLE_STATIC_ASSERT_AT(41, sizeof(CTCFlourishTarget) == 0x18);
#endif
