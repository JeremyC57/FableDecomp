// Behaviour test for CConfigOptionsDefaultsDef::operator= (0x004345A0): the inlined CDefClassBase chain
// (root operator= spy, manager/index words, the three flag bits, instantiation name, sub-def vector map
// spy) and the fourteen config words; checks *this returns and the bit fields are copied bit by bit.
#include <cstdio>
#ifndef _MSC_VER
#define __forceinline inline
#endif

static int g_order[4];
static int g_calls = 0;

#pragma pack(push, 1)
struct CDefRootBase {
    virtual void v00() {}
    unsigned long m_words[6];
    CDefRootBase& operator=(const CDefRootBase& other)
    { for (int i = 0; i < 6; ++i) m_words[i] = other.m_words[i]; g_order[g_calls++] = 1; return *this; }
};
struct CDefinitionManager;
struct CDefClassBase : CDefRootBase {
    CDefinitionManager* PDefManager;
    unsigned long GlobalIndex;
    unsigned char Setup : 1;
    unsigned char Template : 1;
    unsigned char DefaultValsApplied : 1;
    __forceinline CDefClassBase& operator=(const CDefClassBase& other)
    {
        CDefRootBase::operator=(other);
        PDefManager = other.PDefManager;
        GlobalIndex = other.GlobalIndex;
        Setup = other.Setup;
        Template = other.Template;
        DefaultValsApplied = other.DefaultValsApplied;
        return *this;
    }
};
#pragma pack(pop)

template<class T> struct CRetailVector {
    T* m_first; T* m_last; T* m_end;
    CRetailVector& operator=(const CRetailVector& o)
    { m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; g_order[g_calls++] = 2; return *this; }
};
struct CKeyPairCompareLess {};
template<class K, class V> struct CVectorMap {
    CRetailVector<V>    m_vector;
    CKeyPairCompareLess m_compare;
    bool                m_sorted;
    __forceinline CVectorMap& operator=(const CVectorMap& other)
    {
        m_vector = other.m_vector;
        m_compare = other.m_compare;
        m_sorted = other.m_sorted;
        return *this;
    }
};
struct CDefString { long TablePos; };
struct CSubDefInfo { long m_key; };

#pragma pack(push, 1)
struct CParentDefClassBase : CDefClassBase {
    CDefString                             InstantiationName;
    CVectorMap<unsigned long, CSubDefInfo> SubDefInfoMap;
    __forceinline CParentDefClassBase& operator=(const CParentDefClassBase& other)
    {
        CDefClassBase::operator=(other);
        InstantiationName = other.InstantiationName;
        SubDefInfoMap = other.SubDefInfoMap;
        return *this;
    }
};
#pragma pack(pop)

struct CConfigOptionsDefaultsDef : CParentDefClassBase {
    unsigned long Antialiasing;
    long  ResolutionWidth;
    long  ResolutionHeight;
    long  BitDepth;
    long  MinResolutionWidth;
    long  MinResolutionHeight;
    float TextureDetail;
    float MaxTextureDetail;
    float ShadowDetail;
    float MaxShadowDetail;
    float MeshDetail;
    float MaxMeshDetail;
    float EffectsDetail;
    float MaxEffectsDetail;
    CConfigOptionsDefaultsDef& operator=(const CConfigOptionsDefaultsDef& other);
};

CConfigOptionsDefaultsDef& CConfigOptionsDefaultsDef::operator=(const CConfigOptionsDefaultsDef& other)
{
    CParentDefClassBase::operator=(other);
    Antialiasing = other.Antialiasing;
    ResolutionWidth = other.ResolutionWidth;
    ResolutionHeight = other.ResolutionHeight;
    BitDepth = other.BitDepth;
    MinResolutionWidth = other.MinResolutionWidth;
    MinResolutionHeight = other.MinResolutionHeight;
    TextureDetail = other.TextureDetail;
    MaxTextureDetail = other.MaxTextureDetail;
    ShadowDetail = other.ShadowDetail;
    MaxShadowDetail = other.MaxShadowDetail;
    MeshDetail = other.MeshDetail;
    MaxMeshDetail = other.MaxMeshDetail;
    EffectsDetail = other.EffectsDetail;
    MaxEffectsDetail = other.MaxEffectsDetail;
    return *this;
}

