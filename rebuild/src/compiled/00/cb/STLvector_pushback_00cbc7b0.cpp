// std::vector::push_back(const CPersistPair& x) -- append a (UID, persist info) pair: placement-copy at finish while there is room, else the out-of-line _M_insert_overflow @ 0x00CBC130
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBC7B0, 87 bytes (`ret 4`, thiscall).
// TU: the scripting library's STLport 4.x instantiations (0x00CB6xxx-0x00CBDxxx); the game is built /Gr, so the
//   algorithm templates are __fastcall (first two pointer arguments in ecx/edx, callee pops the rest).
// Identity: STLport vector<CPersistPair>::push_back over (UID, persist info) = std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> (0x18 bytes, 8-aligned);
//   callers SetEntityPersistScript 0x00CB838C and 0x00CBD25C. The manifest label "CMusicEntry_InsertIfCapacity" is
//   propagated and wrong.
// Callees: CCharString copy constructor 0x0099EC30, vector::_M_insert_overflow 0x00CBC130 (`push 1 ; push 1`).
// Shape: placement copy at _M_finish (null-checked) and `add [esi+4],18h`, else _M_insert_overflow(finish, x,
//   __false_type(), 1, true); the tag has a user constructor so no byte is stored for it.
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
}
template <class K, class V> class CKeyPairCompareLess {    // empty, passed by value (4-byte slot)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> CPersistPair;   // 24 bytes
typedef CKeyPairCompareLess<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> CPersistLess;
inline void* operator new(unsigned int, void* where) { return where; }
namespace std {
struct __false_type { __false_type() { } };     // empty tag with a user constructor (no byte store)
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                                    // +0
    _Tp* _M_finish;                                                   // +4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;           // +8
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false);   // 0x00CBC130
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}
template void std::vector<CPersistPair>::push_back(const CPersistPair&);