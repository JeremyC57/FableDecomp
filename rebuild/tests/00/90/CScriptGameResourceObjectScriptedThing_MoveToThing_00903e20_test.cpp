// Behaviour model of CScriptGameResourceObjectScriptedThing::MoveToThing @ 0x00903E20 (copy of the source's logic).
#include <cstdio>
#include <new>
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
};
struct CThing_Retail {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool m_dead;
    unsigned long Flags34;
    unsigned char HasTC(long t) const { return (unsigned char)((m_presentBits[t >> 5] >> (t & 31)) & 1); }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        const unsigned char& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component;
        }
        return present != 0;
    }
};
struct CIntelligentPointer_Retail { CThing_Retail* m_pItem; CThing_Retail* GetPItem() { return m_pItem; } };
struct CScriptThing {
    CThing_Retail* thing; bool alive;
    CThing_Retail* GetPThing() const { return thing; }
    bool IsAlive() const { return alive; }
};
struct CActionBase { virtual ~CActionBase() {} };
struct CTCScriptedControl {
    CActionBase* actions[8]; int n;
    void AddAction(CActionBase* a) { actions[n++] = a; }
};
struct CActionMoveTo : CActionBase {
    CThing_Retail* target; long type; float proximity; bool face, avoid, useAP, backup, cancel, ignore;
    CActionMoveTo(CThing_Retail* t, long m, float p, bool f, bool a, bool u, bool b, bool c, bool i)
        : target(t), type(m), proximity(p), face(f), avoid(a), useAP(u), backup(b), cancel(c), ignore(i) {}
};
enum EScriptEntityMoveType { SCRIPT_MOVE_WALK = 0, SCRIPT_MOVE_RUN = 1 };
static int g_waits = 0;
struct CScriptGameResourceObjectScriptedThing {
    CIntelligentPointer_Retail PThing;
    void WaitWhilePerformingTasks() { ++g_waits; }
    void MoveToThing(const CScriptThing& target, float proximity, EScriptEntityMoveType moveType, bool wait,
                     bool avoidDynamicObstacles, bool ignorePathPreferability, bool faceMovement)
    {
        CIntelligentPointer_Retail* thing = &PThing;
        if (thing->GetPItem()) {
            if (!thing->GetPItem()->m_dead) {
                if (target.IsAlive()) {
                    CThing_Retail* targetThing = target.GetPThing();
                    CTCScriptedControl* control;
                    thing->GetPItem()->GetInterface(&control, 0x1f);
                    long movementType;
                    switch (moveType) {
                    case SCRIPT_MOVE_WALK: movementType = 3; break;
                    case SCRIPT_MOVE_RUN:  movementType = 5; break;
                    default:               movementType = 1; break;
                    }
                    control->AddAction(new CActionMoveTo(targetThing, movementType, proximity, faceMovement,
                                                         avoidDynamicObstacles, false, false, false, ignorePathPreferability));
                    if (wait)
                        WaitWhilePerformingTasks();
                }
            }
        }
    }
};
int main()
{
    CTCScriptedControl control; control.n = 0;
    CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0x1f, (CTCBase*)&control } };
    CThing_Retail self = { { 0x80000000UL, 0, 0, 0, 0, 0, 0, 0, 0 }, { entries, entries + 2 }, false, 0 };
    CThing_Retail other = { { 0 }, { 0, 0 }, false, 0 };
    CScriptThing target = { &other, true };
    CScriptGameResourceObjectScriptedThing s; s.PThing.m_pItem = &self;
    s.MoveToThing(target, 2.5f, SCRIPT_MOVE_RUN, true, true, false, true);
    s.MoveToThing(target, 1.0f, SCRIPT_MOVE_WALK, false, false, true, false);
    s.MoveToThing(target, 1.0f, (EScriptEntityMoveType)7, false, false, false, false);
    CActionMoveTo* a = (CActionMoveTo*)control.actions[0];
    CActionMoveTo* b = (CActionMoveTo*)control.actions[1];
    CActionMoveTo* c = (CActionMoveTo*)control.actions[2];
    int ok = control.n == 3 && g_waits == 1
          && a->target == &other && a->type == 5 && a->proximity == 2.5f && a->face && a->avoid && !a->ignore && !a->useAP
          && b->type == 3 && b->ignore && !b->face && c->type == 1;
    target.alive = false; s.MoveToThing(target, 1.0f, SCRIPT_MOVE_RUN, true, false, false, false);
    target.alive = true; self.m_dead = true; s.MoveToThing(target, 1.0f, SCRIPT_MOVE_RUN, true, false, false, false);
    ok = ok && control.n == 3 && g_waits == 1;
    for (int i = 0; i < control.n; ++i) delete control.actions[i];
    if (!ok) { std::printf("00903e20_TEST FAIL\n"); return 1; }
    std::printf("00903e20_TEST PASS\n");
    return 0;
}