// _STL::__copy_backward(pair<CCharString,C3DDummyObject>* first, pair<CCharString,C3DDummyObject>* last, pair<CCharString,C3DDummyObject>* result, const random_access_iterator_tag&, int*) -- copy the dummy-object map entries [first, last) backwards onto the range ending at result
// Leaf: ??$__copy_backward@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6E840, 134 bytes, /Gr `__fastcall` (first in ecx, last in edx; result,
//   the tag reference and the null distance pointer on the stack, `ret 0Ch`).
// Callee: CCharString::operator=(const CCharString&) 0x0099EFB0.
// Label correction: catalogued as `_Copy_backward<C3DGeneratorObject*,...>` (propagated). The 56-byte stride (the
//   0x92492493 multiply + `sar 5` = / 56) is the CPhysicsMesh::DummyObjects entry pair<CCharString, C3DDummyObject>
//   (C3DDummyObject = CMatrix3x4 Transform + long BoneIndex = 0x34 bytes, Ego_r.pdb), sorted by the TU's CCompareString.
// Shape: SGI `for (_Distance n = last - first; n > 0; --n) *--result = *--last; return result;` (as the landed
//   20-byte twin 0x00A6E7C0): the element assignment is the pair's implicit operator= -- the string's out-of-line
//   operator=, then the trivially assigned C3DDummyObject as a 13-dword `rep movsd` through strength-reduced src+4 /
//   dst+4 pointers (spilled to the two frame words around the call); the count is decremented in the dead `result`
//   argument slot; an empty range returns result from its slot.
class CCharString {
public:
    void* PStringData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CMatrix3x4 {                                      // 48 bytes
public:
    float M[12];
};

class C3DDummyObject {                                  // 0x34 bytes (Ego_r.pdb)
public:
    CMatrix3x4 Transform;                               // +0x00
    long       BoneIndex;                               // +0x30
};

namespace _STL {

class random_access_iterator_tag { public: random_access_iterator_tag() { } };

template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

typedef pair<CCharString, C3DDummyObject> TDummyEntry;
template TDummyEntry* __fastcall __copy_backward<TDummyEntry*, TDummyEntry*, int>(TDummyEntry*, TDummyEntry*, TDummyEntry*,
                                                                                 const random_access_iterator_tag&, int*);

}