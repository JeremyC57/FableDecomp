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
private:
    CIntelligentPointer<const CThing> PAttacker;                         // +0x0C
    bool CullNegativeXAxisMovement;                                      // +0x14
    bool CullAllYAxisMovement;                                           // +0x15
public:
    static void CheckRetailLayout() { FABLE_STATIC_ASSERT(offsetof(CTCFlourishTarget, PAttacker) == 0x0c); FABLE_STATIC_ASSERT(offsetof(CTCFlourishTarget, CullAllYAxisMovement) == 0x15); }
};
FABLE_STATIC_ASSERT(sizeof(CTCFlourishTarget) == 0x18);
#endif
