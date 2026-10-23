// CTCFishing::CreateFish(bool firstFish) -- pick a fish def for the fishing spot (a random one whose level equals the hero's fishing level -- 1 for the first fish -- else a random lower-level one), create it in limbo at the owner's position, scale its size/weight by the level difference and its weight by the spot, environment, sea-level and region multipliers, and record the rounded weight
// Grade: DIFFER(1207v1145) -- first drafts: control flow, callees and x87 chains as retail; residue is register allocation (ours keeps a zero register in ebx; retail caches the same-level vector end in edi, write-through around the inline push_back), the def release forms (retail sub-def release `dec [ecx+4]` v ours load/dec/store) and duplicated failure epilogues (retail shares one free-both-vectors exit)
// Leaf: ?CreateFish@CTCFishing
//
// Retail 0x007B1A30, 1152 bytes (FPO `sub esp,28h`, `ret 4`).
// Callees: CVectorMap::LowerBound 0x0040F020, CTCBase::GetDefinitions 0x006869D0, CDefinitionManager::GetPDefFromGlobalIndex
//   0x009AD9E0, CDefClassBase::GetSubDef<CFishDef> 0x007B2600, vector<long> InsertGrow 0x00440990, the lower-level list's
//   push_back 0x00485FF3, CTCBase::GetRandomSeed 0x00686DA0, GFROR13 0x00497890, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   CThing::GetPosition 0x004C73D0, the thing factory Create 0x00703210 (__fastcall), CBaseIntelligentPointer ctor 0x00A01B10 /
//   SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / ~ 0x00A01C10, CThing::SetInLimbo 0x004C8CF0, CTCBase::GetWorldMap 0x00686D20,
//   the environment getter 0x004FB150, CWorldMap::GetRegion 0x004FC180, CTCBase::GetWorld 0x00686A70,
//   CWorld::GetEnvironmentFishWeightMult 0x0049F2D0, the map's three sea-level multipliers 0x006B2BD0/0x006B2BE0/0x006B2BF0,
//   __ftol2 0x00BFEA70, free 0x00BFEA14.
// Layout: retail CTCFishing: ParentThing +0x04, Fish CIntelligentPointer +0x0C, FishWeight +0x5C, the spot def +0x60 (fish
//   def indices vector +0x28, sea-level weight factors +0x58/+0x5C/+0x60); hero fishing TC (interface 0x68) level +0x0C; fish
//   def level +0x50; fish TC (interface 0xDC) def +0x0C, size +0x10, weight +0x14; region def +0x24 of the region, its fish
//   weight multiplier +0xC0; CThing player byte +0x90.
#include <new>
extern "C" void __cdecl free(void*);

struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x90 - 0x4c];
    signed char    Player;                              // +0x90
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void SetInLimbo(bool limbo);                        // 0x004C8CF0
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
CThing* __fastcall Create(long defIndex, const C3DVector& pos, long player, long a3, long a4, const CCharString& name);   // 0x00703210
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    unsigned long m_uid[2];
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    void* GetPItem() const;                             // 0x00A01B50
    void SetPItem(void* item);                          // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) { SetPItem(item); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct SScalarTag { SScalarTag() { } };
