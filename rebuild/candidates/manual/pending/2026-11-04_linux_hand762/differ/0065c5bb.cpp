// CTCVillage::UpdateMirroredCrimeData() -- mirror the village's crime records into the hero-facing copy, updating each entry against the main player's hero
// Grade: DIFFER(ours v retail) -- DIFFER dl=64: callee-saved assignment only: retail keeps this in ebx and spills both CSE'd component addresses (hero+0x20 / hero+0x44 to [ebp-0Ch]/[ebp-14h]), ours keeps hero/hero+0x44 in ebx and spills this; the out-param GetTC<T>(T**) over the real CVectorMap (TC 4 is CTCHeroStats, map OutstandingCrimes) gives the same bytes otherwise. Tried village alias, combined guard, crimes reference, inline header loop, deed-log placement, class form. Next: TU-mate CTCVillage rows with a shared CTCBase/CThing header (best try/0065c5bb_b.cpp)
// Leaf: ?UpdateMirroredCrimeData@CTCVillage@
// Flags: none
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
class CTCBaseComponent { public: virtual ~CTCBaseComponent(); };
typedef CVectorMap<ETCInterfaceType, CTCBaseComponent*, CKeyPairCompareLess<ETCInterfaceType, CTCBaseComponent*> > CTCMap;
class CThing { public:
    void* m_pVtable;
    unsigned char _pad04[0x20 - 4];
    std::bitset<274> UsingComponents;                       // +0x20
    CTCMap Components;                                      // +0x44
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    template<class T> __forceinline bool GetTC(T** out)
    {
        if (!HasTC(T::StatGetInterfaceType()))
            return false;
        ETCInterfaceType key = T::StatGetInterfaceType();
        CTCMap* map = &Components;
        std::pair<ETCInterfaceType, CTCBaseComponent*>* it = map->LowerBound(key);
        if (it == map->end() || T::StatGetInterfaceType() < it->first)
            it = map->end();
        *out = (T*)(it->second);
        return true;
    }
};
struct CPlayer { CThing* GetHeroThing(); };                 // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };        // 0x0049D9D0
struct CMainGameComponent { unsigned char _pad00[0x24]; CPlayerManager* PlayerManager; };
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0

struct CRegion { unsigned char _pad00[0x18]; CCharString Name; };
struct CWorldMap {
    long GetCurrentRegion();                                // 0x004FB150
    CRegion* GetRegion(long region);                        // 0x004FC180
};
struct SVillageCrimeData {                                  // 0x44 bytes
    long CrimeCounts[10];                                   // +0x00
    long FramePlayerLastSeenByGuard;                        // +0x28
    long Other[6];
};
struct CCrimeDataMap {
    SVillageCrimeData& operator[](const CCharString& region);   // 0x005812DF
};
struct CTCHeroStats {                                       // the hero's TC 4 (TCI_HERO_STATS)
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HERO_STATS; }
    unsigned char _pad00[0x0c];
    CCrimeDataMap OutstandingCrimes;                        // +0x0c (Ego_r.pdb name)
};
struct CDeed { long Type; };
struct CDeedDef {
    virtual void v00();
    virtual void DeleteThis();                              // slot 1
    long RefCount;                                          // +0x04
    unsigned char _pad08[0x7c - 0x08];
    unsigned char Flags;                                    // +0x7c
    void Release() { if (--RefCount == 0) DeleteThis(); }
};
struct CDeedDefPointer {
    CDeedDef* Object;
    __forceinline ~CDeedDefPointer() { if (Object) { Object->Release(); Object = 0; } }
    CDeedDef* operator->() const { return Object; }
};
struct CTCHeroOpinionDeedLog {                              // the hero's TC 0x51
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HERO_OPINION_DEED_LOG; }
    CDeed* GetDeedFromHandle(long handle);                  // 0x006C58F0
    CDeedDefPointer GetDeedDef(long type);                  // 0x006C5630
};
struct CLongSetNode {
    long  Color;
    CLongSetNode* Parent;
    CLongSetNode* Left;
    CLongSetNode* Right;
    long  Value;                                            // +0x10
};
CLongSetNode* IncrementNode(CLongSetNode* node);            // 0x004292C0
struct CLongSet {
    CLongSetNode* Header;
    long  NodeCount;
    long  Pad;
};
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    CWorldMap* GetWorldMap() const;                         // 0x00686D20
};
struct CTCVillage : CTCBase {
    unsigned char _pad08[0x64 - 0x08];
    CLongSet PlayersCrimes;                                 // +0x64
    unsigned char _pad70[0x80 - 0x70];
    long FramePlayerLastSeenByGuard;                        // +0x80
    void UpdateMirroredCrimeData();
};

void CTCVillage::UpdateMirroredCrimeData()
{
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
    if (hero == 0)
        return;
    CTCHeroStats* heroStats;
    if (!hero->GetTC(&heroStats))
        return;
    CCharString region = GetWorldMap()->GetRegion(GetWorldMap()->GetCurrentRegion())->Name;
    SVillageCrimeData data = heroStats->OutstandingCrimes[region];
    data.FramePlayerLastSeenByGuard = FramePlayerLastSeenByGuard;
    for (long i = 0; i < 10; ++i)
        data.CrimeCounts[i] = 0;
    CTCHeroOpinionDeedLog* deedLog;
    hero->GetTC(&deedLog);
    CLongSetNode* end = PlayersCrimes.Header;
    for (CLongSetNode* node = end->Left; node != end; node = IncrementNode(node)) {
        CDeed* deed = deedLog->GetDeedFromHandle(node->Value);
        if (deed && deed->Type < 10) {
            if (!(deedLog->GetDeedDef(deed->Type)->Flags & 0x20) || data.CrimeCounts[deed->Type] == 0)
                ++data.CrimeCounts[deed->Type];
        }
    }
    heroStats->OutstandingCrimes[region] = data;
}
