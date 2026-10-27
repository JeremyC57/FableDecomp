// Behaviour test for 0x00A4AC60 std::vector<CLipSyncPhonemeSample>::_M_insert_overflow: the source body verbatim (comments and CRT declarations dropped), mocks for its callees.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#define std tstd
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif


class CCPPointerInfo { public:
    unsigned long RefCount;                            // +0x00
    void (__fastcall* DeleteFunc)(void* data);         // +0x04
    void*         Data;                                // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                              // +0x00
    CCPPointerInfo* Info;                              // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
        Data = 0;
        Info = 0;
    }
};

class CCharString {
public:
    void* m_pData;                                     // +0x00
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};

class CSpeechFrame;
class CLipSyncPhonemeSample {                          // Ego_r.pdb, 16 bytes
public:
    CCharString                   Symbol;              // +0x00
    unsigned long                 ID;                  // +0x04
    CCountedPointer<CSpeechFrame> SampleFrame;         // +0x08
};

namespace std {

struct __true_type { };
struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                       const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIterator>
__declspec(noinline) void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
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
public:                                                         // the bound (public) spelling of 0x00A4AC60
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
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

typedef CLipSyncPhonemeSample _Elem;
template void vector<_Elem>::_M_insert_overflow(_Elem*, const _Elem&, const __false_type&, unsigned int, bool);

} // namespace std

static int g_copies = 0, g_dtors = 0;
CCharString::CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_copies; }
CCharString::~CCharString() { ++g_dtors; }
typedef CLipSyncPhonemeSample S;
struct TestVec : std::vector<S> {
    void Call(S* pos, const S& x, unsigned int n, bool atend) { _M_insert_overflow(pos, x, std::__false_type(), n, atend); }
};
static char g_text[8][4] = { "a", "b", "c", "x", "", "", "", "" };
static S* Make(S* p, int i, CCPPointerInfo* info)
{
    CCharString* s = (CCharString*)&p->Symbol; s->m_pData = g_text[i]; p->ID = i;
    p->SampleFrame.Data = 0; p->SampleFrame.Info = info; if (info) ++info->RefCount;
    return p;
}
int main()
{
    CCPPointerInfo info; info.RefCount = 0; info.DeleteFunc = 0; info.Data = 0;
    TestVec v;
    v._M_start = (S*)malloc(3 * sizeof(S));
    Make(v._M_start + 0, 0, &info); Make(v._M_start + 1, 1, &info); Make(v._M_start + 2, 2, 0);
    v._M_finish = v._M_start + 3; v._M_end_of_storage._M_data = v._M_finish;
    S* x = (S*)malloc(sizeof(S)); Make(x, 3, &info);
    int ok = 1;
    v.Call(v._M_start + 1, *x, 1, false);
    ok &= v.size() == 4 && v._M_end_of_storage._M_data - v._M_start == 6;
    ok &= v._M_start[0].ID == 0 && v._M_start[1].ID == 3 && v._M_start[2].ID == 1 && v._M_start[3].ID == 2;
    ok &= (char*)v._M_start[1].Symbol.m_pData == g_text[3];
    ok &= g_copies == 4 && g_dtors == 3 && info.RefCount == 4;          // refs: 3 + 3 copies - 2 released
    v.Call(v._M_finish, *x, 2, true);
    ok &= v.size() == 6 && v._M_start[4].ID == 3 && v._M_start[5].ID == 3 && info.RefCount == 4 + 2;
    if (ok) printf("00a4ac60_TEST PASS\n");
    return ok ? 0 : 1;
}