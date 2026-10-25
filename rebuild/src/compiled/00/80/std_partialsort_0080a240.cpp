// std::__partial_sort(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* middle, CIntelligentPointer<CThing>* last, CIntelligentPointer<CThing>*, CDoNotUseBinaryPredicate comp) -- STLport partial sort of the thing-pointer shuffle (heap-sort fallback of the introsort): heapify [first, middle), pop every later element the coin-flip predicate prefers into the heap, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080A240, 252 bytes, /Gr `__fastcall` (first ecx, middle edx; last, the
//   value-type pointer and the 4-byte predicate on the stack, `ret 0Ch`), `and esp,-8` frame.
// Identity: called from the landed __introsort_loop 0x0080AF70 (its depth-limit `partial_sort(first, last, last, comp)`,
//   which names this address) and 0x0080B01B. Template arguments as the PDB name set's
//   `(anonymous namespace)::CDoNotUseBinaryPredicate` instantiations; the catalog's "std_introsort_TargetPointHeapsort"
//   is descriptive.
// Callees: __make_heap(first, last, comp, _Tp*, int*) 0x00808230 (landed), __adjust_heap(first, hole, len, value, comp)
//   0x00807520 (landed), sort_heap(first, last, comp) 0x00809700, GFROR13 0x00497890 (the inline predicate),
//   CBaseIntelligentPointer ctor / GetPItem / SetPItem / dtor 0x00A01B10 / 0x00A01B50 / 0x00A01B90 / 0x00A01C10;
//   CIntelligentPointer<CThing> vtable 0x01238C6C. Global: the world 0x013B86A0 (random seed +0x1613C).
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, (int*)0); sort_heap(first, middle, comp);` -- the predicate inline (the
//   landed 0x00807520 named-local coin: `and eax,1` parked in a frame word), __pop_heap `__forceinline` (assign
//   *first to *i, then __adjust_heap with the temporary copied into its argument slot), the temporary destroyed after.
class CThing;
class CBaseObject;

long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CRandomSeed {
    unsigned long Seed;
    unsigned long Random()
    {
        Seed = Seed * 0x24a1 + 0x24df;
        Seed = GFROR13(Seed);
        return Seed;
    }
};

struct CWorld {
    unsigned char _pad[0x1613c];
    CRandomSeed RandomSeed;                     // +0x1613c
};

extern CWorld* g_pWorld;                        // 0x013B86A0

class CBaseIntelligentPointer {
public:
    void* PData;
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
public:
    CIntelligentPointer(const CIntelligentPointer& p) { SetPItem(p.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& p) { SetPItem(p.GetPItem()); return *this; }
};
typedef CIntelligentPointer<CThing> ThingPointer;

namespace {
struct CDoNotUseBinaryPredicate {
    char m_tag;
    bool operator()(const ThingPointer&, const ThingPointer&) const
    {
        unsigned long bit = g_pWorld->RandomSeed.Random() & 1;
        if (bit != 0)
            return false;
        return true;
    }
};
}

namespace std {
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*);                                                             // 0x00808230
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);                                                      // 0x00807520
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00809700

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (ThingPointer*)0, (int*)0);
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
__forceinline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
                              _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}

template void __fastcall __partial_sort(ThingPointer*, ThingPointer*, ThingPointer*, ThingPointer*,
                                        CDoNotUseBinaryPredicate);
}