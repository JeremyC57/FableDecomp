// Behaviour test for CTargetingDef::operator= (0x0045D729): copies the model, spies the base and map
// assignments (order and count), and checks every scalar lands and *this comes back.
#include <cstdio>

static int g_order[4];
static int g_calls = 0;

class CDefClassBase { public:
    virtual void v00() {}
    unsigned char _body[0x24];
    CDefClassBase& operator=(const CDefClassBase& other)
    {
        for (int i = 0; i < 0x24; ++i) _body[i] = other._body[i];
        g_order[g_calls++] = 1;
        return *this;
    }
};

struct CRetailTree_0045d871 {
    struct node;
    node* head; unsigned long size; unsigned long _alloc;
    CRetailTree_0045d871& operator=(const CRetailTree_0045d871& o)
    { head = o.head; size = o.size; _alloc = o._alloc; g_order[g_calls++] = 2; return *this; }
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
    CRetailTree_0045d871 TargetingRanges;
    float PreferPlayerWeight;
    float PreferNonCombatantsWeight;
    float PreferLastAttackerWeight;
    float PreferCurrentBestEnemyWeight;
    CTargetingDef& operator=(const CTargetingDef& other);
};

CTargetingDef& CTargetingDef::operator=(const CTargetingDef& other)
{
    CDefClassBase::operator=(other);
    Type = other.Type;
    UseGlowTargeting = other.UseGlowTargeting;
    TargetArc = other.TargetArc;
    TargetArcPCThirdPersonAimingMode = other.TargetArcPCThirdPersonAimingMode;
    TargetingBasePosFromCamera = other.TargetingBasePosFromCamera;
    TargetingFacingDirFromCamera = other.TargetingFacingDirFromCamera;
    TargetingPCFacingDirFromCamera = other.TargetingPCFacingDirFromCamera;
    RejectTargetsBehindIfRunning = other.RejectTargetsBehindIfRunning;
    AcceptOnlyEnemiesBehind = other.AcceptOnlyEnemiesBehind;
    EnemyBehindTargetingRange = other.EnemyBehindTargetingRange;
    FirstPersonTargetingAngleFalloff = other.FirstPersonTargetingAngleFalloff;
    FirstPersonTargetingDistanceFalloff = other.FirstPersonTargetingDistanceFalloff;
    ThirdPersonTargetingAngleFalloff = other.ThirdPersonTargetingAngleFalloff;
    ThirdPersonTargetingDistanceFalloff = other.ThirdPersonTargetingDistanceFalloff;
    ZLockTargetSelectAngleFalloff = other.ZLockTargetSelectAngleFalloff;
    ZLockTargetSelectDistanceFalloff = other.ZLockTargetSelectDistanceFalloff;
    FirstPersonTargetingOverrideShortRangeFactor = other.FirstPersonTargetingOverrideShortRangeFactor;
    FirstPersonTargetingOverrideShortRangeFalloff = other.FirstPersonTargetingOverrideShortRangeFalloff;
    ThirdPersonTargetingOverrideShortRangeFactor = other.ThirdPersonTargetingOverrideShortRangeFactor;
    ThirdPersonTargetingOverrideShortRangeFalloff = other.ThirdPersonTargetingOverrideShortRangeFalloff;
    FirstPersonTargetingEnemyPreferenceFactor = other.FirstPersonTargetingEnemyPreferenceFactor;
    ThirdPersonTargetingEnemyPreferenceFactor = other.ThirdPersonTargetingEnemyPreferenceFactor;
    TargetingRanges = other.TargetingRanges;
    PreferPlayerWeight = other.PreferPlayerWeight;
    PreferNonCombatantsWeight = other.PreferNonCombatantsWeight;
    PreferLastAttackerWeight = other.PreferLastAttackerWeight;
    PreferCurrentBestEnemyWeight = other.PreferCurrentBestEnemyWeight;
    return *this;
}

