// std::__find_if(CThing** first, CThing** last, CIsTriggerActivated pred, const random_access_iterator_tag&) -- STLport random-access find_if: the first trigger thing whose activation-trigger component is activated, or last
// Leaf: ??$__find_if@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F7C10, 608 bytes, /Gr `__fastcall` (first ecx, last edx -- spilled to a frame
//   word; the empty CIsTriggerActivated by value and the tag reference on the stack, `ret 8`).
// Identity: Ego_r.pdb `std::_Find_if<CThing**, CIsTriggerActivated>` is the Dinkumware counterpart; retail is STLport's
//   `__find_if` (callers 0x007F78F6, 0x007F7EDA: CTCActivationReceptorBase's trigger tests; its unary_negate twin is
//   0x007F7EF0, whose predicate is the landed 0x007F7E70). The catalog's "Entity_FindHidden" is descriptive.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0 (interface 0x66 = CTCActivationTrigger,
//   presence word 3 bit 6: `test byte [eax+2Ch],40h`).
// Shape: STLport 4.x `__find_if` (trip count `(last - first) >> 2`, four tests per pass, `switch (last - first)` over
//   3/2/1) around the landed CIsTriggerActivated::operator() (PeekTC with the out result ignored, so an absent component
//   reads the never-written local); LowerBound has its inline body (`len / 2` halving) and VC7.1's per-function inline
//   budget expands it at the first site only, calling 0x004365B0 at the other six. The component pick is GOTCHAS'
//   one-ternary `((it == last || type < it->kind) ? last : it)->m_pTC` (the if/assign form copies the inlined
//   LowerBound's result into eax instead of reusing esi).
class CTCBase;

struct STCEntry {
    long     m_kind;
    CTCBase* m_pTC;
};

struct CTCMap {
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    const STCEntry* LowerBound(const unsigned long& kind) const   // 0x004365B0
    {
        const STCEntry* first = m_pFirst;
        long len = m_pLast - m_pFirst;
        while (len > 0) {
            long half = len / 2;
            const STCEntry* middle = first + half;
            if (middle->m_kind < (long)kind) {
                first = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        return first;
    }
};

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
            const STCEntry* it = map->LowerBound((unsigned long)T::INTERFACE_TYPE);
            *out = (const T*)((it == map->m_pLast || T::INTERFACE_TYPE < it->m_kind) ? map->m_pLast : it)->m_pTC;
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
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for (; __trip_count > 0; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
    case 0:
    default:
        return __last;
    }
}

template CThing** __fastcall __find_if(CThing**, CThing**, CIsTriggerActivated, const random_access_iterator_tag&);
}