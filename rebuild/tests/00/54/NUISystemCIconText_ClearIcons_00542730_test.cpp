// Behaviour test for 00542730: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)

static int g_handleClears = 0, g_copyCalls = 0;
class CEnginePrimitiveHandle {
public:
    void* Data;
    virtual ~CEnginePrimitiveHandle() { }
    void Clear() { Data = 0; ++g_handleClears; }                   // stub for 0x0041BE70
};
class C2DVector { public: float X; float Y; };
struct random_access_iterator_tag { random_access_iterator_tag() {} };
struct __false_type { __false_type() {} };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                   const random_access_iterator_tag&, _Distance*)              // stub for 0x00543700
{
    ++g_copyCalls;
    for (_Distance __n = __last - __first; __n > 0; --__n, ++__first, ++__result)
        *__result = *__first;
    return __result;
}
template <class _Tp>
inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (long*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _Tp> class vector {
public:
    typedef _Tp* iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, _M_finish, __first, __false_type());
        _Destroy(__i, _M_finish);
        _M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
template <class _Ret, class _Tp> class mem_fun_ref_t {
    typedef _Ret (_Tp::*__fun_type)(void);
public:
    explicit mem_fun_ref_t(__fun_type __pf) : _M_f(__pf) {}
    _Ret operator()(_Tp& __r) const { return (__r.*_M_f)(); }
private:
    __fun_type _M_f;
};
template <class _Ret, class _Tp> inline mem_fun_ref_t<_Ret, _Tp> mem_fun_ref(_Ret (_Tp::*__f)())
{
    return mem_fun_ref_t<_Ret, _Tp>(__f);
}
template <class _InputIter, class _Function>
_Function for_each(_InputIter __first, _InputIter __last, _Function __f)        // stub for 0x0043F360
{
    for ( ; __first != __last; ++__first)
        __f(*__first);
    return __f;
}
class CIconText {
public:
    enum EIconFlags { ICON_FLAGS_NONE };
    struct CEmbeddedIcon {
        long       Graphic;
        long       CharIndex;
        EIconFlags Flags;
        C2DVector  Size;
        C2DVector  Offset;
    };
    vector<CEnginePrimitiveHandle> IconHandles;
    vector<CEmbeddedIcon>          Icons;
    virtual ~CIconText() { }
    virtual void ClearIcons()
    {
        Icons.clear();
        for_each(IconHandles.begin(), IconHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    }
};
static void RunTest()
{
    CIconText::CEmbeddedIcon icons[3];
    CEnginePrimitiveHandle handles[2];
    int marker[2];
    handles[0].Data = &marker[0];
    handles[1].Data = &marker[1];
    CIconText t;
    t.Icons._M_start = icons; t.Icons._M_finish = icons + 3; t.Icons._M_end_of_storage = icons + 3;
    t.IconHandles._M_start = handles; t.IconHandles._M_finish = handles + 2; t.IconHandles._M_end_of_storage = handles + 2;
    t.ClearIcons();
    CHECK(t.Icons._M_finish == icons && t.Icons._M_start == icons && t.Icons._M_end_of_storage == icons + 3);
    CHECK(g_copyCalls == 1);                                       // the empty tail move of erase(begin, end)
    CHECK(g_handleClears == 2 && handles[0].Data == 0 && handles[1].Data == 0);
    CHECK(t.IconHandles._M_finish == handles + 2);                 // the handles stay, only cleared
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("00542730_TEST FAIL\n");
        return 1;
    }
    std::printf("00542730_TEST PASS\n");
    return 0;
}