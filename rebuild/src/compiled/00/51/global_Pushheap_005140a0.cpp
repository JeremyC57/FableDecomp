// _global::_Push_heap(CThingBuilding**, long, long, CThingBuilding*, CIsHouseLargerThan) -- heap sift-up of building pointers under the house-size comparator
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005140A0, 97 bytes, `ret 0xc`.
// Label correction: the catalog "_Push_heap<float*,int,float,std::less<float>>" is propagated; the
// predicate called here is CIsHouseLargerThan::operator()(CThingBuilding*, CThingBuilding*) @ 0x00512410
// (manifest), so the heap holds CThingBuilding pointers.
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the pointer value
// and the 4-byte comparator by value (4 + 4 + 4 = 0xc).  The comparator's operator() is out of line
// (`this` = the comparator slot, `push value ; push parent`): both pointers travel by value.
// Shape: SGI/STLport `__push_heap`; VC7.1 tail-duplicates the `first[hole] = value` drop into the
// "hole <= top" early exit (value reloaded from its slot) and the loop exit (value kept in ebp).
class CThingBuilding;

struct CIsHouseLargerThan {
    char m_tag;
    bool operator()(CThingBuilding* a, CThingBuilding* b) const;   // 0x00512410
};

void __fastcall _Push_heap(CThingBuilding** first, long hole, long top, CThingBuilding* value, CIsHouseLargerThan comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}