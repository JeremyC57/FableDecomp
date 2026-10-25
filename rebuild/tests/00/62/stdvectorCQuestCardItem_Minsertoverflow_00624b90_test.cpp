// Behaviour test for std::vector<CQuestCardItem>::_M_insert_overflow (0x00624B90): the source body with a public base,
// counting CWideString copy/destroy stubs.
#include <stdio.h>
extern "C" void* __cdecl malloc(unsigned int size);                // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                             // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);                         // 0x0099B720
    ~CWideString();                                                // 0x0099B510
};

class CQuestCardItem {                                             // 8 bytes (Ego_r.pdb)
public:
    long        ItemIndex;                                         // +0x00
    CWideString ItemText;                                          // +0x04
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIter> inline void _Destroy(_ForwardIter __first, _ForwardIter __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                  // 0x00623660
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                  // 0x006231B0
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CQuestCardItem>::_M_insert_overflow(CQuestCardItem*, const CQuestCardItem&, const __false_type&,
                                                         unsigned int, bool);

}

static int g_copies = 0, g_dtors = 0;
CWideString::CWideString(const CWideString& other) : PStringData(other.PStringData) { ++g_copies; }
CWideString::~CWideString() { ++g_dtors; }

int main()
{
    std::vector<CQuestCardItem> v;
    CQuestCardItem* blk = (CQuestCardItem*)malloc(2 * sizeof(CQuestCardItem));
    blk[0].ItemIndex = 10; blk[0].ItemText.PStringData = (void*)0x100;
    blk[1].ItemIndex = 30; blk[1].ItemText.PStringData = (void*)0x300;
    v._M_start = blk; v._M_finish = blk + 2; v._M_end_of_storage._M_data = blk + 2;
    static long xb[2]; CQuestCardItem& x = *(CQuestCardItem*)xb; x.ItemIndex = 20; x.ItemText.PStringData = (void*)0x200;
    v._M_insert_overflow(v._M_start + 1, x, std::__false_type(), 1, false);
    int ok = v._M_finish - v._M_start == 3 && v._M_end_of_storage._M_data - v._M_start == 4
        && v._M_start[0].ItemIndex == 10 && v._M_start[1].ItemIndex == 20 && v._M_start[2].ItemIndex == 30
        && v._M_start[1].ItemText.PStringData == (void*)0x200 && v._M_start[2].ItemText.PStringData == (void*)0x300
        && g_copies == 3 && g_dtors == 2;
    static long yb[2]; CQuestCardItem& y = *(CQuestCardItem*)yb; y.ItemIndex = 40; y.ItemText.PStringData = (void*)0x400;
    v._M_insert_overflow(v._M_finish, y, std::__false_type(), 3, true);
    ok = ok && v._M_finish - v._M_start == 6 && v._M_end_of_storage._M_data - v._M_start == 6
        && v._M_start[3].ItemIndex == 40 && v._M_start[5].ItemIndex == 40 && v._M_start[2].ItemIndex == 30
        && g_copies == 3 + 6 && g_dtors == 2 + 3;
    printf(ok ? "00624b90_TEST PASS\n" : "00624b90_TEST FAIL\n");
    return ok ? 0 : 1;
}
