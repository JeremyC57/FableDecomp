// Behaviour test for CTCCombat::CTCCombat (0x00735540): base + combat manager from the world, empty containers (the
// multimap header allocated and self-linked), index -1, zeroed scalars, the flourish flag cleared before the reset call.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
struct CThing;
struct CCombatManager;
template <class T> struct CAllocator {                  // STLport allocator<T>: empty
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> { // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {                 // STLport _Vector_base<T, allocator<T> >
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template <class T> struct CVector : CVectorBase<T> {    // STLport vector<T> (12 bytes)
    CVector() : CVectorBase<T>(CAllocator<T>()) { }
};
struct SRbTreeNode {                                    // header / node of the ability multimap (0x24 bytes)
    unsigned char m_color;                              // +0x00 (0 = red)
    SRbTreeNode*  m_pParent;                            // +0x04
    SRbTreeNode*  m_pLeft;                              // +0x08
    SRbTreeNode*  m_pRight;                             // +0x0c
    unsigned char m_value[0x14];                        // +0x10 pair<ECombatAbilityDataType, pair<CCountedPointer, CCombatAbility>>
};
struct CRbTreeBase {                                    // STLport _Rb_tree_base
    SRbTreeNode* m_pHeader;
    CRbTreeBase() : m_pHeader(0)
    {
        m_pHeader = (SRbTreeNode*)malloc(sizeof(SRbTreeNode));
    }
};
struct CRbTree : CRbTreeBase {                          // STLport _Rb_tree (12 bytes with the empty compare)
    unsigned long m_nodeCount;
    unsigned char m_compare;
    CRbTree() : m_nodeCount(0)
    {
        m_pHeader->m_color = 0;
        m_pHeader->m_pParent = 0;
        m_pHeader->m_pLeft = m_pHeader;
        m_pHeader->m_pRight = m_pHeader;
    }
};
struct CDefPointer {
    void* Object;
    CDefPointer() : Object(0) { }
};
struct CWorld {
    unsigned char   _pad00[0x4c];
    CCombatManager* PCombatManager;                     // +0x4c
};
static CWorld* g_world = 0; static int g_resets = 0; static long g_resetSawIndex = 0; static bool g_resetSawFlag = true;
struct CTCBase {
    virtual void v00() { }
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CTCBase(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
    CWorld* GetWorld() { return g_world; }
};
struct CThingCreatureBase;
struct CTCCombat : CTCBase {
    long                ComboChainPos;                  // +0x0c
    long                ComboChainID;                   // +0x10
    long                ComboChainGracePeriodCountdown; // +0x14
    long                HandednessGracePeriodCountdown; // +0x18
    long                UnarmedCombatIdleCountdown;     // +0x1c
    CCombatManager*     CombatManager;                  // +0x20
    CThingCreatureBase* ParentCreature;                 // +0x24
    CDefPointer         PCombatTypeDef;                 // +0x28
    CVector<CDefPointer> AbilityList;                   // +0x2c
    CVector<CDefPointer> UsableAbilityList;             // +0x38
    long                CurrentAbilityIndex;            // +0x44
    CDefPointer         PBlockCounterAbility;           // +0x48
    CDefPointer         PFlourish360Ability;            // +0x4c
    long                BlockCounterTimer;              // +0x50
    CVector<void*>      AlwaysBlockThings;              // +0x54 vector<CIntelligentPointer<const CThing>>
    bool                PerformingAction;               // +0x60
    bool                PerformingStrikeAction;         // +0x61
    bool                StartingToBlock;                // +0x62
    bool                Blocking;                       // +0x63
    long                BlockHoldTimer;                 // +0x64
    long                BlockingNumFrames;              // +0x68
    long                UnsheathingWeaponType;          // +0x6c
    CRbTree             CombatAbilities;                // +0x70
    long                ShotOverrideType;               // +0x7c
    bool                ReadyForMelee;                  // +0x80
    long                CurFlourishTimer;               // +0x84
    CVector<void*>      FlourishParticles;              // +0x88 vector<CIntelligentPointer<CThing>>
    bool                IsInMiddleOfFlourishAction;     // +0x94
    virtual void v00() { }
    CTCCombat(CThing* thing);
    void ResetCombatState() { ++g_resets; g_resetSawIndex = CurrentAbilityIndex; g_resetSawFlag = IsInMiddleOfFlourishAction; }
};

CTCCombat::CTCCombat(CThing* thing)
    : CTCBase(thing)
    , CombatManager(GetWorld()->PCombatManager)
    , ParentCreature(0)
    , CurrentAbilityIndex(-1)
    , UnsheathingWeaponType(0)
    , ShotOverrideType(0)
    , CurFlourishTimer(0)
{
    IsInMiddleOfFlourishAction = false;
    ResetCombatState();
}

int main()
{
    int bad = 0;
    CWorld world; std::memset(&world, 0, sizeof world); world.PCombatManager = (CCombatManager*)&world;
    g_world = &world;
    CThing* thing = (CThing*)&bad;
    static unsigned char raw[sizeof(CTCCombat)];
    std::memset(raw, 0xcd, sizeof raw);
    CTCCombat* c = new (raw) CTCCombat(thing);
    if (c->ParentThing != thing || c->CombatManager != world.PCombatManager || c->ParentCreature != 0 || c->PCombatTypeDef.Object != 0) bad |= 1;
    if (c->AbilityList._M_start || c->AbilityList._M_finish || c->AbilityList._M_end_of_storage.m_data || c->UsableAbilityList._M_start
        || c->AlwaysBlockThings._M_finish || c->FlourishParticles._M_end_of_storage.m_data) bad |= 2;
    if (c->CurrentAbilityIndex != -1 || c->PBlockCounterAbility.Object || c->PFlourish360Ability.Object || c->UnsheathingWeaponType
        || c->ShotOverrideType || c->CurFlourishTimer || c->IsInMiddleOfFlourishAction) bad |= 4;
    SRbTreeNode* h = c->CombatAbilities.m_pHeader;
    if (!h || h->m_color != 0 || h->m_pParent != 0 || h->m_pLeft != h || h->m_pRight != h || c->CombatAbilities.m_nodeCount != 0) bad |= 8;
    if (g_resets != 1 || g_resetSawIndex != -1 || g_resetSawFlag) bad |= 16;
    if (((unsigned char*)&c->BlockCounterTimer)[0] != 0xcd || ((unsigned char*)&c->Blocking)[0] != 0xcd) bad |= 32;   // left to the reset
    std::free(h);
    if (bad) { std::printf("00735540_TEST FAIL\n"); return 1; }
    std::printf("00735540_TEST PASS\n");
    return 0;
}
