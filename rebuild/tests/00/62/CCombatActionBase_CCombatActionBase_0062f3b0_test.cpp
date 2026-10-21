#include <cstdio>
#include <cstring>
#include <new>
struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    void* GetPItem() const { return m_pItem; }
    void SetPItem(void* item) { m_pItem = item; }
};
struct CThingPointer : CBaseIntelligentPointer {
    CThingPointer() {}
    CThingPointer(const CThingPointer& other) { SetPItem(other.GetPItem()); }
};
struct CConstThingPointer : CBaseIntelligentPointer {
    CConstThingPointer() {}
    CConstThingPointer(const CConstThingPointer& other) { SetPItem(other.GetPItem()); }
};
struct CMeleeCombatAbilityDef { void* m_pVtable; int m_refCount; };
struct CMeleeAbilityDefPointer {
    CMeleeCombatAbilityDef* Data;
    CMeleeAbilityDefPointer() : Data(0) {}
    CMeleeAbilityDefPointer(const CMeleeAbilityDefPointer& other) : Data(other.Data) { if (Data) ++Data->m_refCount; }
};
static int g_listCopies, g_vectorCopies, g_baseCopies;
struct CStrikeEventList {
    void* _M_node;
    CStrikeEventList() : _M_node(0) {}
    CStrikeEventList(const CStrikeEventList& other) : _M_node(other._M_node) { ++g_listCopies; }
};
struct CAnimationEntryVector {
    const void** _M_start; const void** _M_finish; const void** _M_end_of_storage;
    CAnimationEntryVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    CAnimationEntryVector(const CAnimationEntryVector& o) : _M_start(o._M_start), _M_finish(o._M_finish), _M_end_of_storage(o._M_end_of_storage) { ++g_vectorCopies; }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CCreatureActionBase {
    virtual void v00() {}
    int m_creature;
    CCreatureActionBase() : m_creature(0) {}
    CCreatureActionBase(const CCreatureActionBase& other) : m_creature(other.m_creature) { ++g_baseCopies; }
};
struct CCombatActionBase : CCreatureActionBase {
    CThingPointer           PTarget;
    CConstThingPointer      PCameraPoint;
    CMeleeAbilityDefPointer PMeleeAbilityDef;
    CStrikeEventList        StrikeEvents;
    int   LastStrikeEventEndTime;
    float StrikeEventInterruptTime, StrikeEventResetComboTime;
    int   MovementEventInterruptFrame;
    float StrikeSpeedVariationStartTime, StrikeSpeedVariationEndTime;
    bool  HasStrikeEventInterrupt, HasStrikeEventResetCombo, HasMovementEventInterrupt, StrikeEventInterruptActive,
          MovementEventInterruptActive, PreferredFinalPosSet, HasPerformAction, SentMissedEvent, HasResetMeleeCombatAbilityEffects;
    int   LastAnimStage;
    bool  HitThing, HitCreature;
    int   FrameUpdatePauseFrames;
    CAnimationEntryVector CombatAnimations;
    int   ActiveStrikeEventIndex;
    CThingPointer LastWeaponUsedInStrike;
    C3DVector PreferredMovementVector;
    CCombatActionBase() {}
};
CCombatActionBase* CopyCombatAction(void* where, const CCombatActionBase& other) { return new (where) CCombatActionBase(other); }
int main()
{
    int target, camera, weapon;
    CMeleeCombatAbilityDef def; def.m_refCount = 1;
    CCombatActionBase a;
    a.m_creature = 7; a.PTarget.SetPItem(&target); a.PCameraPoint.SetPItem(&camera); a.PMeleeAbilityDef.Data = &def;
    a.LastStrikeEventEndTime = 42; a.StrikeEventInterruptTime = 1.5f; a.HasPerformAction = true; a.SentMissedEvent = false;
    a.HasResetMeleeCombatAbilityEffects = true; a.LastAnimStage = 3; a.HitCreature = true; a.ActiveStrikeEventIndex = 5;
    a.LastWeaponUsedInStrike.SetPItem(&weapon);
    a.PreferredMovementVector.X = 1.0f; a.PreferredMovementVector.Y = 2.0f; a.PreferredMovementVector.Z = 3.0f;
    static double buf[sizeof(CCombatActionBase) / sizeof(double) + 1];
    CCombatActionBase* b = CopyCombatAction(buf, a);
    if (b->m_creature != 7 || b->PTarget.GetPItem() != &target || b->PCameraPoint.GetPItem() != &camera
        || b->PMeleeAbilityDef.Data != &def || def.m_refCount != 2 || g_listCopies != 1 || g_vectorCopies != 1 || g_baseCopies != 1
        || b->LastStrikeEventEndTime != 42 || b->StrikeEventInterruptTime != 1.5f || !b->HasPerformAction || b->SentMissedEvent
        || !b->HasResetMeleeCombatAbilityEffects || b->LastAnimStage != 3 || !b->HitCreature || b->ActiveStrikeEventIndex != 5
        || b->LastWeaponUsedInStrike.GetPItem() != &weapon || b->PreferredMovementVector.Z != 3.0f) {
        std::printf("0062f3b0_TEST FAIL\n"); return 1;
    }
    // a null def is not add-ref'd
    a.PMeleeAbilityDef.Data = 0;
    CCombatActionBase* c = CopyCombatAction(buf, a);
    if (c->PMeleeAbilityDef.Data != 0 || def.m_refCount != 2) { std::printf("0062f3b0_TEST FAIL\n"); return 1; }
    std::printf("0062f3b0_TEST PASS\n");
    return 0;
}