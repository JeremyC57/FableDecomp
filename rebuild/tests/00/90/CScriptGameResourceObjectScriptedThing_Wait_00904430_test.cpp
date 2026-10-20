// Behaviour model of CScriptGameResourceObjectScriptedThing::Wait @ 0x00904430 (copy of the source logic, stand-in callees).
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
static long GFSecondsToFrames(float seconds) { return (long)(seconds * 30.0f); }
struct CActionWait : CActionBase {
    void* PTCScript; bool Activated, Terminated; long NoFramesToWait, UpdateCounter;
    CActionWait(long frames) : PTCScript(0), Activated(false), Terminated(false), NoFramesToWait(frames), UpdateCounter(0) {}
};
struct CScriptGameResourceObjectScriptedThing {
    CIntelligentPointer_Retail PThing;
    void Wait(float seconds)
    {
        CIntelligentPointer_Retail* thing = &PThing;
        if (thing->GetPItem()) {
            if (!thing->GetPItem()->m_dead) {
                CTCScriptedControl* control;
                thing->GetPItem()->GetInterface(&control, 0x1f);
                if (seconds == 0.0f)
                    control->AddAction(new CActionWait(0));
                else
                    control->AddAction(new CActionWait(GFSecondsToFrames(seconds)));
            }
        }
    }
};
int main()
{
    CTCScriptedControl control; control.n = 0;
    CInterfaceMap::SEntry entries[1] = { { 0x1f, (CTCBase*)&control } };
    CThing_Retail self = { { 0x80000000UL, 0, 0, 0, 0, 0, 0, 0, 0 }, { entries, entries + 1 }, false, 0 };
    CScriptGameResourceObjectScriptedThing s; s.PThing.m_pItem = &self;
    s.Wait(0.0f);
    s.Wait(2.0f);
    CActionWait* a = (CActionWait*)control.actions[0];
    CActionWait* b = (CActionWait*)control.actions[1];
    int ok = control.n == 2 && a->NoFramesToWait == 0 && a->UpdateCounter == 0 && !a->Activated
          && b->NoFramesToWait == 60 && b->UpdateCounter == 0;
    self.m_dead = true; s.Wait(1.0f);
    s.PThing.m_pItem = 0; s.Wait(1.0f);
    ok = ok && control.n == 2;
    delete a; delete b;
    if (!ok) { std::printf("00904430_TEST FAIL\n"); return 1; }
    std::printf("00904430_TEST PASS\n");
    return 0;
}