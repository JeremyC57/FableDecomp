// CTCCombat::CTCCombat(CThing* thing) -- CTCBase(thing), then the combat manager from the world, empty ability lists / blocker list / ability multimap / flourish particles, no current ability (-1), and the state reset
// Leaf: ??0CTCCombat
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00735540, 170 bytes (`ret 4`). Vtable 0x0126724C.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CTCBase::GetWorld() @ 0x00686A70 (the world's combat manager at +0x4c),
// malloc @ 0x00BFEA0E (the 0x24-byte multimap header node), CTCCombat's state reset @ 0x007334F0 (manifest
// "CTCCombat_ResetState"; it sets the members this ctor leaves alone).
// Layout: local model -- engine/CTCCombat.h is Ego_r.pdb (16-byte containers); retail containers are 12 bytes, so from
// UsableAbilityList on everything sits 4 bytes lower per container: AbilityList +0x2c, UsableAbilityList +0x38,
// CurrentAbilityIndex +0x44, PBlockCounterAbility +0x48, PFlourish360Ability +0x4c, AlwaysBlockThings +0x54,
// UnsheathingWeaponType +0x6c, CombatAbilities +0x70, ShotOverrideType +0x7c, CurFlourishTimer +0x84, FlourishParticles +0x88,
// IsInMiddleOfFlourishAction +0x94.
// Shape: the init list in declaration order with one zero register; vectors are the STLport _Vector_base (start, finish and
// an alloc proxy over an allocator temporary); the multimap's tree base nulls then allocates its header, the tree sets its
// count and empty-initialises the header (re-reading it after every store); the malloc's `add esp,4` lands after the
// following member ctors and before the body's byte store, then the reset call.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
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
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CTCBase(CThing* thing);                             // 0x00686800
    CWorld* GetWorld();                                 // 0x00686A70
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
    virtual void v00();
    CTCCombat(CThing* thing);
    void ResetCombatState();                            // 0x007334F0
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