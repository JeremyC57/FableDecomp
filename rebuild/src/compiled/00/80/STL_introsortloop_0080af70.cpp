// _STL::__introsort_loop(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last, CIntelligentPointer<CThing>*, int depth_limit, CDoNotUseBinaryPredicate comp) -- STLport introsort recursion of the thing-pointer shuffle: partition around a coin-flip median, recurse on the right part, loop on the left; heap-sort fallback when the depth runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080AF70, 181 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx,
//   last in edx), the value-type pointer, the depth limit and the 4-byte predicate on the stack, `ret 0Ch`.
// Name: STLport `__introsort_loop<CIntelligentPointer<CThing>*, CIntelligentPointer<CThing>, int,
//   `anonymous namespace'::CDoNotUseBinaryPredicate>`; the catalog's "std_introsort_TargetPointRecursive" is
//   descriptive.
// Callees: STLport __median(a, b, c, comp) 0x00805FE0 (landed), __unguarded_partition(first, last, pivot, comp)
//   0x00808950 (this chunk), __partial_sort(first, middle, last, _Tp*, comp) 0x0080A240, itself; CBaseIntelligentPointer
//   ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Shape: STLport `while (last - first > __stl_threshold) { if (depth_limit == 0) { partial_sort(first, last, last,
//   comp); return; } --depth_limit; cut = __unguarded_partition(first, last, _Tp(__median(*first, *(first + (last -
//   first) / 2), *(last - 1), comp)), comp); __introsort_loop(cut, last, (_Tp*)0, depth_limit, comp); last = cut; }`
//   -- the comparator is loaded once (ebp) and pushed before the depth test; the pivot is copy-constructed in its
//   argument slot from the median's reference (parked in the dead comparator slot across the base constructor).
class CThing;
class CBaseObject;

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
};
typedef CIntelligentPointer<CThing> ThingPointer;

namespace {
struct CDoNotUseBinaryPredicate {
    char m_tag;
    bool operator()(const ThingPointer&, const ThingPointer&) const;
};
}

namespace _STL {
template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp);   // 0x00805FE0
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp);   // 0x00808950
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);   // 0x0080A240
template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (ThingPointer*)0, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    _RandomAccessIter __cut;
    while (__last - __first > 16) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        __cut = __unguarded_partition(__first, __last,
                                      _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                      __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}

template void __fastcall __introsort_loop(ThingPointer*, ThingPointer*, ThingPointer*, int, CDoNotUseBinaryPredicate);
}