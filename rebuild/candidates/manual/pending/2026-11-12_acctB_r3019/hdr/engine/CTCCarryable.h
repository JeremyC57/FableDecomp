#ifndef FABLE_ENGINE_CTCCARRYABLE_H
#define FABLE_ENGINE_CTCCARRYABLE_H
// CTCCarryable (new shared header, chunk 1002, row 0x005CB974 CTCInventoryWeapons::OnInitialActivate): the retail CTCCarryable over
// FABLE_RETAIL_CTCBASE (RTTI .?AVCTCCarryable@@ : CTCBase). Members: Ego_r.pdb CTCCarryable (struct_layouts_egor.tsv, 0x24 bytes) in order
// and spelling: the row tests PCarryingTC +0x10 in place (`cmp dword ptr [edi+10h],0`). Methods: Ego_r.pdb / FableWin.pdb
// ?GetPCarriedByThing@CTCCarryable@@QBEPBVCThing@@XZ (0x00705170, landed) and ?GetCarrySlotActive@CTCCarryable@@QBEJXZ (0x00705CC0, landed).
// StatGetInterfaceType: TCI_CARRYABLE (71; FableWin ??$PeekTC@$$CBVCTCCarryable@@@CThing@@..., retail tests bit 0x47).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CThing;
class CTCCarrying;
class CTCCarryable : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CARRYABLE; }
    const CThing* GetPCarriedByThing() const;                        // 0x00705170
#ifdef FABLE_RETAIL_CTCCARRYABLE_NONCONST_CARRIER
    // Opt-in (w22 agent2007, rows 0x006DE8C0 / 0x006DEEE0 CTCTrophy): Ego_r.pdb / FableWin.pdb ?GetPCarriedByThing@CTCCarryable@@QAEPAVCThing@@XZ,
    // the non-const twin, out of line at 0x00705180 right after the const copy 0x00705170 (byte-identical bodies; the trophy rows
    // call 0x00705180). Declaration only.
    CThing* GetPCarriedByThing();                                    // 0x00705180
#endif
    long GetCarrySlotActive() const;                                 // 0x00705CC0
#ifdef FABLE_RETAIL_CTCCARRYABLE_SETASCARRIEDBY
    // Opt-in (w22 agent1016, row 0x006A16A0 CTCCarrying::RemoveThingInCarrySlot calls it with 0): Ego_r.pdb / FableWin.pdb
    // ?SetAsCarriedBy@CTCCarryable@@AAEXPAVCTCCarrying@@@Z (private; out of line at 0x00705D40, landed). CTCCarrying calls the private
    // member, so it is a friend (inferred from the access: the PDBs keep no friend records). Declaration only: no layout change.
    friend class CTCCarrying;
private:
    void SetAsCarriedBy(CTCCarrying* carrier);                       // 0x00705D40
public:
#endif
    bool          ForceFragility;                                    // +0x0c
    CTCCarrying*  PCarryingTC;                                       // +0x10
    float         OffsetCoordRelativeToAttachToDummy[3];             // +0x14 C3DVector
    float         OffsetAmount;                                      // +0x20
};
FABLE_STATIC_ASSERT(sizeof(CTCCarryable) == 0x24);
#endif // FABLE_ENGINE_CTCCARRYABLE_H
