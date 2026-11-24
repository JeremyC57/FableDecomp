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
