// _STL::__adjust_heap(CThing** first, int holeIndex, int len, CThing* val, CThingCompare_NearestToPos comp) -- STLport heap sift-down for the lightning target sort: move the nearer child up until the hole reaches the bottom, then push `val` back up from there
// Grade: DIFFER(135v135) -- scheduling only: retail loads the comparator's X and Y (ecx, edx) before `sub esp,0Ch` and Z after; ours hoists only X (tried: trivial C3DVector [ecx pointer, val/topIndex push order also off], C3DVector copy ctor, comparator copy ctor, body-assign copy ctor)
// Leaf: __adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A34A0, 135 bytes, __fastcall (/Gr algorithm TU: first in ecx, holeIndex
//   in edx; len, val and the 12-byte comparator on the stack), `ret 14h`.
// Instantiation: sort / partial_sort<CThing**, CThingCompare_NearestToPos> of CTCWillLightning.cpp -- Ego_r.pdb places
//   `CThingCompare_NearestToPos::operator()<CThing>` and the std::_Push_heap / _Adjust_heap / ... / _Sort<CThing**,
//   CThingCompare_NearestToPos> family straight after CTCWillLightning::FrameUpdate, the same position this code holds in
//   retail (after the CTCWillLightning block, before its GetInterfaceType); callers __make_heap 0x007A3740.., __partial_sort
//   0x007A3FA0 and __sort_heap 0x007A3B60 of the same family. The catalog name is the Ego_r spelling.
// Callees: CThingCompare_NearestToPos::operator()<CThing>(CThing*, CThing*) const 0x007A2F90 (thiscall on the comparator;
//   landed under the CThingCompare_Nearest<CThingAICreature> label), __push_heap 0x007A30B0 (same instantiation, __fastcall).
// Layout: CThingCompare_NearestToPos (Ego_r.pdb, 12 bytes) = Pos (C3DVector) +0x00, copied as three dwords.
// Shape: STLport `topIndex = holeIndex; secondChild = 2 * holeIndex + 2; while (secondChild < len) { if
//   (comp(*(first + secondChild), *(first + (secondChild - 1)))) secondChild--; *(first + holeIndex) = *(first +
//   secondChild); holeIndex = secondChild; secondChild = 2 * (secondChild + 1); } if (secondChild == len) { ... }
//   __push_heap(first, holeIndex, topIndex, val, comp);` -- topIndex lives in the `push ecx` frame word; the comparator
//   copy goes through eax only with a C3DVector user copy constructor (the trivial copy uses ecx).
class CThing;

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

struct CThingCompare_NearestToPos {                      // 12 bytes, passed by value
    C3DVector Pos;                                       // +0x00
    bool operator()(CThing* a, CThing* b) const;         // 0x007A2F90
};

void __fastcall __push_heap(CThing** first, int holeIndex, int topIndex, CThing* val,
                            CThingCompare_NearestToPos comp);   // 0x007A30B0

void __fastcall __adjust_heap(CThing** first, int holeIndex, int len, CThing* val, CThingCompare_NearestToPos comp)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
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
