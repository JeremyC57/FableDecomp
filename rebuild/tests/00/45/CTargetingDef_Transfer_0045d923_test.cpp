// Behaviour test for CTargetingDef::Transfer (0x0045D923): every field is handed to the matching typed
// transfer exactly once, in declaration order (ulong, bool, float x2, bool x5, float x13, map, float x4).
#include <cstdio>

struct CTargetingRangeMap { void* head; unsigned long size; unsigned long _alloc; };

static const void* g_addr[64];
static char g_kind[64];
static int g_n = 0;

class CPersistContext { public:
    void Transfer(unsigned long& v) { g_addr[g_n] = &v; g_kind[g_n++] = 'u'; }
    void Transfer(bool& v) { g_addr[g_n] = &v; g_kind[g_n++] = 'b'; }
    void Transfer(float& v) { g_addr[g_n] = &v; g_kind[g_n++] = 'f'; }
    void Transfer(CTargetingRangeMap& v) { g_addr[g_n] = &v; g_kind[g_n++] = 'm'; }
};

class CDefClassBase { public:
    virtual void Transfer(CPersistContext&) {}
    virtual ~CDefClassBase() {}
    unsigned char _body[0x24];
};

class CTargetingDef : public CDefClassBase { public:
    unsigned long Type;
    bool  UseGlowTargeting;
    float TargetArc;
    float TargetArcPCThirdPersonAimingMode;
    bool  TargetingBasePosFromCamera;
    bool  TargetingFacingDirFromCamera;
    bool  TargetingPCFacingDirFromCamera;
    bool  RejectTargetsBehindIfRunning;
    bool  AcceptOnlyEnemiesBehind;
    float EnemyBehindTargetingRange;
    float FirstPersonTargetingAngleFalloff;
    float FirstPersonTargetingDistanceFalloff;
    float ThirdPersonTargetingAngleFalloff;
    float ThirdPersonTargetingDistanceFalloff;
    float ZLockTargetSelectAngleFalloff;
    float ZLockTargetSelectDistanceFalloff;
    float FirstPersonTargetingOverrideShortRangeFactor;
    float FirstPersonTargetingOverrideShortRangeFalloff;
    float ThirdPersonTargetingOverrideShortRangeFactor;
    float ThirdPersonTargetingOverrideShortRangeFalloff;
    float FirstPersonTargetingEnemyPreferenceFactor;
    float ThirdPersonTargetingEnemyPreferenceFactor;
    CTargetingRangeMap TargetingRanges;
    float PreferPlayerWeight;
    float PreferNonCombatantsWeight;
    float PreferLastAttackerWeight;
    float PreferCurrentBestEnemyWeight;
    virtual void Transfer(CPersistContext& context);
};

void CTargetingDef::Transfer(CPersistContext& context)
{
    context.Transfer(Type);
    context.Transfer(UseGlowTargeting);
    context.Transfer(TargetArc);
    context.Transfer(TargetArcPCThirdPersonAimingMode);
    context.Transfer(TargetingBasePosFromCamera);
    context.Transfer(TargetingFacingDirFromCamera);
    context.Transfer(TargetingPCFacingDirFromCamera);
    context.Transfer(RejectTargetsBehindIfRunning);
    context.Transfer(AcceptOnlyEnemiesBehind);
    context.Transfer(EnemyBehindTargetingRange);
    context.Transfer(FirstPersonTargetingAngleFalloff);
    context.Transfer(FirstPersonTargetingDistanceFalloff);
    context.Transfer(ThirdPersonTargetingAngleFalloff);
    context.Transfer(ThirdPersonTargetingDistanceFalloff);
    context.Transfer(ZLockTargetSelectAngleFalloff);
    context.Transfer(ZLockTargetSelectDistanceFalloff);
    context.Transfer(FirstPersonTargetingOverrideShortRangeFactor);
    context.Transfer(FirstPersonTargetingOverrideShortRangeFalloff);
    context.Transfer(ThirdPersonTargetingOverrideShortRangeFactor);
    context.Transfer(ThirdPersonTargetingOverrideShortRangeFalloff);
    context.Transfer(FirstPersonTargetingEnemyPreferenceFactor);
    context.Transfer(ThirdPersonTargetingEnemyPreferenceFactor);
    context.Transfer(TargetingRanges);
    context.Transfer(PreferPlayerWeight);
    context.Transfer(PreferNonCombatantsWeight);
    context.Transfer(PreferLastAttackerWeight);
    context.Transfer(PreferCurrentBestEnemyWeight);
}

int main()
{
    CTargetingDef def;
    CPersistContext ctx;
    CDefClassBase* base = &def;
    base->Transfer(ctx);
    const char* kinds = "ubffbbbbbfffffffffffffmffff";
    int ok = (g_n == 27);
    for (int i = 0; ok && i < 27; ++i)
        if (g_kind[i] != kinds[i]) ok = 0;
    if (ok && (g_addr[0] != &def.Type || g_addr[1] != &def.UseGlowTargeting || g_addr[8] != &def.AcceptOnlyEnemiesBehind
               || g_addr[22] != &def.TargetingRanges || g_addr[26] != &def.PreferCurrentBestEnemyWeight))
        ok = 0;
    for (int i = 1; ok && i < 27; ++i)
        if ((const char*)g_addr[i] <= (const char*)g_addr[i - 1]) ok = 0;   // strictly ascending member order
    if (!ok) { std::printf("0045d923_TEST FAIL\n"); return 1; }
    std::printf("0045d923_TEST PASS\n");
    return 0;
}