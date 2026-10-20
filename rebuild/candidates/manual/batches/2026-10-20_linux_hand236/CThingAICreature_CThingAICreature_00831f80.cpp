// CThingAICreature::CThingAICreature() -- construct the AI creature: creature base of thing type 1, null relationship/building/family pointers, empty brain name, debug list and suspender set, cleared speeds and flag bits
// Leaf: ??0CThingAICreature
// Retail: Fable.exe (TLC, VC7.1) @ 0x00831F80, 319 bytes (+1 int3), `ret`.
// Callees: CThingCreatureBase::CThingCreatureBase(long type) @ 0x00666090 (landed), CBaseIntelligentPointer ctor
//   @ 0x00A01B10 (x5), CCharString::CCharString() @ 0x0099E4B0, malloc @ 0x00BFEA0E (list header node 0x10, set header
//   node 0x14; one deferred `add esp,8`).
// Vtables: CThingAICreature 0x0127293C; CIntelligentPointer<CThingBuilding> 0x01244344 (x2); CIntelligentPointer
//   <CThingAICreature> 0x0123E7E0 (x3).
// Layout: retail CThingCreatureBase is 0x160 (the landed base ctor), so the PDB CThingAICreature members sit 0x28 low:
//   RelationshipsInit +0x160, HomeBuilding +0x168, WorkBuilding +0x170, Father/Mother/SpouseCreature +0x178/+0x180/+0x188,
//   OverridingBrainName +0x190, DebugRenderHandles +0x194 (4-byte STLport list), MoodSprite +0x198, MoodSpriteCounter
//   +0x19C, PCurrentBrainOutput +0x1A0, PBrain +0x1A8, WFTimeLastDamaged +0x1B0, VillageTaskPerforming +0x1B4, the four
//   speeds +0x1B8..+0x1C4, flag bits +0x1C8/+0x1C9, Suspenders +0x1CC (STLport set: header node pointer + count).
//   engine/CThingAICreature.h is a flat PDB-offset struct, so a local model is used.
// Shape: init list in declaration order through one zero register (ebx); the list head is zeroed, then given its
//   self-linked header node; the set is the inlined _Rb_tree ctor (header zeroed, node malloc'd, count 0, red colour,
//   null parent, leftmost/rightmost re-read through the member); the flag bits are mem-initializers in declaration order
//   (the landed CThing ctor 0x004C9030 idiom), so they are set before the set's malloc: the first byte folds to one store,
//   the second to `and 0E4h ; or 4` with its load hoisted above the speed stores (body assignments land after the set).
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

struct CThingCreatureBase {
    virtual void v00();
    unsigned char _body[0x15c];
    CThingCreatureBase(long type);               // 0x00666090
};
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};
struct CBuildingPointer : CBaseIntelligentPointer {          // CIntelligentPointer<CThingBuilding>, vtable 0x01244344
    virtual void v00();
    CBuildingPointer() { }
};
struct CAICreaturePointer : CBaseIntelligentPointer {        // CIntelligentPointer<CThingAICreature>, vtable 0x0123E7E0
    virtual void v00();
    CAICreaturePointer() { }
};
struct CCPPointerInfo;
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
struct CCharStringData;
struct CCharString {
    CCharStringData* PData;
    CCharString();                              // 0x0099E4B0
};
struct CEnginePrimitiveHandle { unsigned long m_words[2]; };
struct SHandleListNode { SHandleListNode* _M_next; SHandleListNode* _M_prev; CEnginePrimitiveHandle _M_data; };
struct CHandleList {                            // list<CEnginePrimitiveHandle>: the header-node pointer
    SHandleListNode* _M_node;
    CHandleList() : _M_node(0)
    {
        SHandleListNode* node = (SHandleListNode*)malloc(sizeof(SHandleListNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};
struct _Rb_tree_node_base {
    char                 _M_color;              // +0
    _Rb_tree_node_base*  _M_parent;             // +4
    _Rb_tree_node_base*  _M_left;               // +8
    _Rb_tree_node_base*  _M_right;              // +0xc
};
struct SSuspenderNode : _Rb_tree_node_base { void* _M_value_field; };   // 0x14 bytes
struct CSuspenderSet {                          // set<void*>
    SSuspenderNode* _M_header;
    unsigned long   _M_node_count;
    CSuspenderSet() : _M_header(0)
    {
        _M_header = (SSuspenderNode*)malloc(sizeof(SSuspenderNode));
        _M_node_count = 0;
        _M_header->_M_color = 0;                // red
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};
struct CRelationshipsInit;
struct CBrainOutput;
struct CAIBrain;
struct CVillageTask;

struct CThingAICreature : CThingCreatureBase {  // vtable 0x0127293C
    CCountedPointer<CRelationshipsInit> RelationshipsInit;  // +0x160
    CBuildingPointer   HomeBuilding;            // +0x168
    CBuildingPointer   WorkBuilding;            // +0x170
    CAICreaturePointer FatherCreature;          // +0x178
    CAICreaturePointer MotherCreature;          // +0x180
    CAICreaturePointer SpouseCreature;          // +0x188
    CCharString        OverridingBrainName;     // +0x190
    CHandleList        DebugRenderHandles;      // +0x194
    unsigned long      MoodSprite;              // +0x198
    unsigned long      MoodSpriteCounter;       // +0x19c
    CCountedPointer<CBrainOutput> PCurrentBrainOutput;      // +0x1a0
    CCountedPointer<CAIBrain>     PBrain;                   // +0x1a8
    float              WFTimeLastDamaged;       // +0x1b0
    CVillageTask*      VillageTaskPerforming;   // +0x1b4
    float              MaxWalkingSpeed;         // +0x1b8
    float              MaxJoggingSpeed;         // +0x1bc
    float              MaxRunningSpeed;         // +0x1c0
    float              MaxFlyingSpeed;          // +0x1c4
    bool PerformingVillageTaskFlag : 1;         // +0x1c8
    bool AISuspended : 1;
    bool HasInformation : 1;
    bool WanderWithInformation : 1;
    bool WaveWithInformation : 1;
    bool ContinueAIWithInformation : 1;
    bool ImmuneToAttack : 1;
    bool EnableCreatureAutoPlacing : 1;
    bool AllowedToFollowHero : 1;               // +0x1c9
    bool RegionFollowingOverriddenFromScript : 1;
    bool RespondingToFollowAndWait : 1;
    bool CanBeCourted : 1;
    bool CanBeMarried : 1;
    CSuspenderSet      Suspenders;              // +0x1cc
    virtual void v00();
    CThingAICreature();
};

CThingAICreature::CThingAICreature()
    : CThingCreatureBase(1)
    , MoodSprite(0)
    , MoodSpriteCounter(0)
    , WFTimeLastDamaged(0)
    , VillageTaskPerforming(0)
    , MaxWalkingSpeed(0)
    , MaxJoggingSpeed(0)
    , MaxRunningSpeed(0)
    , MaxFlyingSpeed(0)
    , PerformingVillageTaskFlag(false)
    , AISuspended(false)
    , HasInformation(false)
    , WanderWithInformation(false)
    , WaveWithInformation(false)
    , ContinueAIWithInformation(false)
    , ImmuneToAttack(false)
    , EnableCreatureAutoPlacing(false)
    , AllowedToFollowHero(false)
    , RegionFollowingOverriddenFromScript(false)
    , RespondingToFollowAndWait(true)
    , CanBeCourted(false)
    , CanBeMarried(false)
{
}
