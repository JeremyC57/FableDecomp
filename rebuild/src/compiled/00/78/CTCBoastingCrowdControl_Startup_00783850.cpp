// CTCBoastingCrowdControl::Startup() -- when boasting is enabled and the quest manager holds boasts: find the boasting podium thing (interface 0x72) in the hero's region, the "MK_BoastingCrowd" marker and the nearest village, take the renown-level crowd size (halved once while suppressed), spawn that many crowd members and mark the control active if any spawned
// Leaf: ?Startup@CTCBoastingCrowdControl@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00783850, 443 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb ?Startup@CTCBoastingCrowdControl@@QAEXXZ; the sole caller 0x00783A27 is the landed
//   CTCBoastingCrowdControl::OnSetInLimbo 0x00783A10 (Startup when leaving limbo); the body calls the private
//   SpawnCrowdMember 0x00783380 (?SpawnCrowdMember@CTCBoastingCrowdControl@@AAE_NABVCThing@@ABVCThingMarker@@AAVCThingVillage@@@Z)
//   and reads Active +0x0c / CrowdSizesSuppressed +0x1c (the landed ctor 0x007837C0 layout).
// Callees: CTCBase::PeekThingSearchTools 0x00686D90 (IBE), the TU's CThingSearchTools member templates GetPFirstThing
//   <CThingFilter_HasTC, const CThing> 0x00783020, GetPFirstThing<CThingFilter_HasScriptName, const CThingMarker> 0x00783090 and
//   GetPBestThing<CThingFilter_None, CThingCompare_Nearest, CThingVillage> 0x00783110 (manifest, Ego_r name pattern), CTCBase::
//   GetWorldMap 0x00686D30 (IAE), CWorldMap::GetRegionNumberMapIsIn 0x004FC190, CCharString copy ctor 0x0099EC30 / ~CCharString
//   0x0099EAE0, CThing::GetPos 0x004C73D0, CVectorMap::LowerBound 0x004365B0 (inline GetTC<CTCHeroStats>, type 4),
//   CTCHeroStats::GetMaxBoastingCrowdForRenownLevel 0x0057AB6A (landed), __ftol2 0x00BFEA70, SpawnCrowdMember 0x00783380.
// Globals: the boasting-enabled flag 0x013833D8 (written by the landed CGameScriptInterface::SetBoastingEnabled 0x0088FC20, read by
//   GetBoastingEnabled 0x0088FC30; no PDB data symbol: descriptive `g_boastingEnabled` as that landing), g_pQuestManager 0x013B89FC
//   (Boasts list at +0x68, the landed CTCBoastUI::BoastTaken 0x0056A980 model), the anonymous-namespace
//   BOASTING_CROWD_MARKER_SCRIPT_NAME 0x013BADEC ("MK_BoastingCrowd", landed initializer 0x0121EFE0). Constants 1.0f / 0.5f.
// Shape: three early returns before the search objects; the filters are stack temporaries (CThingFilter_HasTC {0x72}, the string
//   filter copied from the global and destroyed after the search, the empty CThingFilter_None with a user ctor, the comparator's
//   memberwise C3DVector copy of the marker position); the hero-stats pointer through the PDB out-parameter GetTC<T>(T**) whose
//   result is not tested (the else path reads the shared key slot); the suppressed size is `(long)(max < 2 ? 1.0f : max * 0.5f)`;
//   the spawn loop counts down with a `spawned` byte flag.
// Layout: engine/CTCBase.h models the vptr as data; the component and its base are restated with the landed 0x007837C0 members
//   (STLport map 12 bytes: CrowdSizesSuppressed +0x1c). CThing as the landed retail-layout models (bitset +0x20, interface map
//   +0x44, CurrentMapNumber +0x8e); CTCHeroStats RenownLevel +0x70 (PDB +0x78). CThingSearchTools as the landed 0x0056D050 model.
// Same-TU body (chunk 892): retail's TU compiles CThingCompare_Nearest::operator()<CThingVillage> (the COMDAT whose kept copy is
//   0x0048B160; body = that landed row) because GetPBestThing calls it. With it visible c2 knows the comparator's address is not
//   captured, so the marker-position copy into it pipelines (`mov edx,[eax]; mov ecx,[eax+4]; mov [esp+20h],edx; mov edx,[eax+8]`,
//   0x00783924) as in retail; declared only, every store orders the next load. C3DVector is the shared engine/C3DVector.h.
#include "fable_compiler.h"
class CCharStringData;
class CCharString { public:
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;
};
#include "engine/C3DVector.h"
enum ETCInterfaceType { TC_INTERFACE_HERO_STATS = 4, TC_INTERFACE_BOASTING_PODIUM = 0x72 };   // GetInterfaceType values (0x72: the podium component this searches for)
#include "stlport/fable_containers.h"                     // std::pair, std::bitset (STLport 4.6)
class CTCBase;
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char      _pad00[0x20];
    std::bitset<274>   UsingComponents;                     // +0x20
    CInterfaceMap      Components;                          // +0x44 (first/last; the rest of the 0x10-byte map unused here)
    unsigned char      _pad4c[0x6c - 0x4c];
    unsigned long      ThingTypeFlags;                      // +0x6c
    unsigned char      _pad70[0x8e - 0x70];
    short              CurrentMapNumber;                    // +0x8e (PDB +0x9e)
    unsigned char      _pad90[1];
    unsigned char      StateBits;                           // +0x91 Dead / IncludeInThingScans / Activated / InLimbo
    enum { ThingTypeMask = 0 };
    bool IsIncludedInThingScans() const
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
    long GetCurrentMapNumber() const { return CurrentMapNumber; }   // Ego_r ?GetCurrentMapNumber@CThing@@QBEJXZ
    const C3DVector& GetPos() const;                        // 0x004C73D0
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    FABLE_FORCEINLINE CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (it == map->last || interfaceType < it->first ? map->last : it)->second;
    }
    FABLE_FORCEINLINE CTCBase* GetTCIfPresent(long interfaceType)
    {
        CTCBase* tc;
        if (HasTC((ETCInterfaceType)interfaceType))
            tc = GetTC(interfaceType);
        return tc;
    }
};
class CThingMarker : public CThing { public: enum { ThingTypeMask = 0x4 }; };
class CThingVillage : public CThing { public: enum { ThingTypeMask = 0x1000 }; };
class CWorldMap { public:
    long GetRegionNumberMapIsIn(long mapNumber) const;      // 0x004FC190
};
struct CThingList {                                          // the thing manager's intrusive list of every thing
    struct SNode { SNode* m_pNext; SNode* m_pPrev; CThing* m_value; };
    SNode* m_pHead;
    struct iterator {
        SNode* m_pNode;
        iterator(SNode* node) : m_pNode(node) {}
        CThing* operator*() const { return m_pNode->m_value; }
        iterator& operator++() { m_pNode = m_pNode->m_pNext; return *this; }
        bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; }
    };
    iterator begin() { return iterator(m_pHead->m_pNext); }
    iterator end() { return iterator(m_pHead); }
};
template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
class CWorldMap;
class CThingManager;
struct CThingMapManager;
namespace NThingMap { class CThingMap; }
class CThingSearchTools { public:                           // Ego_r.pdb (retail 0x18)
    CThingList* PeekAllThings() const;                      // 0x0049C770
    template <class TFilter, class TThing> bool GetPFirstThing(const TFilter& filter, TThing** out) const;
    template <class TFilter, class TCompare, class TThing> bool GetPBestThing(const TFilter& filter, const TCompare& compare, TThing** out) const;
    CWorldMap* WorldMap;
    CThingMapManager* PThingMapManager;
    CThingManager* ThingManager;
    unsigned char AreaMaps[0xc];                            // std::vector<NThingMap::CThingMap*>
};
class CThingFilter_HasTC { public:                          // Ego_r ??0CThingFilter_HasTC@@QAE@W4ETCInterfaceType@@@Z
    CThingFilter_HasTC(ETCInterfaceType type) : Type(type) { }
    bool operator()(const CThing& thing) const { return thing.HasTC(Type); }
    ETCInterfaceType Type;
};
class CThingFilter_HasScriptName { public:                  // Ego_r ??0CThingFilter_HasScriptName@@QAE@ABVCCharString@@@Z
    CThingFilter_HasScriptName(const CCharString& name) : Name(name) { }
    bool operator()(const CThing& thing) const;             // 0x006A7E30
    CCharString Name;
};
class CThingFilter_None { public:
    CThingFilter_None() { }
    bool operator()(const CThing& thing) const;             // Ego_r ??RCThingFilter_None@@QBE_NABVCThing@@@Z
};
class CThingCompare_Nearest { public:                       // Ego_r ??0CThingCompare_Nearest@@QAE@ABVC3DVector@@@Z
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) { }
    template <class T> T* operator()(T* best, T* candidate) const;   // out-of-line member template per thing type
    C3DVector Pos;
};

