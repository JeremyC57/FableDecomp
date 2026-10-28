// std::__median(const CPersistPair& a, const CPersistPair& b, const CPersistPair& c, CPersistLess comp) -- the median of three (UID, persist info) pairs by UID (introsort pivot)
// Leaf: ??$__median@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB9D30, 111 bytes (`ret 8`).
// TU: the scripting library's STLport 4.x instantiations (0x00CB6xxx-0x00CBDxxx); the game is built /Gr, so the
//   algorithm templates are __fastcall (first two pointer arguments in ecx/edx, callee pops the rest).
// Identity: STLport __median over (UID, persist info) = std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> (0x18 bytes, 8-aligned); the 5 callers 0x00CBCBC7..0x00CBCDB9
//   are the 24-byte introsort loops. The manifest label "MinOf3" is propagated and wrong (0x00CB9DA0 has the same
//   bytes for the 16-byte element).
// Callees: none (the comparator compares the u64 keys inline, high dword first).
// Shape: the textbook three-way STLport __median returning a const reference.
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
template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}
template const CPersistPair& __fastcall __median<CPersistPair, CPersistLess>(const CPersistPair&, const CPersistPair&, const CPersistPair&, CPersistLess);
}