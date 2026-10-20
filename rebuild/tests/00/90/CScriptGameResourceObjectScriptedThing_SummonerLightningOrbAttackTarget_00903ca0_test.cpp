// Behaviour test for 0x00903CA0 CScriptGameResourceObjectScriptedThing::SummonerLightningOrbAttackTarget
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

namespace NTCScriptedControl {
struct CActionSummonerLightningAttackTarget : CActionBase {
    CThing_Retail* target;
    CActionSummonerLightningAttackTarget(CThing_Retail* t) : target(t) {}
};
}

struct CScriptGameResourceObjectScriptedThing_Methods {
    CIntelligentPointer_Retail PThing;
    void SummonerLightningOrbAttackTarget(const CScriptThing& target);
};

void CScriptGameResourceObjectScriptedThing_Methods::SummonerLightningOrbAttackTarget(const CScriptThing& target)
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)&PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            if (target.IsAlive()) {
                CThing_Retail* targetThing = target.GetPThing();
                CTCScriptedControl* control;
                thing->GetPItem()->GetInterface(&control, 0x1f);
                control->AddAction(new NTCScriptedControl::CActionSummonerLightningAttackTarget(targetThing));
            }
        }
    }
}

int main()
{
    SThingRig rig; rig.Init();
    CThing_Retail victim; std::memset(&victim, 0, sizeof(victim));
    CScriptThing target; target.thing = &victim; target.alive = true;
    CScriptGameResourceObjectScriptedThing_Methods o;
    int ok = 1;
    o.PThing.m_pItem = 0;                               // no thing: nothing queued
    o.SummonerLightningOrbAttackTarget(target);
    if (rig.control.count != 0) ok = 0;
    o.PThing.m_pItem = &rig.thing; rig.thing.m_dead = 1; // dead thing
    o.SummonerLightningOrbAttackTarget(target);
    if (rig.control.count != 0) ok = 0;
    rig.thing.m_dead = 0; target.alive = false;          // dead target
    o.SummonerLightningOrbAttackTarget(target);
    if (rig.control.count != 0) ok = 0;
    target.alive = true;
    o.SummonerLightningOrbAttackTarget(target);
    if (rig.control.count != 1 || !rig.control.added) ok = 0;
    else if (((NTCScriptedControl::CActionSummonerLightningAttackTarget*)rig.control.added)->target != &victim) ok = 0;
    delete rig.control.added;
    if (!ok) { std::printf("00903ca0_TEST FAIL\n"); return 1; }
    std::printf("00903ca0_TEST PASS\n");
    return 0;
}