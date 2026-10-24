// _STL::__adjust_heap(CThing** first, int holeIndex, int len, CThing* value, CThingCompare_Nearest comp) -- STLport heap sift-down for the nearest-first sort: move the hole to a leaf through the preferred children, then push the value back up
// Leaf: ??$__adjust_heap@
// Grade: DIFFER(135v135) -- scheduling only in the __push_heap argument copy: retail loads comp.Pos.X and
//   .Y (`mov ecx,[esp+20h] ; mov edx,[esp+24h]`) before `sub esp,0Ch`, ours loads only X there and Y after the first store.
//   Tried: init-list and body-assignment C3DVector copy ctors, a same-TU auto_inline(off) __push_heap body, /Os /Oa /Og /G6 /G7.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0091FB30, 135 bytes, __fastcall (first in ecx, holeIndex in edx; len, value and the
//   12-byte comparator on the stack), `ret 14h`.
// Identity: STLport 4.6 __adjust_heap<CThing**, int, CThing*, CThingCompare_Nearest>; callers: __make_heap 0x0091FDB0
//   (0x0091FD7F), the 0x0091FDF2/0x0091FE3F/0x0091FF9F heap users, sort_heap 0x00920160, __partial_sort 0x009204F0. The
//   catalog label is the Dinkumware spelling `_Adjust_heap<...>` of the same instantiation.
// Callees: CThingCompare_Nearest::operator() 0x0091F740, __push_heap<CThing**, int, CThing*, CThingCompare_Nearest>
//   0x0091F870 (its catalog label "_Push_heap<float*,...>" is propagated).
// Family: the STLport sort<CThing**, CThingCompare_Nearest> instantiation of this TU (CAIStateGroup_TestTestTest::Update
//   0x0091F140 sorts its things by distance to the creature, call at 0x0091F2EF); /Gr algorithm TU: every free template is
//   __fastcall (first in ecx, second in edx when it is a pointer/int, the rest -- the 12-byte comparator last -- on the
//   stack). Same family as the landed CThingCompare_NearestToPos sort (0x007A4260 / 0x007A3B60). The comparator's
//   operator()<T> returns the nearer pointer (its catalog label says <CThingMarker>: the template body is identical for
//   every T), tested as a bool (`test eax,eax`).
// Layout: CThingCompare_Nearest (Ego_r.pdb, 12 bytes) = Pos (C3DVector, byte-packed, memberwise user copy ctor: the
//   comparator words are re-read from its argument slot for every by-value copy).
// Shape: the STLport body verbatim: topIndex homed in the `push ecx` word, `comp(first[second], first[second - 1])`,
//   the lone-child step `first[hole] = first[second - 1]`, then __push_heap with the comparator copied into the
//   argument area (`sub esp,0Ch ; mov eax,esp`, words re-read from the parameter slot).
#pragma pack(push, 1)
class C3DVector {                               // retail: byte-packed, memberwise user copy ctor
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
#pragma pack(pop)

class CThing;

class CThingCompare_Nearest {                   // Ego_r.pdb, 12 bytes, passed by value
public:
    C3DVector Pos;                              // +0x00
    template <class T> T* operator()(T* a, T* b) const;   // 0x0091F740 (returns the nearer thing; null loses)
};

namespace _STL {

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __value,
                            _Compare __comp);   // 0x0091F870

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value,
                              _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __value, __comp);
}

template void __fastcall __adjust_heap<CThing**, int, CThing*, CThingCompare_Nearest>(CThing**, int, int, CThing*,
    CThingCompare_Nearest);

}
