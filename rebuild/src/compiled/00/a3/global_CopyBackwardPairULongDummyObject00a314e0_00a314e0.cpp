// _global::CopyBackward_PairULongDummyObject_00a314e0(pair<unsigned long,C3DDummyObject>*, pair<unsigned long,C3DDummyObject>*, pair<unsigned long,C3DDummyObject>*, const random_access_iterator_tag&, int*) -- SGI __copy_backward over 0x38-byte POD pairs
// Leaf: CopyBackward_PairULongDummyObject_00a314e0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A314E0, length 73 bytes (+ int3 padding).
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the two tag arguments on the stack, `ret 0Ch`). The count is (last - first) / 0x38
// (`imul 92492493h ; add ; sar 5`); each step moves both pointers back one element and copies it with the implicit
// operator= of a POD, which VC7.1 emits as a 14-dword `rep movsd`. Element identity: pair<unsigned long, C3DDummyObject>
// (Ego_r.pdb C3DDummyObject 0x34: CMatrix3x4 Transform + long BoneIndex); the caller 0x00A33800 __linear_insert
// (landed by chunk 330) passes this pair type. The manifest's `_Copy_backward` label is the Dinkumware name propagated
// across byte-identical bodies.
class CMatrix3x4 { public: float M[12]; };
class C3DDummyObject { public:                // 0x34 bytes
    CMatrix3x4 Transform;              // +0x00
    long       BoneIndex;              // +0x30
};
struct PairULongDummyObject {          // pair<unsigned long, C3DDummyObject>, 0x38 bytes
    unsigned long  first;              // +0x00
    C3DDummyObject second;             // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

PairULongDummyObject* __fastcall CopyBackward_PairULongDummyObject_00a314e0(PairULongDummyObject* first, PairULongDummyObject* last, PairULongDummyObject* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}