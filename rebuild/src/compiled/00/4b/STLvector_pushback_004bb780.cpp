// std::vector::push_back(const pair<CCharString, CPersistInfo>& x) -- append one entry to the quest manager's persistence table storage: construct it in place when there is room, else grow through _M_insert_overflow
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004BB780, 99 bytes (+ int3 padding), thiscall `ret 4`.
// Callers: 0x004B4ED0 / 0x004B4FDB (CRuntimeQuestPersistence::SetQuestScriptPersistScript 0x004B4E30 and
//   SetQuestMasterDataPersistScript 0x004B4F30, landed) and 0x004BB957.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (x3: the in-place pair copy),
//   _M_insert_overflow(pos, x, const __false_type&, fill_len, atend) 0x004BB400 (catalog "Vector_Reserve_And_Assign").
// Identity: vector<pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo> >, the storage of the
//   quest-persistence CVectorMap (12-byte entries, see 0x004BB6B0). The catalog's "std::vector_Push_Back_Realloc" is
//   descriptive.
// Shape: STLport 4.x `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);` -- _Construct is the null-tested placement new of the
//   pair's explicit copy constructor (key string, then CPersistInfo's implicit copy through two induction pointers);
//   `++_M_finish` re-reads the member after the calls; the __false_type temporary is homed in x's (already loaded)
//   argument slot.
class CCharString {
public:
    void* PStringData;                                  // the shared string block
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace NQuestManager {
class CRuntimeQuestPersistence {
public:
    class CPersistInfo {                                // Ego_r.pdb, 8 bytes (implicit copy)
    public:
        CCharString QuestName;                          // +0x00
        CCharString PersistScript;                      // +0x04
    };
};
}

inline void* operator new(unsigned int, void* where) { return where; }

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {          // STLport pair: user constructors
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x004BB400
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

template class vector<pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo> >;
}