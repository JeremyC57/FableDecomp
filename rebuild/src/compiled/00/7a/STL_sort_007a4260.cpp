// _STL::sort(CThing** first, CThing** last, CThingCompare_NearestToPos comp) -- STLport sort of a CThing* range by distance to a position: introsort to depth 2*lg(n), then the final insertion sort
// Retail: Fable.exe @ 0x007A4260, 111 bytes. /Gr TU (__fastcall: first in ecx, last in edx, the
// 12-byte comparator by value on the stack, callee-cleaned `ret 0Ch`).
// Callees: __introsort_loop<CThing**, CThing*, int, CThingCompare_NearestToPos> @ 0x007A4140,
//          __final_insertion_sort<CThing**, CThingCompare_NearestToPos> @ 0x007A3F30 (out of line here).
// Shape: `if (first != last) { __introsort_loop(first, last, (T*)0, __lg(last - first) * 2, comp);
// __final_insertion_sort(first, last, comp); }`. `__lg` is the shift-count loop (xor ecx ; sar ; inc).
// The comparator is copied into the argument area (`sub esp,0Ch ; mov eax,esp ; mov [eax],..`) and its
// words are re-read from the parameter slot for each call: its implicit copy constructor runs the
// engine's memberwise C3DVector copy constructor (with a POD C3DVector VC7.1 caches the words in
// ebx/ebp). It is CThingCompare_NearestToPos (Ego_r.pdb: 12 bytes, one C3DVector Pos): the only
// caller (0x007A0FD6) builds it from CThing::GetPosition().
// The catalog label "MakeHeap_AndSort" is propagated.
// Leaf: ??$sort@

#pragma pack(push, 1)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
#pragma pack(pop)

class CThing;

class CThingCompare_NearestToPos {
public:
    C3DVector Pos;
    bool operator()(const CThing* a, const CThing* b) const;
};

namespace _STL {

template <class _Size>
inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1)
        ++__k;
    return __k;
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*,
                                 _Size __depth_limit, _Compare __comp);   // 0x007A4140

template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last,
                                       _Compare __comp);                  // 0x007A3F30

template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (CThing**)0, __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}

template void __fastcall sort<CThing**, CThingCompare_NearestToPos>(CThing**, CThing**, CThingCompare_NearestToPos);

} // namespace _STL