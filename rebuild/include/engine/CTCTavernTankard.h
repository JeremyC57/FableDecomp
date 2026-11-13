#ifndef FABLE_ENGINE_CTCTAVERNTANKARD_H
#define FABLE_ENGINE_CTCTAVERNTANKARD_H
// New shared header (w22 agent2002, row 0x007B90F0 CTCTavernTable::OnInitialActivate): the tavern tankard component
// (TCI_TAVERN_TANKARD 186). RTTI .?AVCTCTavernTankard@@ : CTCBase. Members: Ego_r.pdb (struct_layouts_egor.tsv, sizeof 0x3C, no
// containers so the retail offsets are the PDB's): Fullness +0x0C, RHSet +0x10, Pos +0x28, PBarmaid +0x34 (CIntelligentPointer, byte-packed).
// OnInitialActivate stores a whole CRightHandedSet at +0x10 and a C3DVector at +0x28 (0x007B938C..0x007B93CE). Methods: the Ego_r.pdb /
// FableWin.pdb member set; StatGetInterfaceType inline. Needs FABLE_RETAIL_CTCBASE and FABLE_RETAIL_CINTELLIGENTPOINTER (defined here if absent).
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
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/ETCInterfaceType.h"
class CCharString;
class CThing;
class CThingAICreature;

class CTCTavernTankard : public CTCBase {
public:
    CTCTavernTankard(CThing&);                                           // ??0CTCTavernTankard@@QAE@AAVCThing@@@Z
    static CTCBase* __fastcall Alloc(CThing&);                           // ?Alloc@CTCTavernTankard@@SIPAVCTCBase@@AAVCThing@@@Z
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_TAVERN_TANKARD; }   // (inline)
    static CCharString __fastcall StatGetClassTypeName();                // ?StatGetClassTypeName@CTCTavernTankard@@SI?AVCCharString@@XZ
    virtual void OnInitialActivate();                                    // ?OnInitialActivate@CTCTavernTankard@@UAEXXZ
    virtual ETCInterfaceType GetInterfaceType() const;                   // ?GetInterfaceType@CTCTavernTankard@@UBE?AW4ETCInterfaceType@@XZ
    virtual CCharString GetClassTypeName() const;                        // ?GetClassTypeName@CTCTavernTankard@@UBE?AVCCharString@@XZ
    CThingAICreature* GetBarmaid();                                      // ?GetBarmaid@CTCTavernTankard@@QAEPAVCThingAICreature@@XZ
    void SetBarmaid(CThingAICreature*);                                  // ?SetBarmaid@CTCTavernTankard@@QAEXPAVCThingAICreature@@@Z
    void ReplaceTankard();                                               // ?ReplaceTankard@CTCTavernTankard@@QAEXXZ
    float                                 Fullness;                      // +0x0C
    CRightHandedSet                       RHSet;                         // +0x10
    C3DVector                             Pos;                           // +0x28
    CIntelligentPointer<CThingAICreature> PBarmaid;                      // +0x34
};
FABLE_STATIC_ASSERT(offsetof(CTCTavernTankard, RHSet) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CTCTavernTankard, Pos) == 0x28);
FABLE_STATIC_ASSERT(sizeof(CTCTavernTankard) == 0x3C);
#endif