#pragma auto_inline(off)
template <class TFilter, class TThing>
bool CThingSearchTools::GetPFirstThing(const TFilter& filter, TThing** out) const
{
    CThingList* things = PeekAllThings();
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        CThing* thing = *it;
        if (IsThingOfType<TThing>(thing) && thing->IsIncludedInThingScans() && filter(*(TThing*)thing))
        {
            *out = (TThing*)thing;
            return true;
        }
    }
    return false;
}
template <class TFilter, class TCompare, class TThing>
bool CThingSearchTools::GetPBestThing(const TFilter& filter, const TCompare& compare, TThing** out) const
{
    TThing* best = 0;
    CThingList* things = PeekAllThings();
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        if ((*it)->IsIncludedInThingScans() && IsThingOfType<TThing>(*it))
        {
            TThing* candidate = (TThing*)*it;
            if (candidate && filter(*candidate))
                best = compare(best, candidate);
        }
    }
    *out = best;
    return best != 0;
}
// The TU's own instance of CThingCompare_Nearest::operator()<CThingVillage> (COMDAT; the linker kept 0x0048B160's copy):
// the landed 0x0048B160 body.
template <class T> T* CThingCompare_Nearest::operator()(T* a, T* b) const
{
    if (!a)
        return b;
    if (!b)
        return a;
    C3DVector deltaA = a->GetPos() - Pos;
    C3DVector deltaB = b->GetPos() - Pos;
    return (deltaA.GetSquaredMagnitude() < deltaB.GetSquaredMagnitude()) ? a : b;
}
#pragma auto_inline(on)
class CTCHeroStats { public:
    static ETCInterfaceType StatGetInterfaceType() { return TC_INTERFACE_HERO_STATS; }   // Ego_r ?StatGetInterfaceType@CTCHeroStats@@SI?AW4ETCInterfaceType@@XZ
    long GetMaxBoastingCrowdForRenownLevel(long renownLevel) const;   // 0x0057AB6A
    long GetRenownLevel() const { return RenownLevel; }     // Ego_r ?GetRenownLevel@CTCHeroStats@@QBEJXZ
    unsigned char _pad00[0x70];
    long RenownLevel;                                       // +0x70 (PDB +0x78)
};
struct SListNode { SListNode* _M_next; SListNode* _M_prev; };
class CBoastList { public:                                  // STLport list<CQuestManager::CBoast>, one node pointer
    SListNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};