int main()
{
    static CSubDefInfo sub[2];
    CConfigOptionsDefaultsDef a, b;
    int i;
    for (i = 0; i < 6; ++i) { a.m_words[i] = 0; b.m_words[i] = 0x10 + i; }
    a.PDefManager = 0; b.PDefManager = (CDefinitionManager*)&b;
    a.GlobalIndex = 0; b.GlobalIndex = 42;
    a.Setup = 0; a.Template = 1; a.DefaultValsApplied = 0;
    b.Setup = 1; b.Template = 0; b.DefaultValsApplied = 1;
    a.InstantiationName.TablePos = 0; b.InstantiationName.TablePos = -5;
    a.SubDefInfoMap.m_vector.m_first = a.SubDefInfoMap.m_vector.m_last = a.SubDefInfoMap.m_vector.m_end = 0;
    a.SubDefInfoMap.m_sorted = false;
    b.SubDefInfoMap.m_vector.m_first = sub; b.SubDefInfoMap.m_vector.m_last = sub + 2; b.SubDefInfoMap.m_vector.m_end = sub + 2;
    b.SubDefInfoMap.m_sorted = true;
    a.Antialiasing = 0; b.Antialiasing = 4;
    long* la[] = { &a.ResolutionWidth, &a.ResolutionHeight, &a.BitDepth, &a.MinResolutionWidth, &a.MinResolutionHeight };
    long* lb[] = { &b.ResolutionWidth, &b.ResolutionHeight, &b.BitDepth, &b.MinResolutionWidth, &b.MinResolutionHeight };
    for (i = 0; i < 5; ++i) { *la[i] = 0; *lb[i] = 640 + i; }
    float* fa[] = { &a.TextureDetail, &a.MaxTextureDetail, &a.ShadowDetail, &a.MaxShadowDetail, &a.MeshDetail, &a.MaxMeshDetail, &a.EffectsDetail, &a.MaxEffectsDetail };
    float* fb[] = { &b.TextureDetail, &b.MaxTextureDetail, &b.ShadowDetail, &b.MaxShadowDetail, &b.MeshDetail, &b.MaxMeshDetail, &b.EffectsDetail, &b.MaxEffectsDetail };
    for (i = 0; i < 8; ++i) { *fa[i] = 0.0f; *fb[i] = 0.125f * (float)(i + 1); }
    CConfigOptionsDefaultsDef& r = (a = b);
    bool ok = (&r == &a) && g_calls == 2 && g_order[0] == 1 && g_order[1] == 2;
    for (i = 0; i < 6; ++i) if (a.m_words[i] != (unsigned long)(0x10 + i)) ok = false;
    if (a.PDefManager != (CDefinitionManager*)&b || a.GlobalIndex != 42 || a.Setup != 1 || a.Template != 0 || a.DefaultValsApplied != 1
        || a.InstantiationName.TablePos != -5 || a.SubDefInfoMap.m_vector.m_first != sub || a.SubDefInfoMap.m_vector.m_end != sub + 2
        || !a.SubDefInfoMap.m_sorted || a.Antialiasing != 4)
        ok = false;
    for (i = 0; i < 5; ++i) if (*la[i] != 640 + i) ok = false;
    for (i = 0; i < 8; ++i) if (*fa[i] != 0.125f * (float)(i + 1)) ok = false;
    if (!ok) { std::printf("004345a0_TEST FAIL\n"); return 1; }
    std::printf("004345a0_TEST PASS\n");
    return 0;
}