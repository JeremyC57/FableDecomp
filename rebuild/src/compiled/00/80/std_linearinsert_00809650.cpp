// std::__linear_insert(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last, CIntelligentPointer<CThing> val, CDoNotUseBinaryPredicate comp) -- STLport insertion-sort step under the coin-flip predicate: either shift [first, last) up one and put val first, or hand val to __unguarded_linear_insert
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00809650, 172 bytes, /Gr `__fastcall` (first ecx, last edx, the 8-byte pointer and the
//   4-byte predicate on the stack, `ret 0Ch`); the callee destroys its by-value pointer.
// Identity (proven): bound ??$__linear_insert@PAV?$CIntelligentPointer@VCThing@@@@V1@UCDoNotUseBinaryPredicate@...; the only
//   caller is the landed __insertion_sort 0x00809F30, which names it so.
// Callees: GFROR13 0x00497890 (the predicate's seed step, inlined), __copy_backward<CIntelligentPointer<CThing>*, ..., int>
//   0x00655884 (the random_access_iterator_tag homed in the dead predicate slot), __unguarded_linear_insert 0x00806910,
//   CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10; vtable 0x01238C6C.
// Shape: STLport 4.6 _algo.c `if (__comp(__val, *__first)) { copy_backward(__first, __last, __last + 1); *__first = __val; }
//   else __unguarded_linear_insert(__last, __val, __comp);` with the predicate of the landed _Adjust_heap 0x00807520.
//   The empty tag's no-op user ctor is a modelling device (an implicit one stores its byte before the call), not a PDB fact.
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

class CWorld { public:
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
struct random_access_iterator_tag { random_access_iterator_tag() { } };   // user ctor: no tag-byte store (see Shape)
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*);   // 0x00655884
template <class _BI1, class _BI2>
inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00806910

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

template void __fastcall __linear_insert(ThingPointer*, ThingPointer*, ThingPointer, CDoNotUseBinaryPredicate);
}