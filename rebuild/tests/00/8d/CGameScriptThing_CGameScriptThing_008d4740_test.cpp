// Behaviour test for 0x008D4740 implicit ~CGameScriptThing: DataString, Name, PThing, then the empty ~CScriptThing and ~CBaseClass.
#include <cstdio>
#include <cstring>
static char g_log[32];
static void Log(const char* s) { std::strcat(g_log, s); }
class CCharString { public: CCharString(char c) : C(c) { } ~CCharString() { char b[2] = { C, 0 }; Log(b); } char C; };
class CIntelligentPointer { public: virtual ~CIntelligentPointer() { Log("P"); } void* PData; };
class CBaseClass { public: virtual ~CBaseClass() { Log("B"); } };
class CScriptThing : public CBaseClass { public: virtual ~CScriptThing() { } };
class CWorld { };
class CGameScriptThing : public CScriptThing {
public:
    CGameScriptThing(CWorld& world) : Name('N'), DataString('D'), World(&world) { }
    CIntelligentPointer PThing;
    long ScriptCounter;
    unsigned long ThingUIDLow, ThingUIDHigh;
    CCharString Name;
    CCharString DataString;
    float Pos[3], HomePos[3], AngleXY;
    CWorld* World;
};
int main()
{
    CWorld world;
    g_log[0] = 0;
    CScriptThing* thing = new CGameScriptThing(world);
    delete thing;
    bool ok = std::strcmp(g_log, "DNPB") == 0;
    std::printf(ok ? "008d4740_TEST PASS\n" : "008d4740_TEST FAIL\n");
    return ok ? 0 : 1;
}