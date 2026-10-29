// CThingFilter_IsOnFire::operator()(const CThing& thing) const -- whether the thing has a flammable component (kind 0x6d)
// that is on fire
// Leaf: ??RCThingFilter_IsOnFire@@QBE_NABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8A80, length 73 bytes (+ int3 padding).
// Identity (FINDINGS chunk 385: this and 0x006A8B40 both carried the HasFreeActionPoint label; 0x006A8B40 is the real
// one -- interface 0x2D CTCUserActionPointBase::IsActionPointFree, called from the GetPBestThing<..HasFreeActionPoint..>
// instantiations): Ego_r.pdb `??RCThingFilter_IsOnFire@@QBE_NABVCThing@@@Z` (name set); its callee 0x00780400 is
// CTCFlammable::IsOnFire() const (Ego_r.pdb `?IsOnFire@CTCFlammable@@QBE_NXZ`, manifest label; also the callee of
// CGameScriptInterface::IsThingOnFire 0x0089BD90), and both callers (0x00935970, 0x00936070) sit in the
// CAIStateGroup_MinionPutOutFires block (GoToWell 0x00935C40, PutOutFire 0x00935E40), which also calls IsOnFire directly.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (const instantiation), CTCFlammable::IsOnFire
// @ 0x00780400.
// Layout (retail, local model): the thing's component-presence bitset at +0x20 (kind k = word k>>5, bit k&31) and the
// sorted {kind, component} vector map at +0x44 {first, last} (no engine header names them).
// Shape: `HasTC(k)` then the pointer-returning inline find on the const thing (the component loaded straight into ecx:
// `mov ecx,[eax+4]`), the key converted into a frame temporary for the const LowerBound.
// Ego_r.pdb `??RCThingFilter_HasFreeActionPoint@@QBE_NABVCThing@@@Z`. `if (HasTC(k) && GetTC(k)->f()) return true;
// return false;` (two exits); kind 0x6d = `mov ecx,[eax+2Ch] ; test ch,20h`.
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

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20 which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    CTCBase* GetTC(long kind) const { return m_tcs.Find(kind); }
};

class CTCFlammable {                            // component kind 0x6d
public:
    bool IsOnFire() const;                      // 0x00780400
};

class CThingFilter_IsOnFire {
public:
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsOnFire::operator()(const CThing& thing) const
{
    if (thing.HasTC(0x6d) && ((CTCFlammable*)thing.GetTC(0x6d))->IsOnFire())
        return true;
    return false;
}