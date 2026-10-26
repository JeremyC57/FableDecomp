// Behaviour test for 0x00904780 CScriptGameResourceObjectScriptedThing::Speak
#include <cstdio>
#include <cstring>

// Behaviour model of the scripted-thing handle family (host-sized copy of the retail model).
class CTCBase { public: int tag; };

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

class CTCScriptedControl { public:
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

enum ETextGroupSelectionMethod { TEXT_GROUP_SELECT_RANDOM = 0, TEXT_GROUP_SELECT_SEQUENTIAL = 1 };
namespace NTCScriptedControl {
struct CActionTalkToThing : CActionBase {
    CThing_Retail* listener; const char* text; const char* text2; ETextGroupSelectionMethod method; bool b1, b2, b3;
    CActionTalkToThing(CThing_Retail* l, const char* t, const char* t2, ETextGroupSelectionMethod m, bool x, bool y, bool z)
        : listener(l), text(t), text2(t2), method(m), b1(x), b2(y), b3(z) {}
};
}

struct CScriptGameResourceObjectScriptedThing_Methods {
    CIntelligentPointer_Retail PThing;
    void Speak(const CScriptThing& listener, const char* textGroup, ETextGroupSelectionMethod method, bool b1, bool b2, bool b3);
};

void CScriptGameResourceObjectScriptedThing_Methods::Speak(const CScriptThing& listener, const char* textGroup, ETextGroupSelectionMethod method, bool b1, bool b2, bool b3)
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)&PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            if (listener.IsAlive()) {
                CThing_Retail* me = thing->GetPItem();
                CThing_Retail* listenerThing = listener.GetPThing();
                CTCScriptedControl* control;
                me->GetInterface(&control, 0x1f);
                control->AddAction(new NTCScriptedControl::CActionTalkToThing(listenerThing, textGroup, 0, method, b1, b2, b3));
            }
        }
    }
}

int main()
{
    SThingRig rig; rig.Init();
    CThing_Retail other; std::memset(&other, 0, sizeof(other));
    CScriptThing listener; listener.thing = &other; listener.alive = true;
    CScriptGameResourceObjectScriptedThing_Methods o;
    int ok = 1;
    o.PThing.m_pItem = 0;
    o.Speak(listener, "TEXT_GREETING", TEXT_GROUP_SELECT_RANDOM, false, true, false);
    if (rig.control.count != 0) ok = 0;
    o.PThing.m_pItem = &rig.thing; rig.thing.m_dead = 1;
    o.Speak(listener, "TEXT_GREETING", TEXT_GROUP_SELECT_RANDOM, false, true, false);
    if (rig.control.count != 0) ok = 0;
    rig.thing.m_dead = 0; listener.alive = false;
    o.Speak(listener, "TEXT_GREETING", TEXT_GROUP_SELECT_RANDOM, false, true, false);
    if (rig.control.count != 0) ok = 0;
    listener.alive = true;
    o.Speak(listener, "TEXT_GREETING", TEXT_GROUP_SELECT_RANDOM, false, true, false);
    NTCScriptedControl::CActionTalkToThing* a = (NTCScriptedControl::CActionTalkToThing*)rig.control.added;
    if (rig.control.count != 1 || !a) ok = 0;
    else if (a->listener != &other || std::strcmp(a->text, "TEXT_GREETING") != 0 || a->text2 != 0 || a->method != TEXT_GROUP_SELECT_RANDOM || a->b1 || !a->b2 || a->b3) ok = 0;
    delete rig.control.added;
    if (!ok) { std::printf("00904780_TEST FAIL\n"); return 1; }
    std::printf("00904780_TEST PASS\n");
    return 0;
}