// Behaviour test for 0x009039D0 CScriptGameResourceObjectScriptedThing::CScriptGameResourceObjectScriptedThing(CGameScriptInterface&, CThing&, long, EScriptAIPriority)
#include <cstdio>
#include <cstring>

class CGameScriptInterface { public: int tag; };
class CTCBase { public: int tag; };

class CBaseObject { public:
    void* IntelligentPointer;
    CBaseObject() : IntelligentPointer(0) {}
};

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

class CThing : public CBaseObject { public:
    unsigned long  m_presentBits[9];
    CInterfaceMap  m_interfaces;
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
    template<class T> T* GetTC(long interfaceType)
    {
        T* tc = 0;
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            tc = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return tc;
    }
};

struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    void SetPItem(CBaseObject* item) { m_pItem = item; }
    CBaseObject* GetPItem() const { return (CBaseObject*)m_pItem; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CThing* GetPItem() const { return (CThing*)CBaseIntelligentPointer::GetPItem(); }
};

class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:
    void* PImpData; void* PImpInfo;
    CScriptGameResourceObjectScriptedThingBase() : PImpData(0), PImpInfo(0) {}
};

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_0 = 0, SCRIPT_AI_PRIORITY_2 = 2 };

class CScriptGameResourceObjectScriptedThing;
class CTCScriptedControl { public:
    CTCBase base;
    CScriptGameResourceObjectScriptedThing* owner; EScriptAIPriority priority; int calls;
    void SetLocked(CScriptGameResourceObjectScriptedThing& o, EScriptAIPriority p) { owner = &o; priority = p; ++calls; }
};

struct CCreatureMode {
    unsigned long type;
    virtual unsigned long GetModeType() { return type; }
    virtual ~CCreatureMode() {}
};
class CTCCreatureModeManager { public:
    CTCBase base;
    CCreatureMode* current; unsigned long removed; int removes;
    CCreatureMode* GetCurrentMode() { return current; }
    void RemoveMode(unsigned long mode) { removed = mode; ++removes; }
};

class CScriptGameResourceObjectScriptedThing : public CScriptGameResourceObjectScriptedThingBase { public:
    CGameScriptInterface* ScriptInterface;
    CIntelligentPointer   PThing;
    long                  ScriptID;
    bool                  NullResource;
    long                  ActionPriority;
    CScriptGameResourceObjectScriptedThing(CGameScriptInterface& scriptInterface, CThing& thing, long scriptID, EScriptAIPriority priority);
};

CScriptGameResourceObjectScriptedThing::CScriptGameResourceObjectScriptedThing(CGameScriptInterface& scriptInterface, CThing& thing, long scriptID, EScriptAIPriority priority)
    : ScriptInterface(&scriptInterface), PThing(&thing), ScriptID(scriptID), NullResource(false), ActionPriority(0)
{
    PThing.GetPItem()->GetTC<CTCScriptedControl>(0x1f)->SetLocked(*this, priority);
    CTCCreatureModeManager* modes;
    if (PThing.GetPItem()->GetInterface(&modes, 0x31)) {
        if (modes->GetCurrentMode()->GetModeType() == 7)
            modes->RemoveMode(7);
    }
}

int main()
{
    CTCScriptedControl control; std::memset(&control, 0, sizeof(control));
    CTCCreatureModeManager modes; std::memset(&modes, 0, sizeof(modes));
    CCreatureMode mode; mode.type = 7; modes.current = &mode;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x1f; entries[0].component = (CTCBase*)&control;
    entries[1].type = 0x31; entries[1].component = (CTCBase*)&modes;
    CThing thing; std::memset(thing.m_presentBits, 0, sizeof(thing.m_presentBits));
    thing.m_presentBits[0] = 0x80000000UL;           // 0x1f
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    CGameScriptInterface gsi;
    int ok = 1;

    CScriptGameResourceObjectScriptedThing a(gsi, thing, 42, SCRIPT_AI_PRIORITY_2);   // no mode manager
    if (a.ScriptInterface != &gsi || a.PThing.GetPItem() != &thing || a.ScriptID != 42 || a.NullResource || a.ActionPriority != 0) ok = 0;
    if (a.PImpData != 0 || a.PImpInfo != 0) ok = 0;
    if (control.calls != 1 || control.owner != &a || control.priority != SCRIPT_AI_PRIORITY_2 || modes.removes != 0) ok = 0;

    thing.m_presentBits[1] = 0x20000UL;               // 0x31: mode 7 current -> removed
    CScriptGameResourceObjectScriptedThing b(gsi, thing, 7, SCRIPT_AI_PRIORITY_0);
    if (control.calls != 2 || control.owner != &b || modes.removes != 1 || modes.removed != 7) ok = 0;

    mode.type = 3;                                    // another mode: left alone
    CScriptGameResourceObjectScriptedThing c(gsi, thing, 8, SCRIPT_AI_PRIORITY_0);
    if (control.calls != 3 || modes.removes != 1) ok = 0;

    if (!ok) { std::printf("009039d0_TEST FAIL\n"); return 1; }
    std::printf("009039d0_TEST PASS\n");
    return 0;
}