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
#ifdef FABLE_RETAIL_CTCCARRYABLE_ISCARRIED
    // Retail opt-in (p25F, row 0x00714370): ?IsCarried@CTCCarryable@@QBE_NXZ (PDB name; inline in retail, which tests PCarryingTC +0x10
    // in place at 0x00714517).
    bool IsCarried() const { return PCarryingTC != 0; }
#endif
#ifdef FABLE_RETAIL_CTCCARRYABLE_GETCARRYSLOTPASSIVE
    // NF167 opt-in (q4475, row 0x00968BC0 CAIStateGroup_TavernKeeper::OnReplace): Ego_r.pdb / FableWin.pdb
    // ?GetCarrySlotPassive@CTCCarryable@@QBEJXZ, out of line at 0x00705D00 (landed self row; the GetCarrySlotActive twin). Declaration only.
    long GetCarrySlotPassive() const;                                // 0x00705D00
#endif
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
#ifdef FABLE_RETAIL_CARRYABLE_POSE_PASS29
    bool GetCarriedPositionInfo(C3DVector&,CRightHandedSet&,bool,float) const;
private:
    void GetInterpolatedPickupOffsetAndOrientation(C3DVector&,CRightHandedSet&,bool,float) const;
    void GetInterpolatedPickupOffsetAndOrientationAt(const C3DVector&,const CRightHandedSet&,C3DVector&,CRightHandedSet&,bool) const;
#endif
    CTCCarrying*  PCarryingTC;                                       // +0x10
#ifdef FABLE_RETAIL_CARRYABLE_POSE_PASS29
    C3DVector OffsetCoordRelativeToAttachToDummy;
#else
    float         OffsetCoordRelativeToAttachToDummy[3];
#endif             // +0x14 C3DVector
    float         OffsetAmount;                                      // +0x20
#ifdef FABLE_RETAIL_CARRYABLE_POSE_PASS29
public:
#endif
#ifdef FABLE_RETAIL_CTCCARRYABLE_VSLOTS3
// agentFAM05 (2026-11-11): retail virtual overrides this declaration lacks, declaration only (needs the polymorphic FABLE_RETAIL_CTCBASE; no layout
// change). Slots of the TLC vtable 0x0123B35C (agentFAM03 proof, re-verified by agentFAM05); Ego_r / FableWin spellings.
    virtual void OnKill();                                           // slot 14 0x00705150
    virtual void OnInitialActivate();                                // slot 6  0x00705C30
#endif // FABLE_RETAIL_CTCCARRYABLE_VSLOTS3
};
FABLE_STATIC_ASSERT_AT(46, sizeof(CTCCarryable) == 0x24);
#endif // FABLE_ENGINE_CTCCARRYABLE_H
