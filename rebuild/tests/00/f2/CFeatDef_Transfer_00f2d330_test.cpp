// Behaviour test for 0x00F2D330 CFeatDef::Transfer(CPersistContext&): the 15 keyed transfers in member order, each bound
// to the right member and given its default (empty def string, 0.0f, 0, false, attack type 0), checked through a recording context.
#include <cstdio>
#include <cstring>
static char gLog[2048];
static void Log(const char* s) { std::strcat(gLog, s); std::strcat(gLog, ";"); }
class CDefString { public: long TablePos; CDefString() : TablePos(-1) { } };
enum EFeatAttackType { FEAT_ATTACK_TYPE_0 = 0, FEAT_ATTACK_TYPE_2 = 2 };
static void* gBase;
static void Rec(const char* key, const void* member, const char* kind, long def)
{
    char t[96]; std::sprintf(t, "%s@%d:%s=%ld", key, (int)((const char*)member - (const char*)gBase), kind, def); Log(t);
}
class CPersistContext { public:
    void Transfer(const char* key, CDefString& v, const CDefString& d) { Rec(key, &v, "s", d.TablePos); v.TablePos = 7; }
    void Transfer(const char* key, float& v, const float& d) { Rec(key, &v, "f", (long)(d * 10)); v = 1.5f; }
    void Transfer(const char* key, long& v, const long& d) { Rec(key, &v, "l", d); v = 42; }
    void Transfer(const char* key, bool& v, const bool& d) { Rec(key, &v, "b", d ? 1 : 0); v = true; }
    void Transfer(const char* key, EFeatAttackType& v, const EFeatAttackType& d) { Rec(key, &v, "e", (long)d); v = FEAT_ATTACK_TYPE_2; }
};
class CFeatDef { public:
    virtual ~CFeatDef() { }
    virtual void Transfer(CPersistContext& context);
    CDefString      FeatName;
    CDefString      Verb;
    float           TimeLimit;
    long            TargetNumber;
    long            GoldReward;
    long            XPReward;
    CDefString      ItemReward;
    bool            NoBlocking;
    EFeatAttackType KN_AttackType;
    bool            KN_Perfect;
    long            KN_CreatureType;
    CDefString      GF_FromRegion;
    CDefString      GF_ToRegion;
    bool            GF_NoTeleporting;
    CDefString      CO_ItemName;
};
void CFeatDef::Transfer(CPersistContext& context)
{
    context.Transfer("FeatName", FeatName, CDefString());
    context.Transfer("Verb", Verb, CDefString());
    context.Transfer("TimeLimit", TimeLimit, 0.0f);
    context.Transfer("TargetNumber", TargetNumber, 0L);
    context.Transfer("GoldReward", GoldReward, 0L);
    context.Transfer("XPReward", XPReward, 0L);
    context.Transfer("ItemReward", ItemReward, CDefString());
    context.Transfer("NoBlocking", NoBlocking, false);
    context.Transfer("KN_AttackType", KN_AttackType, EFeatAttackType(0));
    context.Transfer("KN_Perfect", KN_Perfect, false);
    context.Transfer("KN_CreatureType", KN_CreatureType, 0L);
    context.Transfer("GF_FromRegion", GF_FromRegion, CDefString());
    context.Transfer("GF_ToRegion", GF_ToRegion, CDefString());
    context.Transfer("GF_NoTeleporting", GF_NoTeleporting, false);
    context.Transfer("CO_ItemName", CO_ItemName, CDefString());
}
int main()
{
    CFeatDef def;
    gBase = &def;
    char want[2048]; want[0] = 0;
#define W(key, m, kind, d) { char t[96]; std::sprintf(t, "%s@%d:%s=%ld;", key, (int)((char*)&def.m - (char*)&def), kind, (long)(d)); std::strcat(want, t); }
    W("FeatName", FeatName, "s", -1) W("Verb", Verb, "s", -1) W("TimeLimit", TimeLimit, "f", 0) W("TargetNumber", TargetNumber, "l", 0)
    W("GoldReward", GoldReward, "l", 0) W("XPReward", XPReward, "l", 0) W("ItemReward", ItemReward, "s", -1) W("NoBlocking", NoBlocking, "b", 0)
    W("KN_AttackType", KN_AttackType, "e", 0) W("KN_Perfect", KN_Perfect, "b", 0) W("KN_CreatureType", KN_CreatureType, "l", 0)
    W("GF_FromRegion", GF_FromRegion, "s", -1) W("GF_ToRegion", GF_ToRegion, "s", -1) W("GF_NoTeleporting", GF_NoTeleporting, "b", 0)
    W("CO_ItemName", CO_ItemName, "s", -1)
    CPersistContext ctx;
    def.Transfer(ctx);
    int ok = std::strcmp(gLog, want) == 0;
    ok = ok && def.FeatName.TablePos == 7 && def.CO_ItemName.TablePos == 7 && def.TimeLimit == 1.5f && def.XPReward == 42
            && def.KN_CreatureType == 42 && def.NoBlocking && def.GF_NoTeleporting && def.KN_AttackType == FEAT_ATTACK_TYPE_2;
    if (!ok) { std::printf("%s\n%s\n00f2d330_TEST FAIL\n", gLog, want); return 1; }
    std::printf("00f2d330_TEST PASS\n");
    return 0;
}