// std::__adjust_heap<CThingBuilding**,int,CThingBuilding*,CIsHouseLargerThan>(CThingBuilding** first, int holeIndex, int len, CThingBuilding* val, CIsHouseLargerThan comp) -- STLport heap sift-down for the region population's house heap: move the larger child up until the hole reaches a leaf, then push val back up from there
// Leaf: ??$__adjust_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00515570, 115 bytes (padded to 128), `ret 0Ch` (`__fastcall`, a /Gr algorithm TU:
//   first in ecx, holeIndex in edx; len, val and the empty comparator on the stack).
// Callers (7): 0x0051769B, 0x005176F8, 0x0051770D, 0x005177CA (make_heap/pop_heap/sort_heap of the partial_sort
//   fallback), 0x005191CA, 0x00519FD9, 0x0051AD37. Catalog label "_Adjust_heap<...>" is the Dinkumware spelling
//   propagated from Ego_r; retail is STLport 4.x `__adjust_heap` (secondChild = 2 * (secondChild + 1)).
// Callees: CIsHouseLargerThan::operator() 0x00512410 (thiscall on the by-value comparator argument itself:
//   `lea ecx,[esp+28h]`), std::__push_heap<CThingBuilding**,int,CThingBuilding*,CIsHouseLargerThan> 0x005140A0
//   (`__fastcall`, first ecx, holeIndex edx, topIndex/val/comp pushed; topIndex lives in the spare `push ecx` word).
class CThingBuilding;

namespace std {
template <class Arg1, class Arg2, class Result>
struct binary_function {
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
};
}

// PDB: an empty class whose base binary_function<const CThingBuilding*,const CThingBuilding*,bool> is listed with it.
class CIsHouseLargerThan : public std::binary_function<const CThingBuilding*, const CThingBuilding*, bool> {
public:
    bool operator()(const CThingBuilding* a, const CThingBuilding* b) const;   // 0x00512410
};

namespace std {

template <class RandomAccessIterator, class Distance, class T, class Compare>
void __fastcall __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T val, Compare comp);   // 0x005140A0

template <class RandomAccessIterator, class Distance, class T, class Compare>
void __fastcall __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T val, Compare comp)
{
    Distance topIndex = holeIndex;
    Distance secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, val, comp);
}

}

template void __fastcall std::__adjust_heap<CThingBuilding**, int, CThingBuilding*, CIsHouseLargerThan>(
    CThingBuilding** first, int holeIndex, int len, CThingBuilding* val, CIsHouseLargerThan comp);