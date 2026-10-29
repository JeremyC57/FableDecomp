// CThingFilter_IsAffectedByObstructions::operator()(const CThing&) const -- (anonymous-namespace filter) true unless the thing has a creature mode manager whose current mode is HitByEpicSpell (0x5f)
// Leaf: ??RCThingFilter_IsAffectedByObstructions@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FDA20, 82 bytes, `ret 4`, ecx (the empty functor) unused.
// Identity (proven): its only caller 0x007FDAD0 is CFilter_Bind3<CThingFilter_IsNotList (0x006A7EB0), CThingFilter_IsWithinArea
//   (0x007FD830), this>::operator(); Ego_r.pdb's CTCDPhysicalObstruction TU instantiates exactly
//   GetAllThingsInRoughArea<..., CFilter_Bind3<CThingFilter_IsNotList, CThingFilter_IsWithinArea,
//   CThingFilter_IsAffectedByObstructions@?A0x6bf166b2> > and lists `??RCThingFilter_IsAffectedByObstructions@?A0x6bf166b2@@QBE_NABVCThing@@@Z`
//   right after IsWithinArea's operator(), the retail order here. Mode 0x5f is NCreatureMode::CModeHitByEpicSpell (its GetType
//   0x00741CC0 returns 0x5f).
// Callees: CVectorMap LowerBound 0x004365B0 (const), CTCCreatureModeManager::GetCurrentMode 0x0073A6E0, CModeBase::GetType (slot 1).
class CTCBase {
public:
    virtual ~CTCBase();
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    const STCEntry* LowerBound(const ETCInterfaceType& kind) const;  // 0x004365B0 (const CVectorMap::LowerBound)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;
namespace NCreatureMode {
class CModeBase {
public:
    virtual ~CModeBase();
    virtual unsigned long GetType() const;               // slot 1 (+4)
};
}
class CTCCreatureModeManager : public CTCBase {           // interface 0x31
public:
    NCreatureMode::CModeBase& GetCurrentMode() const;     // 0x0073A6E0
};
class CThing { public:
    void*          __vftable;
    unsigned char  _pad04[0x1c];
    unsigned long  m_tcMask[8];                           // +0x20 UsingComponents bitset
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                                 // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCBase* GetTC(long kind) const
    {
        const CTCMap* map = &m_tcs;
        const STCEntry* it = map->LowerBound((ETCInterfaceType)kind);
        return (it == map->m_pLast || kind < it->first ? map->m_pLast : it)->second;
    }
};

namespace {
class CThingFilter_IsAffectedByObstructions {
public:
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsAffectedByObstructions::operator()(const CThing& thing) const
{
    if (thing.HasTC(0x31) &&
        static_cast<CTCCreatureModeManager*>(thing.GetTC(0x31))->GetCurrentMode().GetType() == 0x5f)
        return false;
    return true;
}
}

bool CallIsAffectedByObstructions_007fda20(const CThing& thing)
{
    CThingFilter_IsAffectedByObstructions filter;
    return filter(thing);
}