#ifndef FABLE_ENGINE_CCREATUREACTION_TANKARD_H
#define FABLE_ENGINE_CCREATUREACTION_TANKARD_H
#include "engine/CCreatureActionBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/NCreatureMode.h"

class CThing;
class CThingCreatureBase;

// Original direct bases and member types from the Ego_r and FableWin records.
class CCreatureAction_PickUpThingAtTable : public CCreatureActionBase {
public:
#ifdef FABLE_RETAIL_DRINK_FIXTURE_PASS27
public:
    CCreatureAction_PickUpThingAtTable(CThingCreatureBase&, CThing&, const CCharString&, NCreatureMode::EMode);
#endif
    virtual void PerformAction();
    virtual CCreatureActionBase* Clone() const;
protected:
    CIntelligentPointer<CThing> PThingToPickUp;
private:
    NCreatureMode::EMode Mode;
#ifdef FABLE_RETAIL_CCREATUREACTION_PICKUPTHINGATTABLE_COPYCTOR_DECL
    // Opt-in (NF327, 2026-11-24, NF323 Q2): the copy constructor retail calls out of line at 0x0084C9A0 (0x009680D0 PickUpJug::Clone,
    // 0x0084C970 PickUpThingAtTable::Clone). FableWin.pdb ??0CCreatureAction_PickUpThingAtTable@@QAE@ABV0@@Z, defined by the landed row
    // 0x0084C9A0 under that symbol. Declaration only: a Clone TU of a derived class then calls it instead of expanding the implicit one.
public:
    CCreatureAction_PickUpThingAtTable(const CCreatureAction_PickUpThingAtTable&);                      // 0x0084C9A0
#endif
};

class CCreatureAction_PutDownThingAtTable : public CCreatureActionBase {
public:
#ifdef FABLE_RETAIL_DRINK_FIXTURE_PASS27
public:
    CCreatureAction_PutDownThingAtTable(CThingCreatureBase&, const CCharString&, NCreatureMode::EMode);
#endif
    virtual void PerformAction();
    virtual CCreatureActionBase* Clone() const;
private:
    NCreatureMode::EMode Mode;
};

class CCreatureAction_PickUpTankard : public CCreatureAction_PickUpThingAtTable {
public:
    CCreatureAction_PickUpTankard(CThingCreatureBase&, CThing&);
    virtual CCreatureActionBase* Clone() const;
};

class CCreatureAction_PutDownTankard : public CCreatureAction_PutDownThingAtTable {
public:
    CCreatureAction_PutDownTankard(CThingCreatureBase&);
    virtual CCreatureActionBase* Clone() const;
};

FABLE_STATIC_ASSERT_AT(44, sizeof(CCreatureAction_PickUpTankard) == 0xb4);
FABLE_STATIC_ASSERT_AT(45, sizeof(CCreatureAction_PutDownTankard) == 0xac);
#endif
