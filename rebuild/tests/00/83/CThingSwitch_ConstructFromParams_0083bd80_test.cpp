#include <cstdio>
#include <cstring>
struct C3DVector { float x, y, z; };
struct CThingPhysical;
static int g_live = 0;
struct CCharString {
    char Text[32];
    CCharString(const char* text, long = -1) { std::strcpy(Text, text); ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(Text, o.Text); ++g_live; }
    ~CCharString() { --g_live; }
    CCharString& operator=(const CCharString& o) { std::strcpy(Text, o.Text); return *this; }
};
static bool g_baseOk = true;
static char g_baseName[32];
static long g_baseIndex = 0;
struct CThingGameObject {
    float MaxHealth, Health;
    bool ConstructFromParams(long defIndex, const C3DVector*, long a3, unsigned long a4, const CCharString& scriptName,
                             CThingPhysical* c, CThingPhysical* oc, bool a8)
    {
        std::strcpy(g_baseName, scriptName.Text); g_baseIndex = defIndex;
        return g_baseOk && a3 == -1 && a4 == 0 && c == 0 && oc == 0 && a8;
    }
};
struct CThingSwitch : CThingGameObject {
    float TriggerRadius;
    bool Triggered;
    CCharString TriggerableObjectScriptName;
    long TimesTriggered;
    CThingSwitch() : TriggerableObjectScriptName("old") {}
    bool ConstructFromParams(long defIndex, const C3DVector* pos, float triggerRadius, CCharString name);
};
bool CThingSwitch::ConstructFromParams(long defIndex, const C3DVector* pos, float triggerRadius, CCharString triggerableObjectScriptName)
{
    bool constructed;
    {
        CCharString noScriptName("");
        constructed = CThingGameObject::ConstructFromParams(defIndex, pos, -1, 0, noScriptName, 0, 0, true);
    }
    if (constructed) {
        Triggered = false;
        TriggerRadius = triggerRadius;
        TriggerableObjectScriptName = triggerableObjectScriptName;
        TimesTriggered = 0;
        Health = 1.0f;
        MaxHealth = 1.0f;
        return true;
    }
    return false;
}
static int Fail() { std::printf("0083bd80_TEST FAIL\n"); return 1; }
int main()
{
    C3DVector p = { 1, 2, 3 };
    CThingSwitch s; s.Triggered = true; s.TimesTriggered = 5; s.Health = 0; s.MaxHealth = 0; s.TriggerRadius = 0;
    bool r1 = s.ConstructFromParams(7, &p, 4.5f, CCharString("DOOR"));
    if (!r1 || g_baseIndex != 7 || g_baseName[0] != 0) return Fail();
    if (s.Triggered || s.TriggerRadius != 4.5f || std::strcmp(s.TriggerableObjectScriptName.Text, "DOOR") || s.TimesTriggered != 0
        || s.Health != 1.0f || s.MaxHealth != 1.0f || g_live != 1) return Fail();
    g_baseOk = false; s.TimesTriggered = 3;
    bool r2 = s.ConstructFromParams(8, &p, 2.0f, CCharString("X"));
    if (r2 || s.TimesTriggered != 3 || s.TriggerRadius != 4.5f || g_live != 1) return Fail();
    std::printf("0083bd80_TEST PASS\n");
    return 0;
}