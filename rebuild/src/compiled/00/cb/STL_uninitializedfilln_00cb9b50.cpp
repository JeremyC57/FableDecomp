// std::__uninitialized_fill_n(CPersistPair* first, unsigned int n, const CPersistPair& x, const __false_type&) -- placement-copy x into n raw (UID, persist info) slots, return the end
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB9B50, 68 bytes (`ret 8`).
// TU: the scripting library's STLport 4.x instantiations (0x00CB6xxx-0x00CBDxxx); the game is built /Gr, so the
//   algorithm templates are __fastcall (first two pointer arguments in ecx/edx, callee pops the rest).
// Identity: STLport __uninitialized_fill_n(..., __false_type) over (UID, persist info) = std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> (0x18 bytes, 8-aligned);
//   called from vector<CPersistPair>::_M_insert_overflow (0x00CBC1E5). The manifest label
//   "fill<CTextureManagerPoolInfo*,...>" is propagated and wrong.
// Callees: CCharString copy constructor 0x0099EC30.
// Shape: the count is an unsigned size_type (`test edx,edx ; jbe`), placement new per element as in 0x00CB9B00.
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
struct __false_type { __false_type() { } };
}
inline void* operator new(unsigned int, void* where) { return where; }
typedef std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> CPersistPair;   // 24 bytes
namespace std {
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        new (&*__cur) _Tp(__x);
    return __cur;
}
template CPersistPair* __fastcall __uninitialized_fill_n<CPersistPair*, unsigned int, CPersistPair>(CPersistPair*, unsigned int, const CPersistPair&, const __false_type&);
}