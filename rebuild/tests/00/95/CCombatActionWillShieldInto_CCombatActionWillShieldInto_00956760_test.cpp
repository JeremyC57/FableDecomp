#include <cstdio>
#include <cstring>

class CThingCreatureBase { public: int Id; };

static const char* g_soundName = 0;
static const CThingCreatureBase* g_soundOwner = 0;
static CThingCreatureBase* g_baseOwner = 0;
static long g_baseCode = 0;
static long g_baseFirst = 0;
static int g_strings = 0;

class CCharString {
public:
    CCharString(const char* text, long length = -1) : PStringData((void*)text) { (void)length; ++g_strings; }
    ~CCharString() { --g_strings; }
    void* PStringData;
};

class CSoundPair {
public:
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

class CCombatActionBase {
public:
    CCombatActionBase(CThingCreatureBase& owner, long actionCode, const CSoundPair& sound)
    { g_baseOwner = &owner; g_baseCode = actionCode; g_baseFirst = sound.FirstSound; }
    virtual ~CCombatActionBase() { }
protected:
    static CSoundPair __fastcall GetSoundPair(const CThingCreatureBase& owner, const CCharString& name)
    { g_soundOwner = &owner; g_soundName = (const char*)name.PStringData; CSoundPair p; p.FirstSound = 7; p.LastSound = 9; return p; }
    unsigned char _body[0x114 - 4];
};

class CCombatAction_WillShieldInto : public CCombatActionBase {
public:
    CCombatAction_WillShieldInto(CThingCreatureBase& owner, float shieldUpTime, float shieldHealth);
    virtual ~CCombatAction_WillShieldInto() { }
    float ShieldUpTime;
    float ShieldHealth;
};

CCombatAction_WillShieldInto::CCombatAction_WillShieldInto(CThingCreatureBase& owner, float shieldUpTime, float shieldHealth)
    : CCombatActionBase(owner, 150, GetSoundPair(owner, "SWORD_SWING")),
      ShieldUpTime(shieldUpTime),
      ShieldHealth(shieldHealth)
{
}

int main()
{
    CThingCreatureBase owner; owner.Id = 3;
    CCombatAction_WillShieldInto a(owner, 2.5f, 40.0f);
    int ok = g_baseOwner == &owner && g_soundOwner == &owner && g_baseCode == 150 && g_baseFirst == 7
          && g_soundName && std::strcmp(g_soundName, "SWORD_SWING") == 0 && g_strings == 0
          && a.ShieldUpTime > 2.49f && a.ShieldUpTime < 2.51f && a.ShieldHealth > 39.9f && a.ShieldHealth < 40.1f
          && (char*)&a.ShieldUpTime - (char*)&a == 0x114 && (char*)&a.ShieldHealth - (char*)&a == 0x118;
    std::printf(ok ? "00956760_TEST PASS\n" : "00956760_TEST FAIL\n");
    return ok ? 0 : 1;
}
