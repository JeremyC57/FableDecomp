// Behaviour test for 0x00904940 (CScriptGameResourceObjectScriptedThing)
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
namespace NTCScriptedControl {
struct CActionPerformExpression : CActionBase {
    CThing_Retail* target; const CCharString* expression; long d1, d2;
    CActionPerformExpression(CThing_Retail* t, const CCharString& e, long a, long b) : target(t), expression(&e), d1(a), d2(b) {}
};
}
struct CScriptGameResourceObjectScriptedThing_Methods {
    CIntelligentPointer_Retail PThing;
    void PerformExpression(const CScriptThing& target, const CCharString& expression);
};
void CScriptGameResourceObjectScriptedThing_Methods::PerformExpression(const CScriptThing& target, const CCharString& expression)
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)&PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            CTCScriptedControl* control;
            thing->GetPItem()->GetInterface(&control, 0x1f);
            CThing_Retail* targetThing = target.GetPThing();
            control->AddAction(new NTCScriptedControl::CActionPerformExpression(targetThing, expression, -1, -1));
        }
    }
}
int main()
{
    SThingRig rig; rig.Init();
    CThing_Retail other; std::memset(&other, 0, sizeof(other));
    CScriptThing target; target.thing = &other; target.alive = false;   // no life test in this verb
    CCharString expr("EXPRESSION_LAUGH");
    CScriptGameResourceObjectScriptedThing_Methods o;
    int ok = 1;
    o.PThing.m_pItem = 0;
    o.PerformExpression(target, expr);
    if (rig.control.count != 0) ok = 0;
    o.PThing.m_pItem = &rig.thing; rig.thing.m_dead = 1;
    o.PerformExpression(target, expr);
    if (rig.control.count != 0) ok = 0;
    rig.thing.m_dead = 0;
    o.PerformExpression(target, expr);
    NTCScriptedControl::CActionPerformExpression* a = (NTCScriptedControl::CActionPerformExpression*)rig.control.added;
    if (rig.control.count != 1 || !a) ok = 0;
    else if (a->target != &other || a->expression != &expr || a->d1 != -1 || a->d2 != -1) ok = 0;
    delete rig.control.added;
    if (!ok) { std::printf("00904940_TEST FAIL\n"); return 1; }
    std::printf("00904940_TEST PASS\n");
    return 0;
}
