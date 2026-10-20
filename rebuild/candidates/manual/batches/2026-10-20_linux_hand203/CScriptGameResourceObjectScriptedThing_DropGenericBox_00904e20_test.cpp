// Behaviour test for 0x00904e20 (CScriptGameResourceObjectScriptedThing)
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
struct CActionBaseFull : CActionBase {
    CTCScriptedControl* PTCScript; bool Activated; bool Terminated;
    CActionBaseFull() : PTCScript(0), Activated(false), Terminated(false) { }
};
struct CActionDropGenericBox : CActionBaseFull {
    long ActionID; bool ActionPluggedIn;
    CActionDropGenericBox() : ActionID(-1), ActionPluggedIn(false) { }
};
}
struct CScriptGameResourceObjectScriptedThing_Methods {
    CIntelligentPointer_Retail PThing;
    void DropGenericBox();
};
void CScriptGameResourceObjectScriptedThing_Methods::DropGenericBox()
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)&PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            CTCScriptedControl* control;
            thing->GetPItem()->GetInterface(&control, 0x1f);
            control->AddAction(new NTCScriptedControl::CActionDropGenericBox());
        }
    }
}
int main()
{
    SThingRig rig; rig.Init();
    CScriptGameResourceObjectScriptedThing_Methods o;
    int ok = 1;
    o.PThing.m_pItem = 0;
    o.DropGenericBox();
    if (rig.control.count != 0) ok = 0;
    o.PThing.m_pItem = &rig.thing; rig.thing.m_dead = 1;
    o.DropGenericBox();
    if (rig.control.count != 0) ok = 0;
    rig.thing.m_dead = 0;
    o.DropGenericBox();
    NTCScriptedControl::CActionDropGenericBox* a = (NTCScriptedControl::CActionDropGenericBox*)rig.control.added;
    if (rig.control.count != 1 || !a) ok = 0;
    else if (a->PTCScript != 0 || a->Activated || a->Terminated || a->ActionID != -1 || a->ActionPluggedIn) ok = 0;
    delete rig.control.added;
    if (!ok) { std::printf("00904e20_TEST FAIL\n"); return 1; }
    std::printf("00904e20_TEST PASS\n");
    return 0;
}
