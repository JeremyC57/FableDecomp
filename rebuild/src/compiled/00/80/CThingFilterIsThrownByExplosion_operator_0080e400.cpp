// CThingFilter_IsThrownByExplosion::operator()(const CThing&) const -- (anonymous-namespace filter) true when the thing has a creature mode manager whose current mode is ExplosiveForcePushed (0x42)
// Leaf: ??RCThingFilter_IsThrownByExplosion@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080E400, 82 bytes, `ret 4`, ecx (the empty functor) unused.
// Identity (probable): the CTCDExplosion TU (Ego_r.pdb anon namespace ?A0x41ace303 holds CThingFilter_IsAffectedByExplosions,
//   CThingFilter_IsEvadeExplosion and CThingFilter_IsThrownByExplosion). Its only reference is the tail jump at 0x0080E475 from
//   0x0080E460 (`if (p && !(p->+0x91 & 1)) return (*this)(*p);`, the CThing* overload), and the inlined copies of this exact test
//   in ForEachIf<..., CThingFilter_IsThrownByExplosion, ...> 0x0080E530 / 0x0080E5D0 compare the mode with 0x42 too. Mode 0x42 is
//   NCreatureMode::CModeExplosiveForcePushed (its GetType 0x008680E0 returns 0x42). The PDB names no out-of-line
//   IsThrownByExplosion operator, so the name is probable rather than proven.
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
class CThingFilter_IsThrownByExplosion {
public:
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsThrownByExplosion::operator()(const CThing& thing) const
{
    if (thing.HasTC(0x31) &&
        static_cast<CTCCreatureModeManager*>(thing.GetTC(0x31))->GetCurrentMode().GetType() == 0x42)
        return true;
    return false;
}
}

bool CallIsThrownByExplosion_0080e400(const CThing& thing)
{
    CThingFilter_IsThrownByExplosion filter;
    return filter(thing);
}