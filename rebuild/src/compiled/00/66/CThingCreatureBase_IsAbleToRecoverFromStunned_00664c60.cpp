// CThingCreatureBase::IsAbleToRecoverFromStunned() const -- false while the creature's mode component (kind 0x42) reports mode 13; true otherwise (also when it has no such component)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00664C60, length 70 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (the const instantiation, key by address),
// the component's bool query @ 0x00811340 taking 13 (manifest "CPlayer::HasMode" is propagated).
// Layout (retail, local model): the thing's component-presence bitset at +0x20 (kind k = word k>>5, bit k&31) and the
// sorted {kind, component} vector map at +0x44 {first, last}; engine/CThingCreatureBase.h is the PDB layout and names
// nothing at +0x20..+0x50, so a local CThing model is used.
// Shape: Ego_r.pdb `?IsAbleToRecoverFromStunned@CThingCreatureBase@@QBE_NXZ`. The out-parameter GetInterface template
// (the component comes back in eax: `mov eax,[eax+4] ; mov ecx,eax`), `kind 0x42` tested as `test byte ptr [ecx+28h],4`;
// `if (GetInterface(..) && mode->IsInMode(13)) return false; return true;` (the true exit is the shared tail).
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

struct CTCCreatureModeQuery {                   // component kind 0x42
    bool IsInMode(long mode) const;             // 0x00811340
};

class CThingCreatureBase : public CThing { public:
    bool IsAbleToRecoverFromStunned() const;
};

bool CThingCreatureBase::IsAbleToRecoverFromStunned() const
{
    CTCCreatureModeQuery* modes;
    if (GetInterface(&modes, 0x42) && modes->IsInMode(13))
        return false;
    return true;
}