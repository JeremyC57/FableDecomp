// Behaviour test for 0x004DDA37 CTCTargetingAI::CTCTargetingAI: the targeting base gets the thing, both
// creature pointers start empty and the ThingsToNotAttack list is built with an allocator.
#include <cstdio>
#include <cstring>
#include <new>
class CThing { public: long m_id; };
static int g_baseCtor;
static CThing* g_baseThing;
class CTCTargetingBase { public:
    virtual void v00() { }
    unsigned char _pad04[0x30];
    CTCTargetingBase(CThing* thing) { ++g_baseCtor; g_baseThing = thing; }
};
class CBaseObject;
static int g_ipCtor;
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ipCtor; }
};
struct CCreatureIntelligentPointer : CBaseIntelligentPointer { CCreatureIntelligentPointer() : CBaseIntelligentPointer() { } };
struct CRetailAllocator { CRetailAllocator() { } };
static int g_listCtor;
struct CRetailList_node0x10 {
    void* head;
    CRetailList_node0x10(const CRetailAllocator& al = CRetailAllocator()) : head(0) { (void)al; ++g_listCtor; }
};
class CTCTargetingAI : public CTCTargetingBase { public:
    CCreatureIntelligentPointer PBestEnemyCreatureForAttack;
    CCreatureIntelligentPointer PScriptTargetOverride;
    CRetailList_node0x10        ThingsToNotAttack;
    CTCTargetingAI(CThing* thing) : CTCTargetingBase(thing) { }
};
static double g_storage[64];

int main()
{
    CThing thing;
    std::memset(g_storage, 0xAB, sizeof g_storage);
    CTCTargetingAI* p = new (g_storage) CTCTargetingAI(&thing);
    if (g_baseCtor != 1 || g_baseThing != &thing || g_ipCtor != 2 || g_listCtor != 1 || p->PBestEnemyCreatureForAttack.m_pItem != 0 || p->PScriptTargetOverride.m_pItem != 0) { std::printf("004dda37_TEST FAIL\n"); return 1; }
    std::printf("004dda37_TEST PASS\n");
    return 0;
}