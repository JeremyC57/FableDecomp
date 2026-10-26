// Behaviour test for 0x00903710 (CScriptGameResourceObjectScriptedThing constructor): the handle base is
// empty, the script interface stored, PThing null, ScriptID 0, NullResource true, ActionPriority 0.
#include <cstdio>

class CCPPointerInfo;
class CGameScriptInterface { public: int dummy; };

class CBaseObject { public:
    virtual void v00() { }
    virtual ~CBaseObject() { }
    void* IntelligentPointer;
    CBaseObject() : IntelligentPointer(0) { }
};

class CScriptGameResourceObjectScriptedThingBase;
struct CCountedPointer_ScriptedThingBase {
    CScriptGameResourceObjectScriptedThingBase* Data;
    CCPPointerInfo*                             Info;
    CCountedPointer_ScriptedThingBase() : Data(0), Info(0) { }
};

class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:
    CCountedPointer_ScriptedThingBase PImp;
    virtual void v00() { }
    CScriptGameResourceObjectScriptedThingBase() { }
};

static int g_setCalls = 0;
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem((void*)1) { }
    void SetPItem(CBaseObject* item) { m_pItem = item; ++g_setCalls; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() : CBaseIntelligentPointer() { SetPItem(0); }
};

class CScriptGameResourceObjectScriptedThing : public CScriptGameResourceObjectScriptedThingBase { public:
    CGameScriptInterface* ScriptInterface;
    CIntelligentPointer   PThing;
    long                  ScriptID;
    bool                  NullResource;
    long                  ActionPriority;
    virtual void v00() { }
    CScriptGameResourceObjectScriptedThing(CGameScriptInterface* scriptInterface);
};

CScriptGameResourceObjectScriptedThing::CScriptGameResourceObjectScriptedThing(CGameScriptInterface* scriptInterface)
    : ScriptInterface(scriptInterface), ScriptID(0), NullResource(true), ActionPriority(0)
{
}

int main()
{
    CGameScriptInterface gsi;
    CScriptGameResourceObjectScriptedThing t(&gsi);
    if (t.ScriptInterface != &gsi || t.PThing.m_pItem != 0 || g_setCalls != 1 || t.ScriptID != 0 ||
        !t.NullResource || t.ActionPriority != 0 || t.PImp.Data != 0 || t.PImp.Info != 0 || t.IntelligentPointer != 0) {
        std::printf("00903710_TEST FAIL\n");
        return 1;
    }
    std::printf("00903710_TEST PASS\n");
    return 0;
}