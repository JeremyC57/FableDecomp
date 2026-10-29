// CIsCookingPot::operator()(const CIntelligentPointer<CThing>& item) const -- the item is a cooking pot: a live object of kind 5 whose type flags have bit 19 and that carries a user-action-point component (0x2d); when the predicate asks for a free point, that component must still have one
// Leaf: ??RCIsCookingPot@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00945DE0, 142 bytes (144 with padding), __fastcall `ret 4`, returns bool.
// Identity: the file-local find_if predicate of the CAIStateGroup_SitForMeal TU: Ego_r.pdb instantiates
//   `_Find_if<list<CIntelligentPointer<CThing>>::const_iterator, `anonymous namespace'::CIsCookingPot>` for
//   CAIStateGroup_SitForMeal::GetPCookingPot (landed 0x00945A90, which calls find_if 0x00945FD0 with a value-initialised
//   1-byte CIsCookingPot()); the retail find_if bodies 0x00945F50 / 0x00945FD0 call this. The catalog's
//   "CVillageTaskTorch_FindInvolvedTask" is an autoname. The bool member's name is descriptive (no PDB record).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (four times: every test re-reads the item), the thing's virtual
//   GetThingTypeFlags (slot 48, +0xc0), CVectorMap::LowerBound const 0x004365B0 (inlined GetInterface 0x2d = bit 13 of
//   +0x24), CTCUserActionPointBase::IsActionPointFree 0x007FEBF0.
// Layout: retail CThing kind byte +0x10 (5, as the landed twin CIsSeatWithSpaceOwnedBy 0x00945E70), the interface
//   bitset +0x20 / vector map +0x44.
// Shape: one && chain (null item, kind 5, `(unsigned char)(flags >> 19) & 1`), then the out-parameter GetInterface `if`
//   whose key reuses the dead `item` argument slot; `if (m_onlyWithFreePoint) return points->IsActionPointFree(); return
//   true;` -- the member test is scheduled between the end/hit select and the component load, the call's al is returned
//   as is, and the miss paths share one `xor al,al` exit.
class CTCBase;
class CBaseObject;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const; // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual unsigned long GetThingTypeFlags() const;   // slot 48 (+0xc0)
    unsigned char  _pad04[0x0c];
    unsigned char  m_kind;                      // +0x10 (5 = the object kind of seats and pots)
    unsigned char  _pad11[0x0f];
    unsigned long  m_presentBits[9];            // +0x20 interface bitset
    CInterfaceMap  m_interfaces;                // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CBaseIntelligentPointer {
public:
    virtual void v00();
    void* PData;                                // +0x04
    CBaseObject* GetPItem() const;              // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CTCUserActionPointBase {                  // interface 0x2d
public:
    bool IsActionPointFree();                   // 0x007FEBF0
};

namespace {
class CIsCookingPot {
public:
    bool m_onlyWithFreePoint;                   // +0x00 (descriptive: GetPCookingPot passes false)
    bool operator()(const CIntelligentPointer<CThing>& item) const;
};

bool CIsCookingPot::operator()(const CIntelligentPointer<CThing>& item) const
{
    if (item.GetPItem() && item.GetPItem()->m_kind == 5 && ((unsigned char)(item.GetPItem()->GetThingTypeFlags() >> 19) & 1)) {
        CTCUserActionPointBase* points;
        if (item.GetPItem()->GetInterface(&points, 0x2d)) {
            if (m_onlyWithFreePoint)
                return points->IsActionPointFree();
            return true;
        }
    }
    return false;
}
}

// The retail callers (find_if 0x00945FD0 / 0x00945F50); keeps the file-local functor emitted.
bool CallIsCookingPot(const CIntelligentPointer<CThing>& item, bool onlyFree)
{
    CIsCookingPot pred;
    pred.m_onlyWithFreePoint = onlyFree;
    return pred(item);
}