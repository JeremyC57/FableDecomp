// std::__copy<pair<unsigned long, C3DHelperPoint>*, pair<unsigned long, C3DHelperPoint>*, int>(first, last, result, tag, dist) -- forward copy of helper-point entries
// Retail: Fable.exe 0x00A31150, 86 bytes, __fastcall (/Gr TU: first in ecx, last in edx; result, the
// random_access_iterator_tag reference and the distance pointer on the stack, `ret 0Ch`): n = (last - first) / 20,
// then n element assignments (five dwords each) with first/result advanced by 0x14; returns result.
// Identity: STLport 4.6 random-access __copy (`for (_Distance __n = __last - __first; __n > 0; --__n)`), reached from
// four C3DMeshStats-area routines (0x00A30022 in the mesh-stats loader, 0x00A321EC, 0x00A328DC, 0x00A32E8C). The 0x14-byte
// element is the only 20-byte pair of the C3DMeshStats maps: pair<unsigned long, C3DHelperPoint> of HelperPoints
// (Ego_r.pdb C3DHelperPoint: Position C3DVector +0, BoneIndex +0xC; the PDB name sets hold pair<unsigned long,
// C3DHelperPoint> heap/copy instantiations with CKeyPairCompareLess).
// Leaf: ??$__copy@PAU?$pair@KVC3DHelperPoint@@@std@@PAU12@H@std@@
class C3DVector {
public:
    float X;                                            // +0x00
    float Y;                                            // +0x04
    float Z;                                            // +0x08
};

class C3DHelperPoint {
public:
    C3DVector Position;                                 // +0x00
    long      BoneIndex;                                // +0x0C
};

namespace std {
    template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
    struct input_iterator_tag { };
    struct forward_iterator_tag : public input_iterator_tag { };
    struct bidirectional_iterator_tag : public forward_iterator_tag { };
    struct random_access_iterator_tag : public bidirectional_iterator_tag { };

    template <class _RandomAccessIter, class _OutputIter, class _Distance>
    inline _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                         const random_access_iterator_tag&, _Distance*)
    {
        for (_Distance __n = __last - __first; __n > 0; --__n) {
            *__result = *__first;
            ++__first;
            ++__result;
        }
        return __result;
    }

    template pair<unsigned long, C3DHelperPoint>* __fastcall
    __copy<pair<unsigned long, C3DHelperPoint>*, pair<unsigned long, C3DHelperPoint>*, int>(
        pair<unsigned long, C3DHelperPoint>*, pair<unsigned long, C3DHelperPoint>*, pair<unsigned long, C3DHelperPoint>*,
        const random_access_iterator_tag&, int*);
}