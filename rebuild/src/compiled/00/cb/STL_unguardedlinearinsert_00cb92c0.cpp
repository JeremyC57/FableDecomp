// std::__unguarded_linear_insert(CPersistPair* last, CPersistPair val, CPersistLess comp) -- shift every earlier pair with a larger UID up one slot and drop val into the gap (no lower bound check: the caller guarantees a smaller element below)
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB92C0, 112 bytes (`ret 1Ch`: by-value 24-byte val + comparator slot).
// TU: the scripting library's STLport 4.x instantiations (0x00CB6xxx-0x00CBDxxx); the game is built /Gr, so the
//   algorithm templates are __fastcall (first two pointer arguments in ecx/edx, callee pops the rest).
// Identity: STLport __unguarded_linear_insert over (UID, persist info) = std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> (0x18 bytes, 8-aligned);
//   callers __unguarded_insertion_sort_aux 0x00CB9E7A and __linear_insert 0x00CBB9B5 (both this element type).
//   The manifest label "NavigationPosition_InsertSorted" is propagated and wrong.
// Callees: CCharString::operator= 0x0099EFB0 (the pair's implicit operator=), ~CCharString 0x0099EAE0 (val).
// Shape: textbook STLport loop (`__next = __last; --__next; while (comp(val, *__next)) ...`); the u64 compare is
//   high dword first, the pair assignment copies UID inline and calls the string operator=.
class CCharString {
public:
    void* m_pStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
namespace NScript {
class CRuntimeScriptedEntityPersist {
public:
    class CPersistInfo { public:                        // Ego_r.pdb: 16 bytes (8-aligned)
        unsigned __int64 UID;                    // +0x00
        CCharString      PersistScript;          // +0x08
    };
};
}
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess {    // empty, passed by value (4-byte slot)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> CPersistPair;   // 24 bytes
typedef CKeyPairCompareLess<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> CPersistLess;
namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
template void __fastcall __unguarded_linear_insert<CPersistPair*, CPersistPair, CPersistLess>(CPersistPair*, CPersistPair, CPersistLess);
}