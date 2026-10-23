// Grade: DIFFER(641v631) -- hero/map CSE register alloc (ebp=&map, hero spilled)
// CTCTavernGame::GivePrize() -- give the hero the tavern game's prize object and renown, tell every creature near the table about it, and mark the prize given
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EB590, 640 bytes (+ int3 padding), plain `ret`.
// Callees: CDefClassBase::GetSubDef<CTavernGameDef> 0x005ED4A0 (the inlined CTCTavernGame::GetDef 0x005EA000);
//   CPlayerManager::GetMainPlayer() const 0x00449960; CPlayer::GetHeroThing() 0x00487DC0;
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020; CTCBase::GetPlayerManager 0x00686D50 /
//   CPlayerManager main player number 0x00449700; CThing::GetPosition() const 0x004C73D0; CCharString 0x0099EBF0 /
//   ~CCharString 0x0099EAE0; CThingObject::Create 0x00703210 (__fastcall); CTCHeroReceiveItems::AddItemToInventory
//   0x007CAB30; CTCTavernGame::GetHeroStats 0x005EB4B0; CTCHeroStats::AddRenown 0x0057D8BE; malloc 0x00BFEA0E (list
//   header); CBaseIntelligentPointer::GetPItem 0x00A01B50; CTCPhysicsBase::GetBoundingInfo 0x006AF640;
//   CTCBase::GetThingSearchTools 0x00686D90; CThingSearchTools::GetAllThings<CThingAICreature, list, sphere filter>
//   0x005ED7D0; CTCHeroOpinionDeedLog::PostDeed 0x006C6990; ~list 0x004AB4F0 (out of line).
// Layout: retail CTCTavernGame HasPrizeBeenGiven +0xe9 (Ego_r.pdb +0x105 less 0x1c: retail STLport vectors are 12 bytes
//   and the CTCBase head is 8 bytes smaller; BetIncreasing +0xe8 as the landed ChangeBet 0x005EB810); CTavernGameDef
//   PrizeObjectDefIndex +0x180, PrizeRenown +0x184.
struct CTCBase;
struct CFaction;
struct CThingAICreature;
struct CRightHandedSet;

extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

struct C3DVector {
    float X, Y, Z;
};

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CTavernGameDef : CDefBase {
    unsigned char _pad08[0x180 - 0x08];
    long PrizeObjectDefIndex;                           // +0x180
    long PrizeRenown;                                   // +0x184
};
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    CTavernGameDef* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer* out);                   // 0x005ED4A0
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                                // +0x70
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const;               // 0x004C73D0
};

struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(float ux, float uy, float uz, float fx, float fy, float fz)
    {
        Up.X = ux; Up.Y = uy; Up.Z = uz;
        Forward.X = fx; Forward.Y = fy; Forward.Z = fz;
    }
};

struct CThingObject {
    static CThingObject* __fastcall Create(long defIndex, const C3DVector* position, long player,
                                           const CRightHandedSet* orientation, unsigned long flags,
                                           const CCharString& name);   // 0x00703210
};

struct CTCHeroReceiveItems {                            // interface 0xcb
    void AddItemToInventory(CThingObject* item, bool flag);   // 0x007CAB30
};
struct CTCHeroStats {
    void AddRenown(long renown);                        // 0x0057D8BE
};
enum EOpinionDeedType { OPINION_DEED_TYPE_0x4C = 0x4c };
enum EOpinionPostDeedType { OPINION_POST_DEED_TYPE_1 = 1 };
struct CTCHeroOpinionDeedLog {                          // interface 0x51
    long PostDeed(EOpinionDeedType deedType, const C3DVector& position, const CThing* target, CThingAICreature* victim,
                  EOpinionPostDeedType postType, bool b1, bool b2, const CFaction* faction, bool b3, long extra);   // 0x006C6990
};

struct CPhysicsBody {
    void GetBoundingInfo(C3DVector& centre, float& radius);   // 0x006AF640
};
struct CThingWithBody {
    unsigned char _pad0[0x60];
    CPhysicsBody* Body;                                 // +0x60
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    void* _vfptr;
    void* PData;
    CThingWithBody* GetPItem() const;                   // 0x00A01B50
};
struct CTCPhysicsBase {                                 // interface 0x33
    unsigned char _pad0[0xd];
    CBaseIntelligentPointer Thing;                      // +0x0d
};
#pragma pack(pop)

struct SCreatureNode { SCreatureNode* _M_next; SCreatureNode* _M_prev; CThingAICreature* _M_data; };
struct CCreatureList {                                  // std::list<CThingAICreature*>
    SCreatureNode* _M_node;
    CCreatureList()
    {
        SCreatureNode* node = (SCreatureNode*)malloc(sizeof(SCreatureNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    ~CCreatureList();                                   // 0x004AB4F0
};

struct CThingFilter_InSphere {
    C3DVector Centre;
    float Radius;
    CThingFilter_InSphere(const C3DVector& centre, float radius) : Centre(centre), Radius(radius) { }
};
struct CThingSearchTools {
    void GetAllThings(const CThingFilter_InSphere& filter, CCreatureList& found);   // 0x005ED7D0
};

struct CPlayer { CThing* GetHeroThing(); };             // 0x00487DC0
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;                     // 0x00449960
    long GetMainPlayerNumber() const;                   // 0x00449700
};
struct CMainGameComponent {
    unsigned char _pad0[0x1c];
    CPlayerManager* PlayerManager;                      // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;       // 0x013B86A0

struct CTCTavernGame {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0xe9 - 0x08];
    bool HasPrizeBeenGiven;                             // +0xe9
    CPlayerManager* GetPlayerManager() const;           // 0x00686D50 (CTCBase)
    CThingSearchTools* GetThingSearchTools() const;     // 0x00686D90 (CTCBase)
    void GetDef(CDefPointer* out) { ParentThing->PDef->GetSubDef(out); }   // 0x005EA000
    void GetHeroStats(CTCHeroStats** stats);            // 0x005EB4B0
    void GivePrize();
};

void CTCTavernGame::GivePrize()
{
    CDefPointer def;
    GetDef(&def);
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
    CTCHeroReceiveItems* receive;
    if (hero->GetInterface(&receive, 0xcb) && receive) {
        long player = GetPlayerManager()->GetMainPlayerNumber();
        C3DVector position = ParentThing->GetPosition();
        CRightHandedSet orientation(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
        CThingObject* prize = CThingObject::Create(def->PrizeObjectDefIndex, &position, player, &orientation, 0,
                                                   CCharString(""));
        receive->AddItemToInventory(prize, false);
        CTCHeroStats* stats;
        GetHeroStats(&stats);
        stats->AddRenown(def->PrizeRenown);
        CTCHeroOpinionDeedLog* log;
        if (hero->GetInterface(&log, 0x51)) {
            CCreatureList creatures;
            CTCPhysicsBase* physics;
            ParentThing->GetInterface(&physics, 0x33);
            C3DVector centre;
            float radius;
            physics->Thing.GetPItem()->Body->GetBoundingInfo(centre, radius);
            GetThingSearchTools()->GetAllThings(CThingFilter_InSphere(centre, radius), creatures);
            for (SCreatureNode* node = creatures._M_node->_M_next; node != creatures._M_node; node = node->_M_next)
                log->PostDeed(OPINION_DEED_TYPE_0x4C, hero->GetPosition(), 0, node->_M_data, OPINION_POST_DEED_TYPE_1,
                              false, false, 0, false, 0);
        }
    }
    HasPrizeBeenGiven = true;
}
