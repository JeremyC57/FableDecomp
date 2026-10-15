// Behaviour test for 0x00781200 CFirePoint::CFirePoint: the helper name is copied, the indices,
// resistance and parent stored, Burning false, FireDamageAfterFrame 0, both effect pointers empty.
#include <cstdio>
#include <cstring>
#include <new>
struct CThing { long m_id; };
struct CBaseObject;
static int g_copies;
struct CCharString {
    void* PStringData;
    CCharString(void* data) : PStringData(data) { }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
};
static int g_ipCtor;
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ipCtor; }
};
struct CIntelligentPointer : CBaseIntelligentPointer { CIntelligentPointer() : CBaseIntelligentPointer() { } };
struct CFirePoint {
    CCharString         HelperName;
    long                HelperIndex;
    long                EffectIndex;
    long                Resistance;
    bool                Burning;
    long                FireDamageAfterFrame;
    CThing*             PParentThing;
    CIntelligentPointer PEffect;
    CIntelligentPointer PSingeEffect;
    CFirePoint(CThing* parentThing, const CCharString& helperName, long helperIndex, long effectIndex, long resistance)
        : HelperName(helperName), HelperIndex(helperIndex), EffectIndex(effectIndex), Resistance(resistance)
        , Burning(false), FireDamageAfterFrame(0), PParentThing(parentThing) { }
};
static double g_storage[32];

int main()
{
    CThing parent;
    int data;
    CCharString name(&data);
    std::memset(g_storage, 0xAB, sizeof g_storage);
    CFirePoint* p = new (g_storage) CFirePoint(&parent, name, 3, 7, 11);
    if (g_copies != 1 || p->HelperName.PStringData != &data || p->HelperIndex != 3 || p->EffectIndex != 7 || p->Resistance != 11) { std::printf("00781200_TEST FAIL\n"); return 1; }
    if (p->Burning || p->FireDamageAfterFrame != 0 || p->PParentThing != &parent || g_ipCtor != 2 || p->PEffect.m_pItem != 0 || p->PSingeEffect.m_pItem != 0) { std::printf("00781200_TEST FAIL\n"); return 1; }
    std::printf("00781200_TEST PASS\n");
    return 0;
}
