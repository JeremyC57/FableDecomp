// Behaviour test for 00909520 CCombatSequence_WillForcePushLongRange::OnCreate: actions are queued in retail order with the sequence's
// owner/target, each held once by the queue after the local counted pointer is released.
#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; };
struct CThing { int id; };
static int g_live = 0;
struct CActionBase { virtual ~CActionBase() { --g_live; } const char* kind; CThingCreatureBase* owner; CThing* target; char anim[32]; CActionBase() { ++g_live; anim[0] = 0; } };
struct CCreatureAction_PlayCombatAnimation : CActionBase { bool flag; CCreatureAction_PlayCombatAnimation(CThingCreatureBase* o, CThing* t, const char* s, bool f) { flag = f; init(o, t, s); } void init(CThingCreatureBase* o, CThing* t, const char* s) { kind = "CCreatureAction_PlayCombatAnimation"; owner = o; target = t; std::strcpy(anim, s); } };
struct CCreatureAction_CastWillForceSpell : CActionBase { long level; bool flag; CCreatureAction_CastWillForceSpell(CThingCreatureBase* o, long l, bool f, const char* s) { kind = "CCreatureAction_CastWillForceSpell"; owner = o; target = 0; level = l; flag = f; std::strcpy(anim, s); } };
struct SCountBlock { long references; void (*destroy)(void*); void* object; };
static void DestroyAction(void* p) { delete static_cast<CActionBase*>(p); }
struct CCountedPointer {
    CActionBase* object; SCountBlock* count;
    CCountedPointer() : object(0), count(0) { }
    CCountedPointer(const CCountedPointer& o) : object(o.object), count(o.count) { if (count) ++count->references; }
    void Release() { if (count) { --count->references; if (count->references == 0) { count->destroy(count->object); delete count; } } object = 0; count = 0; }
    void Reset(CActionBase* p) { Release(); object = p; if (p) { count = new SCountBlock; count->references = 1; count->destroy = DestroyAction; count->object = p; } }
    CCountedPointer& operator=(CActionBase* p) { Reset(p); return *this; }
    ~CCountedPointer() { Release(); }
};
struct CSeq {
    CThingCreatureBase* m_pCreature; CThing* m_pTarget;
    CCountedPointer queue[8]; int n; bool baseCreated;
    CSeq() : n(0), baseCreated(false) { }
    CThingCreatureBase* GetCreature() const { return m_pCreature; }
    CThing* GetTarget() const { return m_pTarget; }
    void AddAction(const CCountedPointer& a) { queue[n] = CCountedPointer(); queue[n].object = a.object; queue[n].count = a.count; if (a.count) ++a.count->references; ++n; }
    void BaseOnCreate() { baseCreated = true; }
    void OnCreate();
};
void CSeq::OnCreate()
{
    CCountedPointer action;
    action = new CCreatureAction_PlayCombatAnimation(GetCreature(), GetTarget(), "ST_FORCE_PUSH_INTO", true);
    AddAction(action);
    action = new CCreatureAction_CastWillForceSpell(GetCreature(), 1, true, "ST_FORCE_PUSH_RELEASE");
    AddAction(action);
    BaseOnCreate();
}
int main()
{
    CThingCreatureBase owner = { 1 }; CThing target = { 2 };
    int ok = 1;
    {
        CSeq seq; seq.m_pCreature = &owner; seq.m_pTarget = &target;
        seq.OnCreate();
        if (seq.n != 2 || !seq.baseCreated || g_live != 2) ok = 0;
        if (ok && (std::strcmp(seq.queue[0].object->kind, "CCreatureAction_PlayCombatAnimation") != 0 || seq.queue[0].object->owner != &owner || seq.queue[0].object->target != &target || seq.queue[0].count->references != 1)) ok = 0;
        if (ok && std::strcmp(seq.queue[0].object->anim, "ST_FORCE_PUSH_INTO") != 0) ok = 0;
        if (ok && (std::strcmp(seq.queue[1].object->kind, "CCreatureAction_CastWillForceSpell") != 0 || seq.queue[1].object->owner != &owner || seq.queue[1].object->target != 0 || static_cast<CCreatureAction_CastWillForceSpell*>(seq.queue[1].object)->level != 1 || seq.queue[1].count->references != 1)) ok = 0;
        if (ok && std::strcmp(seq.queue[1].object->anim, "ST_FORCE_PUSH_RELEASE") != 0) ok = 0;
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("00909520_TEST FAIL\n"); return 1; }
    std::printf("00909520_TEST PASS\n");
    return 0;
}