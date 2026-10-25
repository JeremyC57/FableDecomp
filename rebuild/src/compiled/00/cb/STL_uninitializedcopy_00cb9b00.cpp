// std::__uninitialized_copy(const CPersistPair* first, const CPersistPair* last, CPersistPair* result, const __false_type&) -- placement-copy the (UID, persist info) pairs into raw storage, return the advanced result
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB9B00, 72 bytes (`ret 8`).
// TU: the scripting library's STLport 4.x instantiations (0x00CB6xxx-0x00CBDxxx); the game is built /Gr, so the
//   algorithm templates are __fastcall (first two pointer arguments in ecx/edx, callee pops the rest).
// Identity: STLport __uninitialized_copy(..., __false_type) over (UID, persist info) = std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> (0x18 bytes, 8-aligned);
//   called from vector<CPersistPair>::_M_insert_overflow (0x00CBC19C, 0x00CBC1FF). The manifest label
//   "_Move<CMusicEntry*,CMusicEntry*>" is propagated and wrong.
// Callees: CCharString copy constructor 0x0099EC30.
// Shape: `_Construct(&*__cur, *__first)` = placement new of the pair (null-checked, `test edi,edi`), the implicit
//   pair copy constructor copies UID inline and calls the string copy constructor.
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
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        new (&*__cur) CPersistPair(*__first);
    return __cur;
}
template CPersistPair* __fastcall __uninitialized_copy<CPersistPair*, CPersistPair*>(CPersistPair*, CPersistPair*, CPersistPair*, const __false_type&);
}