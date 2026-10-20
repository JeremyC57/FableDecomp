// CTCEnemy::CTCEnemy(CThing* thing) -- the enemy/ally component: not friends with everything, followers' enemy proxy enabled, empty enemy and ally lists, then the faction named by the thing's CEnemyDef
// Grade: DIFFER(254v254) -- register swap only: retail keeps the thing def in ebp and the enemy def in edi, ours the reverse (same instructions, same order); tried holder/holder, raw/holder-in-block, raw/raw with a direct `if (GetSubDef(..))`
// Leaf: ??0CTCEnemy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076C8A0, 252 bytes (+ int3 padding), `ret 4`.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is propagated), malloc
//   @ 0x00BFEA0E (the two list header nodes), CTCBase::GetDefinitionManager @ 0x006869D0 (x2), CDefinitionManager::
//   GetDef<CThingDef>(long, CDefPointer*) @ 0x0042AF3C, CDefClassBase::GetSubDef<CEnemyDef>(CDefPointer*) @ 0x006C7720
//   (manifest "GetSubDef<CAICreatureWillPowerIndicatorDef>" is a byte-twin propagation), CDefinitionManager::
//   GetDefNameFromGlobalIndex(long) @ 0x009ACCC0 (CDefString by hidden pointer), CDefStringTable::GetString(long) @
//   0x009D49B0 on g_DefStringTable 0x013CA828, CTCBase::GetWorld const @ 0x00686A80, CFactionManager::GetPFaction(const
//   CCharString&) const @ 0x006B93D0, ~CCharString @ 0x0099EAE0, the defs' virtual slot 1 (+0x04, delete at zero).
// Layout = Ego_r.pdb CTCEnemy with the retail 4-byte STLport list heads: FriendsWithEverythingFlag +0x0c,
//   EnableFollowersEnemyProxy +0x0d, EnemyThings +0x10, AllyThings +0x14, PFaction +0x18; vtable 0x0126A47C;
//   CThing def global index (unsigned short) +0x8c; CEnemyDef (Ego_r.pdb 0x2c) Faction +0x28; CWorld faction manager +0x54.
// Shape: init list in declaration order through one zero register (each list head zeroed, then its 16-byte self-linked
//   header node); the body is the GetDef / GetSubDef pair of the landed CTCPerceiveThing::OnCreate 0x00744ED0 (GetDef
//   arguments pushed before the manager call, both out pointers with same-TU stand-in bodies so their frame words are
//   reused by the later CDefString / CCharString temporaries), each def taken into a register holder and released through
//   the pointee; SetFaction (landed out of line at 0x0076C810) is an inline member here.
struct CThing {
    unsigned char  _pad00[0x8c];
    unsigned short DefGlobalIndex;              // +0x8c
};
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

class CCharString {
public:
    void* PStringData;
    ~CCharString();                             // 0x0099EAE0
};
struct CDefStringTable {
    CCharString GetString(long index);          // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828
struct CDefString {
    long Index;
    CCharString GetString() const { return g_DefStringTable.GetString(Index); }
};

struct CDefPointeeBase {
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1 (+0x04)
    long m_refCount;                            // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {          // a counted def reference released at scope end
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
struct CEnemyDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 0x08];
    long Faction;                               // +0x28 (global def index of the faction def)
};
struct CThingDef : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CEnemyDef>* out);   // 0x006C7720
};
struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointer<CThingDef>* out);   // 0x0042AF3C
    CDefString GetDefNameFromGlobalIndex(long index);             // 0x009ACCC0
};

void* FindDef_CThingDef_GetSubDef();            // stand-in callees of the stand-in bodies below
void* FindDef_CDefinitionManager_GetDef();
// Stand-in bodies (the real ones are out of line in retail): they write the out slot only inline, so VC7.1 knows the
// def pointer's address does not escape the call (the landed 0x00848730 / 0x00744ED0 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CThingDef::GetSubDef(CDefPointer<CEnemyDef>* out)
{
    void* def = FindDef_CThingDef_GetSubDef();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CThingDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CFaction;
struct CFactionManager {
    CFaction* GetPFaction(const CCharString& name) const;   // 0x006B93D0
};
struct CWorld {
    unsigned char _pad00[0x54];
    CFactionManager* PFactionManager;           // +0x54
};

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CTCBase(CThing* thing);                     // 0x00686800
    CDefinitionManager* GetDefinitionManager(); // 0x006869D0
    CWorld* GetWorld() const;                   // 0x00686A80
};

struct SThingListNode { SThingListNode* _M_next; SThingListNode* _M_prev; void* _M_vptr; void* _M_item; };
struct CThingList {                             // list<CIntelligentPointer<CThing const> >: the header-node pointer
    SThingListNode* _M_node;
    CThingList() : _M_node(0)
    {
        SThingListNode* node = (SThingListNode*)malloc(sizeof(SThingListNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};

struct CTCEnemy : CTCBase {                     // vtable 0x0126A47C
    bool       FriendsWithEverythingFlag;       // +0x0c
    bool       EnableFollowersEnemyProxy;       // +0x0d
    CThingList EnemyThings;                     // +0x10
    CThingList AllyThings;                      // +0x14
    CFaction*  PFaction;                        // +0x18
    virtual void v00();
    void SetFaction(const CCharString& name)
    {
        CFactionManager* manager = GetWorld()->PFactionManager;
        PFaction = manager->GetPFaction(name);
    }
    CTCEnemy(CThing* thing);
};

CTCEnemy::CTCEnemy(CThing* thing)
    : CTCBase(thing)
    , FriendsWithEverythingFlag(false)
    , EnableFollowersEnemyProxy(true)
    , PFaction(0)
{
    CDefPointer<CThingDef> thingDefOut;
    GetDefinitionManager()->GetDef(ParentThing->DefGlobalIndex, &thingDefOut);
    CThingDef* thingDef = thingDefOut.Object;
    CDefPointer<CEnemyDef> enemyDefOut;
    if (thingDef->GetSubDef(&enemyDefOut)) {
        CEnemyDef* enemyDef = enemyDefOut.Object;
        SetFaction(GetDefinitionManager()->GetDefNameFromGlobalIndex(enemyDef->Faction).GetString());
    }
    CEnemyDef* enemyDef = enemyDefOut.Object;
    if (enemyDef && --enemyDef->m_refCount == 0)
        enemyDef->Delete();
    if (thingDef && --thingDef->m_refCount == 0)
        thingDef->Delete();
}