class CQuestManager { public:
    unsigned char _pad00[0x68];
    CBoastList    Boasts;                                   // +0x68 (Ego_r +0xa4 less the STLport container sizes)
};
extern CQuestManager* g_pQuestManager;                      // 0x013B89FC
extern bool g_boastingEnabled;                              // 0x013833D8 (descriptive: set by CGameScriptInterface::SetBoastingEnabled)
namespace { extern CCharString BOASTING_CROWD_MARKER_SCRIPT_NAME; }   // 0x013BADEC "MK_BoastingCrowd"

class CTCBase { public:
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
protected:
    const CThingSearchTools& PeekThingSearchTools() const;  // 0x00686D90
    CWorldMap& GetWorldMap();                               // 0x00686D30
};
struct _Rb_tree_node_base;
class CHeroCrowdMap { public:                               // STLport map<unsigned __int64, CIntelligentPointer<CThing>>, 12 bytes
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    unsigned char       _M_key_compare;
};
class CTCBoastingCrowdControl : public CTCBase { public:
    bool          Active;                                   // +0x0c
    CHeroCrowdMap HeroCrowdThings;                          // +0x10
    bool          CrowdSizesSuppressed;                     // +0x1c
    void Startup();
private:
    bool SpawnCrowdMember(const CThing& podium, const CThingMarker& marker, CThingVillage& village);   // 0x00783380
};

void CTCBoastingCrowdControl::Startup()
{
    if (!g_boastingEnabled)
        return;
    if (g_pQuestManager->Boasts.empty())
        return;
    const CThing* podium;
    if (!PeekThingSearchTools().GetPFirstThing(CThingFilter_HasTC(TC_INTERFACE_BOASTING_PODIUM), &podium))
        return;
    long heroMap = ParentThing->GetCurrentMapNumber();
    long heroRegion = GetWorldMap().GetRegionNumberMapIsIn(heroMap);
    if (GetWorldMap().GetRegionNumberMapIsIn(podium->GetCurrentMapNumber()) != heroRegion)
        return;
    const CThingMarker* marker;
    bool found = PeekThingSearchTools().GetPFirstThing(CThingFilter_HasScriptName(BOASTING_CROWD_MARKER_SCRIPT_NAME), &marker);
    if (!found)
        return;
    CThingVillage* village = 0;
    if (!PeekThingSearchTools().GetPBestThing(CThingFilter_None(), CThingCompare_Nearest(marker->GetPos()), &village))
        return;
    CTCHeroStats* stats = (CTCHeroStats*)ParentThing->GetTCIfPresent(TC_INTERFACE_HERO_STATS);
    long max = stats->GetMaxBoastingCrowdForRenownLevel(stats->GetRenownLevel());
    if (max <= 0)
        return;
    if (CrowdSizesSuppressed) {
        max = (long)(max < 2 ? 1.0f : (float)max * 0.5f);
        CrowdSizesSuppressed = false;
    }
    bool spawned = false;
    for (long i = 0; i < max; ++i) {
        if (SpawnCrowdMember(*podium, *marker, *village))
            spawned = true;
    }
    if (spawned)
        Active = true;
}