struct CLongVector {
    long* first;
    long* last;
    long* end;
    CLongVector() : first(0), last(0), end(0) {}
    ~CLongVector() { if (first) free(first); }
    unsigned long size() const { return last - first; }
    long& operator[](unsigned long i) { return *(first + i); }
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00440990
    void push_back(const long& value)
    {
        if (last != end) {
            new (last) long(value);
            ++last;
        } else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
struct CLongVectorB {
    long* first;
    long* last;
    long* end;
    CLongVectorB() : first(0), last(0), end(0) {}
    ~CLongVectorB() { if (first) free(first); }
    void push_back(const long& value);                  // 0x00485FF3
};
unsigned long __fastcall GFROR13(unsigned long value);  // 0x00497890
inline unsigned long GFRandom(unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed;
}

template<class T> struct CDefOut;
struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
    template<class T> bool GetSubDef(CDefOut<T>* out) const;   // 0x007B2600
};
template<class T> struct CDefOut {
    T* Object;
    CDefOut() : Object(0) {}
};
template<class T> struct CDefPtr {                      // by-value def handle: released and cleared
    T* Object;
    CDefPtr(const CDefPtr& other);
    ~CDefPtr() { if (Object) { Object->Release(); Object = 0; } }
    T* operator->() const { return Object; }
};
template<class T> struct CCountedPtr {                  // counted copy
    T* Object;
    CCountedPtr(const CCountedPtr& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CCountedPtr() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CFishDef : CDefClassBase {
    unsigned char _pad08[0x50 - 0x08];
    long Level;                                         // +0x50
};
struct CThingDef : CDefClassBase {};
struct CRegionDef : CDefClassBase {
    unsigned char _pad08[0xc0 - 0x08];
    float FishWeightMult;                               // +0xC0
};
struct CRegion {
    unsigned char _pad00[0x24];
    CCountedPtr<CRegionDef> Def;                        // +0x24
};
struct CWorldMap {
    long GetEnvironment();                              // 0x004FB150
    CRegion& GetRegion(long environment);               // 0x004FC180
};
struct CMap {
    float GetSeaLevelMult1();                           // 0x006B2BD0
    float GetSeaLevelMult2();                           // 0x006B2BE0
    float GetSeaLevelMult3();                           // 0x006B2BF0
};
struct CWorld {
    unsigned char _pad00[0x18];
    CMap* Map;                                          // +0x18
    float GetEnvironmentFishWeightMult(const C3DVector& pos);   // 0x0049F2D0
};
struct CDefinitionManager {
    CDefPtr<CThingDef> GetPDefFromGlobalIndex(long index);     // 0x009AD9E0
};
struct CTCFish {
    unsigned char _pad00[0x0c];
    CCountedPtr<CFishDef> Def;                          // +0x0C
    float Size;                                         // +0x10
    float Weight;                                       // +0x14
};
struct CTCHeroFishing {
    unsigned char _pad00[0x0c];
    long Level;                                         // +0x0C
};
struct CFishingSpotDef {
    unsigned char _pad00[0x28];
    CLongVector   Fish;                                 // +0x28
    unsigned char _pad34[0x58 - 0x34];
    float SeaLevelFactor1;                              // +0x58
    float SeaLevelFactor2;                              // +0x5C
    float SeaLevelFactor3;                              // +0x60
};

class CTCFishing {
public:
    virtual void v00();
    CThing*                     ParentThing;            // +0x04
    unsigned char               _pad08[0x4];
    CIntelligentPointer<CThing> Fish;                   // +0x0C
    unsigned char               _pad1c[0x5c - 0x1c];
    long                        FishWeight;             // +0x5C
    CFishingSpotDef*            SpotDef;                // +0x60
    CDefinitionManager* GetDefinitions();               // 0x006869D0
    unsigned long* GetRandomSeed();                     // 0x00686DA0
    CWorldMap* GetWorldMap();                           // 0x00686D20
    CWorld* GetWorld();                                 // 0x00686A70
    bool CreateFish(bool firstFish);
};

bool CTCFishing::CreateFish(bool firstFish)
{
    CThing* parent = ParentThing;
    if (parent->HasTC(0x68)) {
        CTCHeroFishing* fishing = parent->GetTC<CTCHeroFishing>(0x68);
        long level = firstFish ? 1 : fishing->Level;
        CLongVectorB lower;
        CLongVector same;
        for (unsigned long i = 0; i < SpotDef->Fish.size(); ++i) {
            CDefPtr<CThingDef> def = GetDefinitions()->GetPDefFromGlobalIndex(SpotDef->Fish[i]);
            CDefOut<CFishDef> fishDef;
            if (def->GetSubDef(&fishDef)) {
                if (fishDef.Object->Level == level)
                    same.push_back(SpotDef->Fish[i]);
                else if (fishDef.Object->Level < level)
                    lower.push_back(SpotDef->Fish[i]);
            }
            if (fishDef.Object && --fishDef.Object->RefCount == 0)
                fishDef.Object->Destroy();
        }
        long fishIndex;
        if (same.first != same.last) {
            unsigned long count = same.last - same.first;
            unsigned long random = GFRandom(GetRandomSeed());
            fishIndex = same.first[count ? random % count : 0];
        } else if (lower.first != lower.last) {
            unsigned long count = lower.last - lower.first;
            unsigned long random = GFRandom(GetRandomSeed());
            fishIndex = lower.first[count ? random % count : 0];
        } else
            return false;
        if (fishIndex <= 0)
            return false;
        Fish = CIntelligentPointer<CThing>(Create(fishIndex, ParentThing->GetPosition(), ParentThing->Player, 0, 0, ""));
        if (!Fish.GetPItem())
            return false;
        Fish.GetPItem()->SetInLimbo(true);
        CTCFish* fishTC = 0;
        CThing* fish = Fish.GetPItem();
        if (fish->HasTC(0xdc))
            fishTC = fish->GetTC<CTCFish>(0xdc);
        long levelDifference;
        {
            CCountedPtr<CFishDef> fishDef = fishTC->Def;
            levelDifference = level - fishDef->Level;
        }
        if (level > 0) {
            float scale = levelDifference * 0.2f + 1.0f;
            fishTC->Size *= scale;
            fishTC->Weight *= scale * scale * scale;
        }
        CCountedPtr<CRegionDef> regionDef = GetWorldMap()->GetRegion(GetWorldMap()->GetEnvironment()).Def;
        if (regionDef.Object) {
            float regionMult = regionDef->FishWeightMult;
            float environmentMult = GetWorld()->GetEnvironmentFishWeightMult(ParentThing->GetPosition());
            float seaMult1 = (SpotDef->SeaLevelFactor1 - 1.0f) * GetWorld()->Map->GetSeaLevelMult1() + 1.0f;
            float seaMult2 = (SpotDef->SeaLevelFactor2 - 1.0f) * GetWorld()->Map->GetSeaLevelMult2() + 1.0f;
            float seaMult3 = (SpotDef->SeaLevelFactor3 - 1.0f) * GetWorld()->Map->GetSeaLevelMult3() + 1.0f;
            fishTC->Weight = seaMult3 * fishTC->Weight * seaMult2 * seaMult1 * environmentMult * regionMult;
        }
        FishWeight = (long)fishTC->Weight;
        return true;
    }
    return false;
}
