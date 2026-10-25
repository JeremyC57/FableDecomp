// std::__uninitialized_copy(pair<CCharString,C3DDummyObject>* first, pair<CCharString,C3DDummyObject>* last, pair<CCharString,C3DDummyObject>* result, const __false_type&) -- copy-construct the dummy-object map entries [first, last) into raw storage at result; returns the end of the copy
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6EE30, 89 bytes (+7 int3), /Gr `__fastcall` (first in ecx, last in
//   edx; result and the tag reference on the stack, `ret 8`).
// Callee: CCharString::CCharString(const CCharString&) 0x0099EC30.
// Identity: the manifest's "std_Copy_CCharString_Array" is descriptive. The 56-byte stride is the CPhysicsMesh::
//   DummyObjects entry pair<CCharString, C3DDummyObject> (as the __copy_backward 0x00A6E840 of the same TU).
// Shape: STLport `_ForwardIter __cur = __result; for (; __first != __last; ++__first, ++__cur) _Construct(&*__cur,
//   *__first); return __cur;` -- the cursor in ebp from before the empty test, the placement-new null test, STLport's
//   explicit pair copy constructor (the string copy, then C3DDummyObject's implicit 13-dword `rep movsd` from a
//   strength-reduced src+4 pointer spilled to the dead result slot); `last` lives in the `push ecx` frame word.
inline void* __cdecl operator new(unsigned int, void* p) throw() { return p; }   // placement new (null-tested result)

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
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

namespace std {

class __false_type { public: __false_type() { } };

template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

typedef pair<CCharString, C3DDummyObject> TDummyEntry;
template TDummyEntry* __fastcall __uninitialized_copy<TDummyEntry*, TDummyEntry*>(TDummyEntry*, TDummyEntry*, TDummyEntry*, const __false_type&);

}