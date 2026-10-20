// Behaviour test for 0x00904F60 CScriptGameResourceObjectScriptedThing::UnsheatheWeapons
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

struct CThingCreatureBase { int tag; };
static int g_built = 0, g_destroyed = 0;
class CCreatureActionBase {
public:
    CThingCreatureBase* creature;
    CCreatureActionBase() : creature(0) {}
    virtual ~CCreatureActionBase() {}
};
class CCombatActionBase : public CCreatureActionBase {
public:
    virtual ~CCombatActionBase() { ++g_destroyed; }
};
class CCreatureAction_UnsheatheWeapons : public CCombatActionBase {
public:
    CCreatureAction_UnsheatheWeapons(CThingCreatureBase& c) { creature = &c; ++g_built; }
};
namespace NTCScriptedControl {
struct CActionDoCreatureAction : CActionBase {
    CThingCreatureBase* creature;
    CActionDoCreatureAction(const CCreatureActionBase& action) : creature(action.creature) {}
};
}
struct CScriptGameResourceObjectScriptedThing_Methods {
    CIntelligentPointer_Retail PThing;
    void UnsheatheWeapons();
};
void CScriptGameResourceObjectScriptedThing_Methods::UnsheatheWeapons()
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)&PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            CTCScriptedControl* control;
            thing->GetPItem()->GetInterface(&control, 0x1f);
            CThingCreatureBase* creature = (CThingCreatureBase*)thing->GetPItem();
            control->AddAction(new NTCScriptedControl::CActionDoCreatureAction(CCreatureAction_UnsheatheWeapons(*creature)));
        }
    }
}
int main()
{
    SThingRig rig; rig.Init();
    CScriptGameResourceObjectScriptedThing_Methods o;
    int ok = 1;
    o.PThing.m_pItem = 0;
    o.UnsheatheWeapons();
    if (rig.control.count != 0 || g_built != 0) ok = 0;
    o.PThing.m_pItem = &rig.thing; rig.thing.m_dead = 1;
    o.UnsheatheWeapons();
    if (rig.control.count != 0 || g_built != 0) ok = 0;
    rig.thing.m_dead = 0;
    o.UnsheatheWeapons();
    NTCScriptedControl::CActionDoCreatureAction* a = (NTCScriptedControl::CActionDoCreatureAction*)rig.control.added;
    if (rig.control.count != 1 || !a || a->creature != (CThingCreatureBase*)&rig.thing) ok = 0;
    if (g_built != 1 || g_destroyed != 1) ok = 0;          // the temporary dies after AddAction
    delete rig.control.added;
    if (!ok) { std::printf("00904f60_TEST FAIL\n"); return 1; }
    std::printf("00904f60_TEST PASS\n");
    return 0;
}