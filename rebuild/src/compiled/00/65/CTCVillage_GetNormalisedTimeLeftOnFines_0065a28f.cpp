// CTCVillage::GetNormalisedTimeLeftOnFines() -- for the village region's fines: the longest time left, in frames, before a guard forgets one of the hero's recorded crimes, as a fraction of that crime's knowledge life (0 when the region has no fines)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065A28F, length 397 bytes (`leave`, plain `ret`): frame-pointer size TU,
//   `#pragma optimize("y", off)` + `#pragma optimize("s", on)` as the landed sibling GetRegionLeavingDurationForCurrentCrimes
//   0x0065A5BB.
// Callees: the CTCBase global-manager getters 0x00686D30 (+0x28 of 0x013B8A1C, the world map), 0x00686D60 (+0x1C, the
//   definition manager) and 0x00686A70 (+0x24, the world) -- all propagated "CViewBase::GetDisplayEngine"; CThing::
//   GetPos 0x004C73D0; the world map's slot 16 (+0x40, the map index at a position); GetRegionNumberMapIsIn
//   0x004FC190 and the region-by-number getter 0x004FB490 (propagated "CTCInventoryBase::GetCategoryWithIndex");
//   CWorld::GetCurrentPlayer 0x0049D9D0; CPlayer::GetPControlledCreature 0x00487DC0; std::bitset<>::test 0x00410DE0;
//   CVectorMap LowerBound 0x0040F020; CTCHeroOpinionDeedLog::GetDeedFromHandle 0x006C58F0 / GetDeedEffectsDef
//   0x006C5630; STLport tree Successor 0x004292C0; the current-frame function 0x0049D870; g_framesPerSecond 0x01375550.
// Layout: retail CTCVillage PlayersCrimes +0x64 and FramePlayerLastSeenByGuard +0x80 (PDB +0xDC/+0x10C less the 4-byte
//   STLport lists and the 12-byte set); the region +0x24 CDefPointer whose def +0x9D bool enables fines (descriptive);
//   COpinionDeedEffectsDef KnowledgeLifeInSeconds +0x3c (as the sibling).
// Shape: a named `CRegion* region` local, then `CDefPointer def = region->PDef` (raw-object ctor: null-tested add-ref)
//   and `if (def.Object && def->HasFines)` (a null def returns 0 unreleased; the region def releases with `--count == 0`,
//   the deed effects def with DecRef/GetRefCount and a cleared pointer); the per-crime frame-rate and current-frame reads
//   go through a manager getter whose result is discarded (static members reached through `GetX()->`); the time left is
//   `knowledge - GetWorld()->GetFramesSince(FramePlayerLastSeenByGuard)` (an inline static, so the getter call precedes
//   the field load); the effects def is a named local released after the comparison.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;
class C3DVector { public: float x, y, z; };

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return it->second;
    }
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class CThingPlayerCreature : public CThing { };

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CWorld { public:
    CPlayer* GetCurrentPlayer() const;          // 0x0049D9D0
    static long __fastcall GetFrame();   // 0x0049D870
    static long GetFramesSince(long frame) { return GetFrame() - frame; }
};
class CMainGameComponent { public:
    unsigned char _pad[0x24];
    CWorld*       World;                        // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
extern long g_framesPerSecond;                  // 0x01375550
class CDefinitionManager { public:
    static long& FramesPerSecond() { return g_framesPerSecond; }
};

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
};
class COpinionDeedEffectsDef : public CDefBase { public:
    unsigned char _pad_0x08[0x34];
    long KnowledgeLifeInSeconds;                // +0x3c
    __forceinline void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
class CRegionDef : public CDefBase { public:
    unsigned char _pad_0x08[0x95];
    bool HasFines;                              // +0x9d (descriptive)
    __forceinline void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer(T* object) : Object(object)
    {
        if (Object)
            ++Object->m_count;
    }
    __forceinline ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
};
class COpinionDeed { public:
    long DeedType;
};
class CTCHeroOpinionDeedLog { public:
    COpinionDeed* GetDeedFromHandle(long handle);   // 0x006C58F0
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long deedType);   // 0x006C5630
};

class CRegion { public:
    unsigned char           _pad00[0x24];
    CRegionDef*   PDef;                         // +0x24 CDefPointer<const CRegionDef>::Object
};
class CWorldMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapAtPos(const C3DVector& pos);     // slot 16 (+0x40)
    long GetRegionNumberMapIsIn(long map);              // 0x004FC190
    CRegion* GetRegionWithIndex(long region);           // 0x004FB490
};

struct SNode {
    long   _M_color;
    SNode* _M_parent;
    SNode* _M_left;
    SNode* _M_right;
    long   _M_value_field;                      // +0x10
};
SNode* __cdecl Successor(SNode* node);          // 0x004292C0

class CTCVillage { public:
    void*         __vftable;                    // +0x00
    CThing*       Owner;                        // +0x04
    unsigned char _pad_0x08[0x5c];
    SNode*        PlayersCrimes;                // +0x64 set<long> header node
    unsigned char _pad_0x68[0x18];
    long          FramePlayerLastSeenByGuard;   // +0x80
    CWorldMap*          GetWorldMap();          // 0x00686D30
    CDefinitionManager* GetDefinitionManager(); // 0x00686D60
    CWorld*             GetWorld();             // 0x00686A70
    float GetNormalisedTimeLeftOnFines();
};

float CTCVillage::GetNormalisedTimeLeftOnFines()
{
    CRegion* region = GetWorldMap()->GetRegionWithIndex(GetWorldMap()->GetRegionNumberMapIsIn(GetWorldMap()->GetMapAtPos(Owner->GetPos())));
    CDefPointer<CRegionDef> def = region->PDef;
    if (def.Object && def->HasFines) {
        long longest = 0;
        long life = 1;
        CThing* hero = g_pMainGameComponent->World->GetCurrentPlayer()->GetPControlledCreature();
        if (hero) {
            CTCHeroOpinionDeedLog* log;
            if (hero->HasTC(0x51))
                log = (CTCHeroOpinionDeedLog*)hero->GetTC(0x51);
            SNode* end = PlayersCrimes;
            for (SNode* it = end->_M_left; it != end; it = Successor(it)) {
                COpinionDeed* deed = log->GetDeedFromHandle(it->_M_value_field);
                if (deed) {
                    CDefPointer<COpinionDeedEffectsDef> effects = log->GetDeedEffectsDef(deed->DeedType);
                    long knowledge = effects->KnowledgeLifeInSeconds * GetDefinitionManager()->FramesPerSecond();
                    long left = knowledge - GetWorld()->GetFramesSince(FramePlayerLastSeenByGuard);
                    if (left >= longest) {
                        life = knowledge;
                        longest = left;
                    }
                }
            }
        }
        return (float)longest / life;
    }
    return 0.0f;
}