// CThingCreatureBase::IsBlocking() const -- whether the creature's combat component (kind 0x30) says it is blocking; false without one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00664D00, length 63 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (const instantiation, key by address), the
// component's bool query @ 0x007331E0 (manifest "CTCInventoryBase::GetCanItemBeQuickAssigned" is propagated).
// Layout (retail, local model): the thing's component-presence bitset at +0x20 (kind k = word k>>5, bit k&31) and the
// sorted {kind, component} vector map at +0x44 {first, last}; engine/CThingCreatureBase.h is the PDB layout and names
// nothing at +0x20..+0x50, so a local CThing model is used.
// Shape: Ego_r.pdb `?IsBlocking@CThingCreatureBase@@QBE_NXZ`. `if (HasTC(k)) return GetTC(k)->f(); return false;` with
// the pointer-returning inline find (component straight into ecx), kind 0x30 = `test dword ptr [ecx+24h],10000h`.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry;

template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                 // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    const STCEntry* LowerBound(const ETCInterfaceType& kind) const;   // 0x004365B0 (CVectorMap<ETCInterfaceType,...>::LowerBound)
    CTCBase* Find(long kind) const                 // the inlined map find: lower bound, then end when the key is absent
    {
        const STCEntry* it = LowerBound((ETCInterfaceType)kind);
        if (it == m_pLast || kind < it->first)
            it = m_pLast;
        return it->second;
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;

class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20 which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    CTCBase* GetTC(long kind) const { return m_tcs.Find(kind); }
};

class CTCCombat { public:                      // component kind 0x30
    bool IsBlocking() const;                    // 0x007331E0
};

class CThingCreatureBase : public CThing { public:
    bool IsBlocking() const;
};

bool CThingCreatureBase::IsBlocking() const
{
    if (HasTC(0x30))
        return ((CTCCombat*)GetTC(0x30))->IsBlocking();
    return false;
}