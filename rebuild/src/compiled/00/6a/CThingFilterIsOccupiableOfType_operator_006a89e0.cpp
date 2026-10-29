// CThingFilter_IsOccupiableOfType::operator()(const CThing& thing) const -- whether the thing is a single-person occupiable (kind 0xac) whose definition carries occupiable type flag 4
// Leaf: ??RCThingFilter_IsOccupiableOfType
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A89E0, length 80 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (const instantiation),
// CTCSinglePersonOccupiable::IsOfType(unsigned long) @ 0x0087C420 (tests the def's type mask at +0x28).
// Identity: the catalog's "CThingCreatureBase::IsAbleToRecoverFromStunned" is a propagated label (Ego_r.pdb's is the
// argument-less 0x00664C60); callers set ecx to a filter object (`ret 4` __fastcall, `this` unused). The class name is
// descriptive (real name unknown).
// Layout (retail, local model): the thing's component-presence bitset at +0x20 (kind k = word k>>5, bit k&31) and the
// sorted {kind, component} vector map at +0x44 {first, last} (no engine header names them).
// Shape: the out-parameter GetInterface template on the const thing (the component comes back in eax: `mov eax,[eax+4]
// ; mov ecx,eax`), key homed in a frame/argument slot and passed by address to the const LowerBound.
// `if (GetInterface(..) && occupiable->IsOfType(4)) return true; return false;` (two exits, the false tail shared);
// kind 0xac = `mov ecx,[eax+34h] ; test ch,10h`.
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
    const STCEntry* LowerBound(const ETCInterfaceType& kind) const;        // 0x004365B0 (CVectorMap<ETCInterfaceType,...>::LowerBound)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;

class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20 which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    template<class T> bool GetInterface(T** ppOut, long kind) const   // the landed out-parameter GetInterface template
    {
        if (m_tcMask[kind >> 5] & (1UL << (kind & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(kind);
            const CTCMap* map = &m_tcs;
            const STCEntry* it = map->LowerBound(type);
            if (it == map->m_pLast || kind < it->first)
                it = map->m_pLast;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCSinglePersonOccupiable { public:              // component kind 0xac
    bool IsOfType(unsigned long typeMask) const;    // 0x0087C420
};

struct CThingFilter_IsOccupiableOfType {
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsOccupiableOfType::operator()(const CThing& thing) const
{
    CTCSinglePersonOccupiable* occupiable;
    if (thing.GetInterface(&occupiable, 0xac) && occupiable->IsOfType(4))
        return true;
    return false;
}