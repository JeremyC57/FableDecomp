#ifndef FABLE_ENGINE_CCREATUREACTION_SHOWTROPHY_H
#define FABLE_ENGINE_CCREATUREACTION_SHOWTROPHY_H
#ifdef FABLE_RETAIL_CCREATUREACTION_SHOWTROPHY_USE
#include "engine/CCreatureActionBase.h"
#include "engine/CIntelligentPointer.h"
class CThing;
class CThingCreatureBase;
// Ego_d 0x24827 gives the base and four members. At retail 0x006DDFD4 the
// caller allocates 0xC0 bytes, with a 0xA8 retail action base.
class CCreatureAction_ShowTrophy : public CCreatureActionBase {
public:
    CCreatureAction_ShowTrophy(CThingCreatureBase&, CThing&, const CCharString&, long);
    virtual ~CCreatureAction_ShowTrophy();
    virtual void Initialise();
    virtual void PerformAction();
    virtual void FinishAction();
    virtual CCreatureActionBase* Clone() const;
private:
    CIntelligentPointer<CThing> PTrophyBeingShown;
    unsigned long ShowingTrophyDeedHandle;
    bool PostedDeed;
    CIntelligentPointer<CThing> PEmitter;
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_ShowTrophy) == 0xc0);
#endif
#endif
