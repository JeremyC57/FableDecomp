// std::__unguarded_partition(CPersistPair* first, CPersistPair* last, CPersistPair pivot, CPersistLess comp) -- Hoare partition of (UID, persist info) pairs around the pivot UID, returns the split point
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBA580, 94 bytes (`ret 1Ch`: by-value pivot + comparator slot).
// TU: the scripting library's STLport 4.x instantiations (0x00CB6xxx-0x00CBDxxx); the game is built /Gr, so the
//   algorithm templates are __fastcall (first two pointer arguments in ecx/edx, callee pops the rest).
// Identity: STLport __unguarded_partition over (UID, persist info) = std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> (0x18 bytes, 8-aligned); the 6 callers
//   0x00CBCB5F..0x00CBCDCD are the 24-byte introsort loops. The manifest label "PSCInfo_Partition" is propagated.
// Callees: std::swap<CPersistPair> 0x00CB9230 (through the inline iter_swap), ~CCharString 0x0099EAE0 (pivot).
// Shape: the textbook loop; iter_swap is inline and calls the out-of-line swap with the two iterators.
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
template <class _Tp>
void __fastcall swap(_Tp& __a, _Tp& __b);   // 0x00CB9230

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    swap(*__i1, *__i2);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
template CPersistPair* __fastcall __unguarded_partition<CPersistPair*, CPersistPair, CPersistLess>(CPersistPair*, CPersistPair*, CPersistPair, CPersistLess);
}