// _STL::__linear_insert(CPersistPair* first, CPersistPair* last, CPersistPair val, CPersistLess comp) -- insert val at last into the sorted (UID, persist info) run [first, last): to the front (after shifting the run up one slot) when it precedes *first, else by the unguarded backward walk
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBB920, 168 bytes (`ret 1Ch`).
// TU: the scripting library's STLport 4.x instantiations (0x00CB6xxx-0x00CBDxxx); the game is built /Gr, so the
//   algorithm templates are __fastcall (first two pointer arguments in ecx/edx, callee pops the rest).
// Identity: STLport __linear_insert over (UID, persist info) = _STL::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> (0x18 bytes, 8-aligned); caller __insertion_sort 0x00CBBE93.
//   The manifest label "NavigationPosition_InsertInOrder" is propagated and wrong.
// Callees: __copy_backward 0x00CB94D0, __unguarded_linear_insert 0x00CB92C0, CCharString copy/assign/dtor.
// Shape: copy_backward is a __forceinline wrapper passing a `random_access_iterator_tag()` (user constructor, so no
//   byte store) and `(int*)0`; `*__first = __val` is the inline pair assignment.
class CCharString {
public:
    void* m_pStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
namespace NScript {
class CRuntimeScriptedEntityPersist {
public:
    struct CPersistInfo {                        // Ego_r.pdb: 16 bytes (8-aligned)
        unsigned __int64 UID;                    // +0x00
        CCharString      PersistScript;          // +0x08
    };
};
}
namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess {    // empty, passed by value (4-byte slot)
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};
typedef _STL::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> CPersistPair;   // 24 bytes
typedef CKeyPairCompareLess<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> CPersistLess;
namespace _STL {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*);   // 0x00CB94D0
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00CB92C0

template <class _BidirectionalIter1, class _BidirectionalIter2>
__forceinline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}
template void __fastcall __linear_insert<CPersistPair*, CPersistPair, CPersistLess>(CPersistPair*, CPersistPair*, CPersistPair, CPersistLess);
}