#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
// Behaviour test for 00dbfea0 COakValeBanditRaidBandit::Init: sight then sound radius from the script def, not turncoatable,
// the gate looked up by name and opened; the looked-up thing and the name string are released afterwards.
static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_strings = 0, g_destroyed = 0; static int g_gate = 0; static const void* g_me = 0;
static void __fastcall Destroy(void*) { ++g_destroyed; }
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length) : m_pData((void*)text) { ++g_strings; }
    ~CCharString() { --g_strings; }
};
class CBaseClass { public:                                // vtable 0x01231710
    virtual void v00() {}
    ~CBaseClass() {}
};
struct SCountBlock {
    long  m_references;                            // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
struct CScriptThingImp;
struct CScriptThingPImp {                          // CCountedPointer<CScriptThingImp>
    CScriptThingImp* Data;
    SCountBlock*     Info;
    ~CScriptThingPImp()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CScriptThing : public CBaseClass { public:                 // 12 bytes, vtable 0x01238C8C
    CScriptThingPImp PImp;
    virtual void v00() {}
    ~CScriptThing() {}
};
class CGameScriptInterface { public:
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const
    { Log(std::strcmp((const char*)name.m_pData, "OakValeBanditRaidGate") == 0 ? "Lookup" : "LookupBAD"); CScriptThing t; t.PImp.Data = (CScriptThingImp*)&g_gate; t.PImp.Info = new SCountBlock; t.PImp.Info->m_references = 1; t.PImp.Info->m_pfnDestroy = &Destroy; t.PImp.Info->m_pObject = 0; return t; }
    virtual void OpenDoor(const CScriptThing& door) const { Log(door.PImp.Data == (CScriptThingImp*)&g_gate ? "Open" : "OpenBAD"); }
    virtual void EntitySetSoundRadius(const CScriptThing& thing, float radius) const { Log(&thing == g_me && radius == 7.0f ? "Sound" : "SoundBAD"); }
    virtual void EntitySetSightRadius(const CScriptThing& thing, float radius) const { Log(&thing == g_me && radius == 25.0f ? "Sight" : "SightBAD"); }
    virtual void SetIsThingTurncoatable(const CScriptThing& thing, bool t) const { Log(&thing == g_me && !t ? "Turn" : "TurnBAD"); }
    virtual ~CGameScriptInterface() {}
};
class CScriptDef { public:
    unsigned char _pad[0xd5c];
    float         OVBRBanditSightRadius;           // +0xd5c
    float         OVBRBanditSoundRadius;           // +0xd60
};
CScriptDef* g_pScriptDef;
struct CQ_OakValeBanditRaidScript;
struct CQ_SunnyvaleMasterData;
struct COakValeBanditRaidBandit {
    CGameScriptInterface*       Game;              // +0x04
    CScriptThing                Me;                // +0x08
    CQ_OakValeBanditRaidScript* ParentClass;       // +0x14
    CQ_SunnyvaleMasterData*     PMasterData;       // +0x18
    void Init();
};

void COakValeBanditRaidBandit::Init()
{
    Game->EntitySetSightRadius(Me, g_pScriptDef->OVBRBanditSightRadius);
    Game->EntitySetSoundRadius(Me, g_pScriptDef->OVBRBanditSoundRadius);
    Game->SetIsThingTurncoatable(Me, false);
    {
        CCharString gateName("OakValeBanditRaidGate", -1);
        Game->OpenDoor(Game->GetThingWithScriptName(gateName));
    }
}

int main()
{
    static CScriptDef def; def.OVBRBanditSightRadius = 25.0f; def.OVBRBanditSoundRadius = 7.0f; g_pScriptDef = &def;
    CGameScriptInterface game;
    static COakValeBanditRaidBandit bandit; bandit.Game = &game; g_me = &bandit.Me;
    g_log[0] = 0;
    bandit.Init();
    bool ok = std::strcmp(g_log, "Sight;Sound;Turn;Lookup;Open;") == 0 && g_strings == 0 && g_destroyed == 1;
    if (!ok) { std::printf("00dbfea0_TEST FAIL\n"); return 1; }
    std::printf("00dbfea0_TEST PASS\n");
    return 0;
}