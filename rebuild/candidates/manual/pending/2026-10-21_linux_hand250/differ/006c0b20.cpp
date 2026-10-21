// Grade: DIFFER(383v368) -- retail caches the result vector's start in ebx (iterator seed and the inline free on both exits, finish re-read each pass), zeroes the vector through eax and keeps the hero in ebp; ours re-reads the start for the free and spills differently
// CTCBuyableHouse::FindPosToCreateNewVillager(C3DVector* position) -- pick where a new villager for the house can appear unseen: the first quest-filtered marker (type 0x63) in the house's region the hero is not aware of, else the house itself when the hero is at least 20m away and unaware of it
// Leaf: FindPosToCreateNewVillager
// Retail 0x006C0B20, 368 bytes, `ret 4`.
// Callees: the TC's world-map accessor 0x00686D30, the world map's region lookup (slot 16, +0x40, const C3DVector&),
// CThing::GetPosition 0x004C73D0, the TC's thing-manager accessor 0x00686A70 (search tools +0x20),
// CThingSearchTools::GetAllThings<CThing, vector<const CThing*>, CFilter_Bind3<...>> 0x006C1DD0, the player-manager accessor
// 0x00686D40, CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetHeroThing 0x00487DC0, IsAwareOfPosition(
// CThingCreatureBase*, const C3DVector&) 0x00864620 (`__fastcall`), free 0x00BFEA14; 400.0f 0x0125D824.
// Layout: CTCBase ParentThing +4. The filter is three small objects (type 0x63, {map, region}, and a pair of pointers
// to them) built in the frame.
// Shape: the result vector is a zeroed local freed inline on both exits; the 2D distance test is `dx*dx + dy*dy < 400`.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
struct C3DVector { float X, Y, Z; };
struct CThing {
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
struct CThingCreatureBase : CThing { };
bool __fastcall IsAwareOfPosition(CThingCreatureBase* creature, const C3DVector& position);   // 0x00864620
struct CRegion;
struct CWorldMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual CRegion* GetRegionAt(const C3DVector& position);   // slot 16 (+0x40)
};
struct CThingFilter_Type {
    long Type;
    CThingFilter_Type(long type) : Type(type) { }
};
struct CThingFilter_InRegion {
    CWorldMap* Map;
    CRegion*   Region;
    CThingFilter_InRegion(CWorldMap* map, CRegion* region) : Map(map), Region(region) { }
};
struct CFilter_Bind2 {
    const CThingFilter_Type*     First;
    const CThingFilter_InRegion* Second;
    CFilter_Bind2(const CThingFilter_Type& first, const CThingFilter_InRegion& second) : First(&first), Second(&second) { }
};
struct CThingVector {
    const CThing** _M_start;
    const CThing** _M_finish;
    const CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingVector() { if (_M_start) free(_M_start); }
};
void PushThing(CThingVector& things, const CThing* thing);
bool MatchesFilter(const CFilter_Bind2& filter, const CThing* thing);
struct CThingSearchTools {
    const CThing** m_pAll;
    long           m_count;
    void GetAllThings(const CFilter_Bind2& filter, CThingVector& things);   // 0x006C1DD0
};
#pragma auto_inline(off)
// Same-TU stand-in for the GetAllThings instantiation (it sits next to this function in retail): VC7.1 then knows the
// result vector does not escape and keeps its start pointer in a register for the walk and the free.
void CThingSearchTools::GetAllThings(const CFilter_Bind2& filter, CThingVector& things)
{
    for (long i = 0; i < m_count; ++i)
        if (MatchesFilter(filter, m_pAll[i]))
            PushThing(things, m_pAll[i]);
}
#pragma auto_inline(on)
struct CThingManager {
    unsigned char      _pad00[0x20];
    CThingSearchTools* SearchTools;                     // +0x20
};
struct CPlayer {
    CThingCreatureBase* GetHeroThing();                 // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                           // 0x00449970
};
class CTCBuyableHouse {
public:
    void*   __vftable;                                  // +0x00
    CThing* ParentThing;                                // +0x04
    CWorldMap* GetWorldMap();                           // 0x00686D30
    CThingManager* GetThingManager();                   // 0x00686A70
    CPlayerManager* GetPlayerManager();                 // 0x00686D40
    bool FindPosToCreateNewVillager(C3DVector* position);
};

bool CTCBuyableHouse::FindPosToCreateNewVillager(C3DVector* position)
{
    CThingVector things;
    CRegion* region = GetWorldMap()->GetRegionAt(ParentThing->GetPosition());
    CThingFilter_InRegion inRegion(GetWorldMap(), region);
    CThingFilter_Type ofType(0x63);
    CFilter_Bind2 filter(ofType, inRegion);
    CThingManager* thingManager = GetThingManager();
    thingManager->SearchTools->GetAllThings(filter, things);
    CThingCreatureBase* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
    for (const CThing** it = things._M_start; it != things._M_finish; ++it) {
        if (!IsAwareOfPosition(hero, (*it)->GetPosition())) {
            *position = (*it)->GetPosition();
            return true;
        }
    }
    if (hero) {
        const C3DVector& house = ParentThing->GetPosition();
        const C3DVector& heroPos = hero->GetPosition();
        float dx = heroPos.X - house.X;
        float dy = heroPos.Y - house.Y;
        if (dx * dx + dy * dy < 400.0f)
            return false;
    }
    if (IsAwareOfPosition(hero, ParentThing->GetPosition()))
        return false;
    *position = ParentThing->GetPosition();
    return true;
}
