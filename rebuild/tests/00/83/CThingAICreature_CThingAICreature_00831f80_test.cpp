#include <cstdio>
#include <cstdlib>
#include <new>
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
// ---- test doubles ----

static long g_baseType = -1; static int g_ipCtors = 0, g_strCtors = 0;
void CThingCreatureBase::v00() {}
CThingCreatureBase::CThingCreatureBase(long type) { g_baseType = type; }
void CBaseIntelligentPointer::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { ++g_ipCtors; }
void CBuildingPointer::v00() {}
void CAICreaturePointer::v00() {}
CCharString::CCharString() : PData(0) { ++g_strCtors; }
void CThingAICreature::v00() {}
int main()
{
    void* mem = std::malloc(sizeof(CThingAICreature));
    unsigned char* raw = (unsigned char*)mem;
    for (unsigned int i = 0; i < sizeof(CThingAICreature); ++i) raw[i] = 0xff;     // dirty memory: every field must be set
    CThingAICreature* c = new (mem) CThingAICreature();
    bool ok = g_baseType == 1 && g_ipCtors == 5 && g_strCtors == 1
        && c->RelationshipsInit.Data == 0 && c->RelationshipsInit.Info == 0
        && c->PCurrentBrainOutput.Data == 0 && c->PBrain.Info == 0
        && c->DebugRenderHandles._M_node != 0 && c->DebugRenderHandles._M_node->_M_next == c->DebugRenderHandles._M_node
        && c->DebugRenderHandles._M_node->_M_prev == c->DebugRenderHandles._M_node
        && c->MoodSprite == 0 && c->MoodSpriteCounter == 0 && c->WFTimeLastDamaged == 0.0f && c->VillageTaskPerforming == 0
        && c->MaxWalkingSpeed == 0.0f && c->MaxFlyingSpeed == 0.0f
        && !c->PerformingVillageTaskFlag && !c->AISuspended && !c->HasInformation && !c->EnableCreatureAutoPlacing
        && !c->AllowedToFollowHero && !c->RegionFollowingOverriddenFromScript && c->RespondingToFollowAndWait
        && !c->CanBeCourted && !c->CanBeMarried
        && c->Suspenders._M_node_count == 0 && c->Suspenders._M_header->_M_color == 0 && c->Suspenders._M_header->_M_parent == 0
        && c->Suspenders._M_header->_M_left == c->Suspenders._M_header && c->Suspenders._M_header->_M_right == c->Suspenders._M_header;
    if (!ok) { std::printf("00831f80_TEST FAIL\n"); return 1; }
    std::printf("00831f80_TEST PASS\n");
    return 0;
}