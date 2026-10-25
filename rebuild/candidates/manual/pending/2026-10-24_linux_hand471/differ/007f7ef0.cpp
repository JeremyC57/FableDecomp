// std::__find_if(CThing** first, CThing** last, unary_negate<CIsTriggerActivated> pred, const random_access_iterator_tag&) -- STLport four-way unrolled find_if over a receptor's trigger things: the first thing whose activation-trigger component is NOT activated, else last
// Grade: DIFFER(430v432) -- register swap only: retail keeps `first` in ebp (moved before `push esi ; push edi`) and the first test's trigger local in edi; VC7.1 here picks edi / ebp (the [ebp] accesses are the 2 extra retail bytes). Inlining decisions, the 3/2/1 switch and the GetInterface ternary all match.
// Leaf: ??$__find_if@PAPAVCThing@@V?$unary_negate@VCIsTriggerActivated@@@std@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F7EF0, 430 bytes (+ int3 padding), `ret 8` (__fastcall: first in ecx, last in
//   edx; the empty predicate by value and the iterator tag by reference on the stack). `last` is homed in the dead
//   predicate slot; the predicate's address still serves as `this` of the out-of-line operator() calls.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0 (inlined at the first test, called at
//   the later inline ones), unary_negate<CIsTriggerActivated>::operator() @ 0x007F7E70 (landed; VC7.1's inline budget
//   runs out after the first two loop tests, so tests 3/4 and switch case 3 call it while cases 2/1 inline it again).
// Layout: CThing interface bitset +0x20 (interface 0x66 = CTCActivationTrigger, word 3 bit 6: `test byte [eax+2Ch],40h`),
//   interface map +0x44 (the 8-byte (type, component) vector); CTCActivationTrigger::Activated +0x18 (Ego_r.pdb) -- the
//   model of the landed predicate 0x007F7E70.
// Shape: STLport 4.6 `__find_if(..., const random_access_iterator_tag&)` (trip count `>> 2`, switch on the remainder
//   falling through 3/2/1); the predicate is `!PeekTC(&trigger), trigger->Activated` with the out result ignored, so a
//   thing without the component reads the never-written local (whatever register / frame word VC7.1 gave it).
class CTCBase;

struct STCEntry {
    long     m_kind;
    CTCBase* m_pTC;
};

struct CTCMap {                                 // CVectorMap<ETCInterfaceType, CTCBase*> (STLport vector, 12 bytes)
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    const STCEntry* LowerBound(const unsigned long& kind) const   // 0x004365B0
    {
        const STCEntry* it = m_pFirst;
        int len = int(m_pLast - m_pFirst);
        while (len > 0) {
            int half = len / 2;
            const STCEntry* middle = it + half;
            if (middle->m_kind < (long)kind) {
                it = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        return it;
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
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class _Predicate> class unary_negate {
protected:
    _Predicate _M_pred;
public:
    explicit unary_negate(const _Predicate& x) : _M_pred(x) { }
    bool operator()(const typename _Predicate::argument_type& x) const { return !_M_pred(x); }   // 0x007F7E70
};

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last,
                                       _Predicate __pred, const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;

    for ( ; __trip_count > 0 ; --__trip_count) {
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

template CThing** __fastcall __find_if(CThing**, CThing**, unary_negate<CIsTriggerActivated>, const random_access_iterator_tag&);
}
