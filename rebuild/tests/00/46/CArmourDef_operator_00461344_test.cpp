// Behaviour test for CArmourDef::operator= (0x00461344): base assignment, the three maps through
// their operator=, then the flag bytes and the two material ids.
#include <cstdio>

static int g_order[8];
static int g_calls = 0;

struct CParentDefClassBase {
    virtual void v00() {}
    unsigned char _body[0x38];
    CParentDefClassBase& operator=(const CParentDefClassBase& other)
    {
        for (int i = 0; i < 0x38; ++i) _body[i] = other._body[i];
        g_order[g_calls++] = 1;
        return *this;
    }
};

struct CAugmentationResponseMap {
    void*         m_allocator;
    void*         m_head;
    unsigned long m_size;
    CAugmentationResponseMap& operator=(const CAugmentationResponseMap& other)
    {
        m_size = other.m_size;
        g_order[g_calls++] = 2;
        return *this;
    }
};

struct CDamageAttributeMap {
    void*         m_allocator;
    void*         m_head;
    unsigned long m_size;
    CDamageAttributeMap& operator=(const CDamageAttributeMap& other)
    {
        m_size = other.m_size;
        g_order[g_calls++] = 3;
        return *this;
    }
};

struct CArmourDef : CParentDefClassBase {
    CAugmentationResponseMap AugmentationResponse;
    CDamageAttributeMap      DamageTypeResponse;
    CDamageAttributeMap      ArmourThresholds;
    bool                     AllHitsKnockdown;
    bool                     AllHitsNegated;
    bool                     AllHitsCauseRecoil;
    long                     DamageMaterial;
    long                     BlockMaterial;
    CArmourDef& operator=(const CArmourDef& other);
};

CArmourDef& CArmourDef::operator=(const CArmourDef& other)
{
    CParentDefClassBase::operator=(other);
    AugmentationResponse = other.AugmentationResponse;
    DamageTypeResponse = other.DamageTypeResponse;
    ArmourThresholds = other.ArmourThresholds;
    AllHitsKnockdown = other.AllHitsKnockdown;
    AllHitsNegated = other.AllHitsNegated;
    AllHitsCauseRecoil = other.AllHitsCauseRecoil;
    DamageMaterial = other.DamageMaterial;
    BlockMaterial = other.BlockMaterial;
    return *this;
}

int main()
{
    CArmourDef a, b;
    for (int i = 0; i < 0x38; ++i) { a._body[i] = 0; b._body[i] = (unsigned char)(i + 1); }
    a.AugmentationResponse.m_size = 0; b.AugmentationResponse.m_size = 7;
    a.DamageTypeResponse.m_size = 0;   b.DamageTypeResponse.m_size = 8;
    a.ArmourThresholds.m_size = 0;     b.ArmourThresholds.m_size = 9;
    a.AllHitsKnockdown = false; b.AllHitsKnockdown = true;
    a.AllHitsNegated = true;    b.AllHitsNegated = false;
    a.AllHitsCauseRecoil = false; b.AllHitsCauseRecoil = true;
    a.DamageMaterial = 0; b.DamageMaterial = 41;
    a.BlockMaterial = 0;  b.BlockMaterial = 42;
    CArmourDef& r = (a = b);
    if (&r != &a || g_calls != 4 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || g_order[3] != 3
        || a._body[0x37] != 0x38 || a.AugmentationResponse.m_size != 7 || a.DamageTypeResponse.m_size != 8
        || a.ArmourThresholds.m_size != 9 || !a.AllHitsKnockdown || a.AllHitsNegated || !a.AllHitsCauseRecoil
        || a.DamageMaterial != 41 || a.BlockMaterial != 42) {
        std::printf("00461344_TEST FAIL\n");
        return 1;
    }
    std::printf("00461344_TEST PASS\n");
    return 0;
}