int main()
{
    CTargetingDef a, b;
    float* fa = &a.TargetArc; float* fb = &b.TargetArc;
    for (int i = 0; i < 0x24; ++i) { a._body[i] = 0; b._body[i] = (unsigned char)(i + 1); }
    a.Type = 0; b.Type = 7;
    a.UseGlowTargeting = false; b.UseGlowTargeting = true;
    a.TargetArc = 0.0f; b.TargetArc = 1.5f;
    a.TargetArcPCThirdPersonAimingMode = 0.0f; b.TargetArcPCThirdPersonAimingMode = 2.5f;
    a.TargetingBasePosFromCamera = false; b.TargetingBasePosFromCamera = true;
    a.TargetingFacingDirFromCamera = true; b.TargetingFacingDirFromCamera = false;
    a.TargetingPCFacingDirFromCamera = false; b.TargetingPCFacingDirFromCamera = true;
    a.RejectTargetsBehindIfRunning = false; b.RejectTargetsBehindIfRunning = true;
    a.AcceptOnlyEnemiesBehind = true; b.AcceptOnlyEnemiesBehind = false;
    float* pa[] = { &a.EnemyBehindTargetingRange, &a.FirstPersonTargetingAngleFalloff, &a.FirstPersonTargetingDistanceFalloff,
        &a.ThirdPersonTargetingAngleFalloff, &a.ThirdPersonTargetingDistanceFalloff, &a.ZLockTargetSelectAngleFalloff,
        &a.ZLockTargetSelectDistanceFalloff, &a.FirstPersonTargetingOverrideShortRangeFactor, &a.FirstPersonTargetingOverrideShortRangeFalloff,
        &a.ThirdPersonTargetingOverrideShortRangeFactor, &a.ThirdPersonTargetingOverrideShortRangeFalloff,
        &a.FirstPersonTargetingEnemyPreferenceFactor, &a.ThirdPersonTargetingEnemyPreferenceFactor,
        &a.PreferPlayerWeight, &a.PreferNonCombatantsWeight, &a.PreferLastAttackerWeight, &a.PreferCurrentBestEnemyWeight };
    float* pb[] = { &b.EnemyBehindTargetingRange, &b.FirstPersonTargetingAngleFalloff, &b.FirstPersonTargetingDistanceFalloff,
        &b.ThirdPersonTargetingAngleFalloff, &b.ThirdPersonTargetingDistanceFalloff, &b.ZLockTargetSelectAngleFalloff,
        &b.ZLockTargetSelectDistanceFalloff, &b.FirstPersonTargetingOverrideShortRangeFactor, &b.FirstPersonTargetingOverrideShortRangeFalloff,
        &b.ThirdPersonTargetingOverrideShortRangeFactor, &b.ThirdPersonTargetingOverrideShortRangeFalloff,
        &b.FirstPersonTargetingEnemyPreferenceFactor, &b.ThirdPersonTargetingEnemyPreferenceFactor,
        &b.PreferPlayerWeight, &b.PreferNonCombatantsWeight, &b.PreferLastAttackerWeight, &b.PreferCurrentBestEnemyWeight };
    int i;
    for (i = 0; i < 17; ++i) { *pa[i] = 0.0f; *pb[i] = (float)(i + 1) * 0.5f; }
    a.TargetingRanges.head = 0; a.TargetingRanges.size = 0; a.TargetingRanges._alloc = 0;
    b.TargetingRanges.head = (CRetailTree_0045d871::node*)&b; b.TargetingRanges.size = 4; b.TargetingRanges._alloc = 9;
    (void)fa; (void)fb;
    CTargetingDef& r = (a = b);
    bool ok = (&r == &a) && g_calls == 2 && g_order[0] == 1 && g_order[1] == 2;
    for (i = 0; i < 0x24; ++i) if (a._body[i] != (unsigned char)(i + 1)) ok = false;
    for (i = 0; i < 17; ++i) if (*pa[i] != (float)(i + 1) * 0.5f) ok = false;
    if (a.Type != 7 || !a.UseGlowTargeting || a.TargetArc != 1.5f || a.TargetArcPCThirdPersonAimingMode != 2.5f
        || !a.TargetingBasePosFromCamera || a.TargetingFacingDirFromCamera || !a.TargetingPCFacingDirFromCamera
        || !a.RejectTargetsBehindIfRunning || a.AcceptOnlyEnemiesBehind
        || a.TargetingRanges.head != (CRetailTree_0045d871::node*)&b || a.TargetingRanges.size != 4 || a.TargetingRanges._alloc != 9)
        ok = false;
    if (!ok) { std::printf("0045d729_TEST FAIL\n"); return 1; }
    std::printf("0045d729_TEST PASS\n");
    return 0;
}