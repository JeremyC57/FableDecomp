// _global::_Push_heap(CThing**, long, long, CThing*, CThingCompare_InDirectionOfAndNearestToHeuristicBitch) -- heap sift-up of thing pointers under the in-direction-and-nearest comparator
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097ADF0, 97 bytes, `ret 0x18`.
// Label correction: the catalog "_Push_heap<float*,int,float,std::less<float>>" is propagated; the
// predicate called here is CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator()(CThing*,
// CThing*) @ 0x0097ABF0 (manifest), so the heap holds CThing pointers.
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the pointer value
// and the 16-byte comparator by value (4 + 4 + 16 = 0x18).  The comparator reads four floats at
// +0..+0xC (0x0097ABF0: a 2D origin and a 2D direction); its operator() is out of line (`this` = the
// comparator slot, `push value ; push parent`).
// Shape: SGI/STLport `__push_heap`; the drop is tail-duplicated into both exits (as 0x005140A0).
class CThing;

struct CThingCompare_InDirectionOfAndNearestToHeuristicBitch {
    float OriginX;                              // +0x0
    float OriginY;                              // +0x4
    float DirectionX;                           // +0x8
    float DirectionY;                           // +0xc
    bool operator()(CThing* a, CThing* b) const;   // 0x0097ABF0
};

void __fastcall _Push_heap(CThing** first, long hole, long top, CThing* value, CThingCompare_InDirectionOfAndNearestToHeuristicBitch comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}