// _global::_Push_heap(THitLocationIntersectionElement*, int, int, THitLocationIntersectionElement, THitLocationIntersectionElementPred) -- STLport heap sift-up: move the hole toward the top while the parent orders before the value, then drop the value in
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008170D0, 95 bytes, `ret 0x28`.  No callees.
// Element: anonymous-namespace THitLocationIntersectionElement (PDB, 32 bytes), sorted for GetPBestHitLocationFromList (0x00815F00, whose _Equal_range 0x00817E30 uses the same predicate).
// Link-time __fastcall (the algorithm TUs are /Gr): first in ecx, hole in edx, then top, the
// 32-byte record by value and the empty 4-byte comparator on the stack (4 + 32 + 4 = 0x28).
// The record is trivially copyable, so both element moves are `rep movsd` x8.
// Predicate: `cmp [parent],value.key ; jge` = signed `parent.key < value.key`.
// Shape: SGI/STLport `__push_heap` -- `parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`
// (the Dinkumware `_Top < _Hole` spelling flips the compare to `cmp ebp,edi ; jge`).
namespace N008170d0 {
struct CHitLocation;
struct C3DVector { float X; float Y; float Z; };
struct THitLocationIntersectionElement {
    long key;                     // +0x0
    CHitLocation* hit_location;   // +0x4
    C3DVector intersection_pos;   // +0x8
    C3DVector outward_normal;     // +0x14
};

struct THitLocationIntersectionElementPred {
    char m_tag;
    bool operator()(const THitLocationIntersectionElement& a, const THitLocationIntersectionElement& b) const { return a.key < b.key; }
};

void __fastcall _Push_heap(THitLocationIntersectionElement* first, int hole, int top, THitLocationIntersectionElement value, THitLocationIntersectionElementPred comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
}