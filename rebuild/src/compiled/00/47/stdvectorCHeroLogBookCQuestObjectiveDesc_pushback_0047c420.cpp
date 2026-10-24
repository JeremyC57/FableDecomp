// std::vector<CHeroLogBook::CQuestObjectiveDesc>::push_back(const CHeroLogBook::CQuestObjectiveDesc& x) -- STLport append: copy-construct at the finish when there is room, else grow through _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047C420, 80 bytes, `ret 4`.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (the implicit CQuestObjectiveDesc copy
// constructor, inlined), vector<CQuestObjectiveDesc>::_M_insert_overflow(position, x, const __false_type&, n, atend)
// @ 0x0047C030.
// Shape: STLport 4.x `push_back` as the CQuestDesc instantiation 0x0047CDE0: placement new with its null test,
// `++_M_finish` re-read (`add dword ptr [edi+4],0Ch`), the `__false_type` temporary over the dead `x` slot. The
// element copy is SubObjectiveID (dword), TextDescription (CCharString copy), QuestObjectiveCompleted (byte).
// Speed TU (the CHeroLogBook module).
// Label correction: the catalog's "Vector_PushBack_PersistInfo" is a guess; the 12-byte {dword, CCharString, bool}
// element is Ego_r.pdb CHeroLogBook::CQuestObjectiveDesc, whose landed __uninitialized_copy 0x0047B5F0 documents this
// row as the objective push_back calling _M_insert_overflow 0x0047C030.
// Leaf: push_back
#include <new>

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);               // 0x0099EC30
};

namespace std {
struct __false_type { __false_type() { } };
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class _Value, class _Tp> class _STLP_alloc_proxy {
public:
    _Value _M_data;
};

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;                                       // +0x00
    _Tp* _M_finish;                                      // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp> _M_end_of_storage;      // +0x08
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);       // 0x0047C030 for CQuestObjectiveDesc
    void push_back(const _Tp& __x);
};
}

class CHeroLogBook {
public:
    class CQuestObjectiveDesc {
    public:
        unsigned long SubObjectiveID;                    // +0x00
        CCharString   TextDescription;                   // +0x04
        bool          QuestObjectiveCompleted;           // +0x08
    };
};

namespace std {
template <class _Tp> void vector<_Tp>::push_back(const _Tp& __x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish, __x);
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
}

template void vector<CHeroLogBook::CQuestObjectiveDesc>::push_back(const CHeroLogBook::CQuestObjectiveDesc&);
}