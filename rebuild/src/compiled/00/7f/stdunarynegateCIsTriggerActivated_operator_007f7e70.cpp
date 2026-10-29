// std::unary_negate<CIsTriggerActivated>::operator()(CThing* const& thing) const -- the negated trigger test used by find_if over a receptor's triggers: true when the thing's activation-trigger component is NOT activated
// Leaf: ??R?$unary_negate@VCIsTriggerActivated@@@std@@QBE_NABQAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F7E70, 88 bytes (`ret 4` + int3 padding). The predicate of the find_if instantiation
//   `_Find_if<CThing**, std::unary_negate<CIsTriggerActivated>>` @ 0x007F7EF0 (its three call sites); FableWin.pdb names
//   `std::not1<CIsTriggerActivated>` / `unary_negate<CIsTriggerActivated>` (CIsTriggerActivated's own operator() is inline
//   only). Catalog label `CTrigger_IsActivated` was descriptive.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0 (interface 0x66 = CTCActivationTrigger,
//   presence word 3 bit 6: `test byte [eax+2Ch],40h`).
// Layout: CTCActivationTrigger (Ego_r.pdb, 0x20): PTCReceptor +0x0c, ReceptorUID +0x10, Activated +0x18.
// Shape: `return !_M_pred(x);` over the inline `PeekTC(&trigger); return trigger->Activated;` with the out result
//   ignored: the key temporary, the out local and the dead argument slot are one frame word, so a thing without the
//   component reads the never-written local (`mov eax,[esp+4]` on the absent arm, as GOTCHAS 24l's GetInterface rule).
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry;

template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    const STCEntry* LowerBound(const ETCInterfaceType& kind) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    template <class T> __forceinline bool PeekTC(const T** out) const
    {
        if (HasTC(T::INTERFACE_TYPE)) {
            const CTCMap* map = &m_tcs;
            const STCEntry* it = map->LowerBound((ETCInterfaceType)T::INTERFACE_TYPE);
            if (it == map->m_pLast || T::INTERFACE_TYPE < it->first)
                it = map->m_pLast;
            *out = (const T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCActivationTrigger {
public:
    enum { INTERFACE_TYPE = 0x66 };
    virtual void v00();
    CThing*       ParentThing;                                  // +0x04
    unsigned char _pad08[4];                                    // CTCBase flags
    void*         PTCReceptor;                                  // +0x0c
    unsigned long ReceptorUID[2];                               // +0x10
    bool          Activated;                                    // +0x18
};

class CIsTriggerActivated {
public:
    typedef CThing* argument_type;
    typedef bool    result_type;
    bool operator()(CThing* thing) const
    {
        const CTCActivationTrigger* trigger;
        thing->PeekTC(&trigger);
        return trigger->Activated;
    }
};

namespace std {
template <class _Predicate> class unary_negate {
protected:
    _Predicate _M_pred;
public:
    explicit unary_negate(const _Predicate& x) : _M_pred(x) { }
    bool operator()(const typename _Predicate::argument_type& x) const { return !_M_pred(x); }
};
}

template class std::unary_negate<CIsTriggerActivated>;