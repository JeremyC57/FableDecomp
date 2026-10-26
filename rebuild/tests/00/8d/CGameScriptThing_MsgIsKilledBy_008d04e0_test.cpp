#include <cstdio>
#include <cstring>

class CCharStringData { public: const char* PData; unsigned long StringLength; };
static bool BasicEquals(const CCharStringData& d, const char* text) { return std::strcmp(d.PData, text) == 0; }
static long Compare_00411570(const char* a, const char* b) { int r = std::strcmp(a, b); return r < 0 ? -1 : (r > 0 ? 1 : 0); }
class CCharString { public:
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData) return true;
        if (PStringData && other.PStringData && PStringData->StringLength == other.PStringData->StringLength)
            return Compare_00411570(PStringData->PData, other.PStringData->PData) ? false : true;
        return false;
    }
};
static int g_scriptThings;
class CScriptThing { public:
    CCharString Name;
    CScriptThing(const CCharString& n) : Name(n) { ++g_scriptThings; }
    CScriptThing(const CScriptThing& o) : Name(o.Name) { ++g_scriptThings; }
    ~CScriptThing() { --g_scriptThings; }
    const CCharString& GetName() const { return Name; }
};
struct CThingRetail {
    CCharString ScriptName; unsigned char Dead;
    CScriptThing GetScriptThing() const { return CScriptThing(ScriptName); }
};
class CMessageEvent { public: long Type; unsigned long long Target; CThingRetail* Originator; };
static CMessageEvent* g_message;
class CMessageEventManager { public:
    CMessageEvent* FindMostRecentMessage(long type, unsigned long long uid)
    { return (g_message && g_message->Type == type && g_message->Target == uid) ? g_message : 0; }
};
class CGameScriptThing { public:
    unsigned long long ThingUID; CMessageEventManager* Messages;
    bool MsgIsKilledBy(const CCharString& killer) const
    {
        CMessageEvent* message = Messages->FindMostRecentMessage(9, ThingUID);
        if (message) {
            if (killer.PStringData == 0) {
                if (std::strcmp("", "") == 0) goto killed;
            } else if (BasicEquals(*killer.PStringData, ""))
                goto killed;
            CThingRetail* thing = message->Originator;
            if (thing && !(thing->Dead & 1) && killer == thing->GetScriptThing().GetName())
                goto killed;
        }
        return false;
    killed:
        return true;
    }
};
int main()
{
    CCharStringData hero = { "SCRIPT_NAME_HERO", 16 }, hero2 = { "SCRIPT_NAME_HERO", 16 }, bandit = { "BANDIT", 6 }, empty = { "", 0 };
    CThingRetail killerThing; killerThing.ScriptName.PStringData = &hero; killerThing.Dead = 0;
    CMessageEvent m = { 9, 0x1234ULL, &killerThing };
    CMessageEventManager mgr; CGameScriptThing me; me.ThingUID = 0x1234ULL; me.Messages = &mgr;
    CCharString nullName = { 0 }, emptyName = { &empty }, heroName = { &hero2 }, banditName = { &bandit };
    g_message = 0;
    if (me.MsgIsKilledBy(nullName)) { std::printf("008d04e0_TEST FAIL\n"); return 1; }     // no message
    g_message = &m;
    if (!me.MsgIsKilledBy(nullName) || !me.MsgIsKilledBy(emptyName) || !me.MsgIsKilledBy(heroName) || me.MsgIsKilledBy(banditName)) {
        std::printf("008d04e0_TEST FAIL\n"); return 1;
    }
    killerThing.Dead = 1;
    if (me.MsgIsKilledBy(heroName) || !me.MsgIsKilledBy(emptyName) || g_scriptThings != 0) { std::printf("008d04e0_TEST FAIL\n"); return 1; }
    m.Target = 0x99ULL;                                 // aimed at someone else
    if (me.MsgIsKilledBy(emptyName)) { std::printf("008d04e0_TEST FAIL\n"); return 1; }
    std::printf("008d04e0_TEST PASS\n");
    return 0;
}