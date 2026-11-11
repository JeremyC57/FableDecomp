// CCreatureAction_Drink::FinishAction() -- when the drink prop is still alive: kill it, drop whatever the creature carries in its primary slot, and when the jug/cup object is alive hand it back to the creature's carry slot (first removing it from the creature when the creature itself is its carrier; another carrier leaves it alone); then the base FinishAction
// Leaf: FinishAction
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z; 004365b0: ?LowerBound@CInterfaceMap@@QBEPBUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Grade: RELOCATION_MATCH -- the other-carrier arm jumps to the shared base FinishAction tail (goto done) instead of its own call+return, so VC7.1 shrink-wraps the ebx/edi pushes into the alive branch as retail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00847C90, length 348 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80, CVectorMap::LowerBound @ 0x0040F020
// (inlined GetInterface for CTCCarrying = type 0x46, bit 6 of +0x28) and the const instantiation @ 0x004365B0 (const GetTC for
// CTCCarriedObject = type 0x47, bit 7 of +0x28: `test cl,cl ; jns`), CTCCarrying::RemoveThingFromPrimarySlot(bool) @ 0x006A1E00,
// CTCCarryable::GetPCarriedByThing @ 0x00705170 (manifest "CGuiControlListBox::GetCurrentSelectedEntryData" is propagated),
// CTCCarrying::RemoveThing(CThing*) @ 0x006A19B0, CTCCarriedObject::GetCarrySlot() @ 0x00705CC0 (manifest "CTCTavernGame::GetBanter"
// is propagated), CTCCarrying::PutThingInCarrySlot(CThing&, long, bool) @ 0x006A1F80, CCreatureActionBase::FinishAction @ 0x00694430.
// Shape: IS_THING_ALIVE on the two intelligent pointers at +0xa8 and +0xb0 (null test, then bit 0 of +0x91); the first lookup is an
// untested out-parameter GetInterface (absent arm reads the dead word), the second a `p = 0; if (HasTC) p = GetTC` preload through
// the const map; `this` is spilled to the frame because ebp holds the carrier for the compare; the "other carrier" arm falls
// straight to the base call. Retail CCombatActionBase/CCreatureActionBase is 0x114 bytes, so `this` is modelled locally.
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCCarrying { public:
    CThing* RemoveThingFromPrimarySlot(bool drop);     // 0x006A1E00
    void RemoveThing(CThing& thing);                   // 0x006A19B0   // agentNF19: ?RemoveThing@CTCCarrying@@QAEXAAVCThing@@@Z (Ego_r/FableWin: by reference)
    void PutThingInCarrySlot(CThing& thing, long slot, bool immediately);   // 0x006A1F80 (Ego_r/FableWin ?PutThingInCarrySlot@CTCCarrying@@QAEXAAVCThing@@J_N@Z)
};
class CTCCarryable { public:
    CThing const* GetPCarriedByThing() const;   // 0x00705170 (Ego_r.pdb ?GetPCarriedByThing@CTCCarryable@@QBEPBVCThing@@XZ)
};
struct CTCCarriedObject {
    unsigned char _pad[0x10];
    CThing* m_pCarrier;                     // +0x10
    long GetCarrySlot() const;              // 0x00705CC0
};
class CBaseObject;
struct CBaseIntelligentPointer {            // CIntelligentPointer<CThing>: vptr + item
    virtual void v00();
    CThing* m_pItem;                        // +4
    CBaseObject* GetPItem() const;          // 0x00A01B50 ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

class CCreatureActionBase { public:                // retail sizeof 0xa8 (PCreature +0x08; the PDB header is 12 bytes larger)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x00694430
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};
class CCreatureAction_Drink : public CCreatureActionBase { public:
    CBaseIntelligentPointer PDrinkThing;    // +0xa8 the consumed prop
    CBaseIntelligentPointer PJug;           // +0xb0 the carried cup/jug object
    virtual void FinishAction();
};

void CCreatureAction_Drink::FinishAction()
{
    if (IS_THING_ALIVE(PDrinkThing)) {
        ((CThing*)PDrinkThing.GetPItem())->Kill(true);
        CTCCarrying* carrying;
        ((CThing*)PCreature.GetPItem())->GetTC(TCI_CARRYING, &carrying);
        carrying->RemoveThingFromPrimarySlot(false);
        if (IS_THING_ALIVE(PJug)) {
            const CTCCarriedObject* carried = 0;
            const CThing* jugThing = ((CThing*)PJug.GetPItem());
            if (jugThing->HasTC(TCI_CARRYABLE))
                carried = ((const CTCCarriedObject*)&jugThing->GetTC(TCI_CARRYABLE));
            if (carried->m_pCarrier) {
                CThing* carrier = ((CThing*)((CTCCarryable*)carried)->GetPCarriedByThing());
                if (carrier == ((CThing*)PCreature.GetPItem()))
                    carrying->RemoveThing(*(((CThing*)PJug.GetPItem())));
                else
                    goto done;
            }
            CThing* jug = ((CThing*)PJug.GetPItem());
            carrying->PutThingInCarrySlot(*jug, carried->GetCarrySlot(), true);
        }
    }
done:
    CCreatureActionBase::FinishAction();
}