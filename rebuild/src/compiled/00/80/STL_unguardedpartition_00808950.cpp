// std::__unguarded_partition(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last, CIntelligentPointer<CThing> pivot, CDoNotUseBinaryPredicate comp) -- STLport introsort partition step of the thing-pointer shuffle: every comparison is a coin flip from the world's random seed; returns the cut
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00808950, 217 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx,
//   last in edx), the 8-byte pivot and the 4-byte predicate by value on the stack, `ret 0Ch`; the pivot is destroyed
//   by the callee.
// Name: STLport `__unguarded_partition<CIntelligentPointer<CThing>*, CIntelligentPointer<CThing>,
//   `anonymous namespace'::CDoNotUseBinaryPredicate>`; the catalog's "std_partition_Generic" is descriptive.
// Callees: GFROR13 0x00497890 (landed, __fastcall), CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 /
//   SetPItem 0x00A01B90 / dtor 0x00A01C10; CIntelligentPointer<CThing> vtable 0x01238C6C. Global: the world
//   0x013B86A0, random seed at +0x1613C.
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last; if
//   (!(first < last)) return first; iter_swap(first, last); ++first; }` with the coin-flip predicate inlined (the
//   landed 0x00805FE0 form: `seed = ROR13(seed * 0x24A1 + 0x24DF); if (seed & 1) return false; return true;`);
//   iter_swap is STLport's swap through a frame temporary (copy ctor, two SetPItem assignments, base destructor).
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
    __forceinline CIntelligentPointer(const CIntelligentPointer& p) { SetPItem(p.GetPItem()); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& p)
    {
        SetPItem(p.GetPItem());
        return *this;
    }
};
typedef CIntelligentPointer<CThing> ThingPointer;

namespace {
struct CDoNotUseBinaryPredicate {
    char m_tag;
    bool operator()(const ThingPointer&, const ThingPointer&) const
    {
        if (g_pWorld->RandomSeed.Random() & 1)
            return false;
        return true;
    }
};
}

namespace std {
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
template <class _ForwardIter1, class _ForwardIter2> inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    swap(*__i1, *__i2);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}

template ThingPointer* __fastcall __unguarded_partition(ThingPointer*, ThingPointer*, ThingPointer, CDoNotUseBinaryPredicate);
}