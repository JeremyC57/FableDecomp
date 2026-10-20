// Behaviour test for 0x00904af0 (CScriptGameResourceObjectScriptedThing)
#include <cstdio>
#include <cstring>

// Behaviour model of the scripted-thing handle family (host-sized copy of the retail model).
struct CTCBase { int tag; };

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* p = first;
        while (p != last && p->type < *type) ++p;
        return p;
    }
};

struct CThing_Retail {
    unsigned long  m_presentBits[9];
    CInterfaceMap  m_interfaces;
    unsigned long  ThingTypeFlags;
    unsigned char  m_dead : 1;
    unsigned char  m_91_1 : 7;
    int            finishCalls;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CIntelligentPointer_Retail {
    void* m_pItem;
    CThing_Retail* GetPItem() { return (CThing_Retail*)m_pItem; }
};

class CScriptThing {
public:
    CThing_Retail* thing;
    bool alive;
    CThing_Retail* GetPThing() const { return thing; }
    bool IsAlive() const { return alive; }
};

namespace NTCScriptedControl {
struct CActionBase { virtual ~CActionBase() {} };
}

struct CTCScriptedControl {
    CTCBase base;
    NTCScriptedControl::CActionBase* added;
    int count;
    void AddAction(NTCScriptedControl::CActionBase* action) { added = action; ++count; }
};

// A thing whose interface 0x1f (bit 31 of word 0) resolves to `control`.
struct SThingRig {
    CThing_Retail thing;
    CInterfaceMap::SEntry entries[2];
    CTCScriptedControl control;
    void Init()
    {
        std::memset(&thing, 0, sizeof(thing));
        std::memset(&control, 0, sizeof(control));
        thing.m_presentBits[0] = 0x80000000UL;
        entries[0].type = 0x10; entries[0].component = 0;
        entries[1].type = 0x1f; entries[1].component = (CTCBase*)&control;
        thing.m_interfaces.first = entries;
        thing.m_interfaces.last = entries + 2;
    }
};

class CCharString { public: const char* text; CCharString(const char* t) : text(t) {} };
struct CThingCreatureBase {
    void FinishCurrentAction() { ((CThing_Retail*)this)->finishCalls++; }
};
struct CTCScriptedControlFull {
    CTCBase base; NTCScriptedControl::CActionBase* added; int count; int cleanups;
    void CleanupAllActions() { ++cleanups; }
    void AddAction(NTCScriptedControl::CActionBase* action) { added = action; ++count; }
};
namespace NTCScriptedControl {
struct CActionPlayCombatAnimation : CActionBase {
    const CCharString* anim; bool a, b, c, d, e;
    CActionPlayCombatAnimation(const CCharString& n, bool x1, bool x2, bool x3, bool x4, bool x5) : anim(&n), a(x1), b(x2), c(x3), d(x4), e(x5) {}
};
}
struct CScriptGameResourceObjectScriptedThing_Methods {
    CIntelligentPointer_Retail PThing;
    void PlayCombatAnimation(const CCharString& animation, bool b1, bool clearActions, bool b3, bool b4, bool b5, bool b6);
};
void CScriptGameResourceObjectScriptedThing_Methods::PlayCombatAnimation(const CCharString& animation, bool b1, bool clearActions, bool b3, bool b4, bool b5, bool b6)
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)&PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            CTCScriptedControlFull* control;
            thing->GetPItem()->GetInterface(&control, 0x1f);
            if (clearActions) {
                control->CleanupAllActions();
                if (thing->GetPItem()->ThingTypeFlags & 8)
                    ((CThingCreatureBase*)thing->GetPItem())->FinishCurrentAction();
            }
            control->AddAction(new NTCScriptedControl::CActionPlayCombatAnimation(animation, b1, b3, b4, b5, b6));
        }
    }
}
int main()
{
    SThingRig rig; rig.Init();
    CTCScriptedControlFull control; std::memset(&control, 0, sizeof(control));
    rig.entries[1].component = (CTCBase*)&control;
    CCharString anim("COMBAT_SWING");
    CScriptGameResourceObjectScriptedThing_Methods o;
    int ok = 1;
    o.PThing.m_pItem = 0;
    o.PlayCombatAnimation(anim, true, true, false, true, false, true);
    if (control.count != 0) ok = 0;
    o.PThing.m_pItem = &rig.thing; rig.thing.m_dead = 1;
    o.PlayCombatAnimation(anim, true, true, false, true, false, true);
    if (control.count != 0) ok = 0;
    rig.thing.m_dead = 0;
    o.PlayCombatAnimation(anim, true, false, false, true, false, true);   // no clear
    if (control.count != 1 || control.cleanups != 0 || rig.thing.finishCalls != 0) ok = 0;
    NTCScriptedControl::CActionPlayCombatAnimation* a = (NTCScriptedControl::CActionPlayCombatAnimation*)control.added;
    if (!a || a->anim != &anim || !a->a || a->b || !a->c || a->d || !a->e) ok = 0;
    delete control.added;
    o.PlayCombatAnimation(anim, false, true, true, false, true, false);   // clear, not a creature
    if (control.count != 2 || control.cleanups != 1 || rig.thing.finishCalls != 0) ok = 0;
    a = (NTCScriptedControl::CActionPlayCombatAnimation*)control.added;
    if (!a || a->a || !a->b || a->c || !a->d || a->e) ok = 0;
    delete control.added;
    rig.thing.ThingTypeFlags = 8;                                       // clear, a creature
    o.PlayCombatAnimation(anim, false, true, true, false, true, false);
    if (control.count != 3 || control.cleanups != 2 || rig.thing.finishCalls != 1) ok = 0;
    delete control.added;
    if (!ok) { std::printf("00904af0_TEST FAIL\n"); return 1; }
    std::printf("00904af0_TEST PASS\n");
    return 0;
}