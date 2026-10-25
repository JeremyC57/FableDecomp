// CTCHeroOnlineScoreboard::GetDecryptedDocument(vector<string>& lines, const CCharString& key) -- reads the HTTP reply document, decrypts it with the key and splits it into CRLF-separated lines; -1 when there is no document
// Grade: DIFFER(322v321) -- register allocation only: retail keeps `this` in ebp and reloads the raw buffer start after resize; ours spills `this` to a frame word (frame 0x20 v 0x1C) and keeps the start in ebp from the GetDocument call (stand-ins already give retail's slot overlaps; a CryptoMatic stand-in changed nothing)
// Leaf: ?GetDecryptedDocument@CTCHeroOnlineScoreboard@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00560BB0, 320 bytes (0x140 + int3 padding), thiscall `ret 8`, returns the line
//   count from Explode (or -1).
// Identity: Ego_r.pdb ?GetDecryptedDocument@CTCHeroOnlineScoreboard@@AAEHAAV?$vector@V?$basic_string@D...@@std@@ABVCCharString@@@Z;
//   callers UploadPlayerScores 0x00561450 and the other scoreboard transactions (the landed rows already declare
//   0x00560BB0 under this name). The catalog's "LHNetworkLib_DecryptResponseDocument" is descriptive.
// Callees: LHHttp2::GetDocumentSize 0x0083EA70, LHHttp2::GetDocument(char*) 0x0083EAA0, vector<char>::vector(n, val,
//   alloc) 0x00567560, vector<char>::resize(n, val) 0x004122D0, CryptoMatic 0x0055E2C0 (the anonymous-namespace
//   DecryptBuffer inlined: mode 1), basic_string<char>::_M_range_initialize<char*> 0x00567D00, Explode 0x00560920,
//   malloc 0x00BFEA0E, memmove 0x00BFEAE6, free 0x00BFEA14; "\r\n" 0x012397F8, empty C string 0x0129AAF4.
// Layout: PHttp (scoped_ptr<LHHttp2>) +0x0c, as the landed UploadPlayerScores 0x00561450.
// Shape: the raw reply buffer is a size + 1 byte vector, the decrypted buffer a default vector resized to the same
//   size; the text string is built from the decrypted range and the "\r\n" separator string is the inline
//   const char* constructor (constant length: malloc(3) + memmove, no length checks). The Explode result is returned
//   after the separator, text, decrypted and raw buffers are freed (in that order).
//   Retail lets the separator share the decrypted vector's frame words and the text the raw vector's, keeping the
//   two buffer starts and the separator start in registers across the later calls: VC7.1 only does that when it can
//   see that no callee keeps a local's address, so every out-of-line callee that receives a local's address has a
//   same-TU `__declspec(noinline)` stand-in body below (the fill constructor, resize -> _M_fill_insert ->
//   _M_insert_overflow, the char* range initialiser, Explode and the string throw helpers). Only the leaf is graded.
#include <string.h>
#pragma intrinsic(strlen)

extern "C" void* __cdecl malloc(unsigned int size);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);    // 0x00BFEAE6
extern "C" void* __cdecl memset(void* dest, int value, unsigned int count);
#pragma intrinsic(memset)
extern "C" int __cdecl puts(const char* text);                                        // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();                                 // 0x00BFEAEC

inline void* operator new(unsigned int, void* p) { return p; }

struct CCharStringData { char* m_pString; };
class CCharString {
public:
    CCharStringData* PStringData;
    const char* c_str() const { return PStringData == 0 ? "" : PStringData->m_pString; }
};

namespace std {

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };

template <class _Tp> inline const _Tp& (min)(const _Tp& __a, const _Tp& __b) { return __b < __a ? __b : __a; }
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _CharT> class char_traits {
public:
    static bool eq(const _CharT& __c1, const _CharT& __c2) { return __c1 == __c2; }
    static unsigned int length(const _CharT* __s) { return strlen(__s); }
};

template <class _Arg1, class _Arg2, class _Result> struct binary_function { };

template <class _Traits> struct _Eq_traits : public binary_function<char, char, bool> {
    bool operator()(const char& __x, const char& __y) const { return _Traits::eq(__x, __y); }
};

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
          ((const char*)__last - (const char*)__first);
}
inline char* uninitialized_copy(const char* __first, const char* __last, char* __result)
{
    return (char*)__copy_trivial(__first, __last, __result);
}
inline char* fill_n(char* __first, unsigned int __n, const char& __val)
{
    char __tmp = __val;
    memset(__first, (unsigned char)__tmp, __n);
    return __first + __n;
}

inline void __stl_throw_length_error(const char* __msg) { puts(__msg); abort(); }
inline void __stl_throw_out_of_range(const char* __msg) { puts(__msg); abort(); }

template <class _ForwardIter1, class _ForwardIter2, class _BinaryPred>
__declspec(noinline) _ForwardIter1 __fastcall search(_ForwardIter1 __first1, _ForwardIter1 __last1,
                                                     _ForwardIter2 __first2, _ForwardIter2 __last2,
                                                     _BinaryPred __predicate)
{                                                                                      // 0x00567120 (stand-in)
    if (__first1 == __last1 || __first2 == __last2)
        return __first1;
    _ForwardIter2 __tmp(__first2);
    ++__tmp;
    if (__tmp == __last2) {
        while (__first1 != __last1 && !__predicate(*__first1, *__first2))
            ++__first1;
        return __first1;
    }
    _ForwardIter2 __p1, __p;
    __p1 = __first2; ++__p1;
    while (__first1 != __last1) {
        while (__first1 != __last1) {
            if (__predicate(*__first1, *__first2))
                break;
            ++__first1;
        }
        while (__first1 != __last1 && !__predicate(*__first1, *__first2))
            ++__first1;
        if (__first1 == __last1)
            return __last1;
        __p = __p1;
        _ForwardIter1 __current = __first1;
        if (++__current == __last1)
            return __last1;
        while (__predicate(*__current, *__p)) {
            if (++__p == __last2)
                return __first1;
            if (++__current == __last1)
                return __last1;
        }
        ++__first1;
    }
    return __first1;
}

template <class _Tp, class _Alloc> class _String_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;                                                                     // +0x00
    _Tp* _M_finish;                                                                    // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;                            // +0x08
    unsigned int max_size() const { return ((unsigned int)-1 / sizeof(_Tp)) - 1; }
    void _M_allocate_block(unsigned int __n)
    {
        if ((__n <= (max_size() + 1)) && (__n > 0)) {
            _M_start = _M_end_of_storage.allocate(__n);
            _M_finish = _M_start;
            _M_end_of_storage._M_data = _M_start + __n;
        }
        else
            _M_throw_length_error();
    }
    void _M_deallocate_block() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    __declspec(noreturn) void _M_throw_length_error() const;                            // 0x00565A30
    __declspec(noreturn) void _M_throw_out_of_range() const;                            // 0x00565360
    _String_base(const allocator_type& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_String_base() { _M_deallocate_block(); }
};

template <class _Tp, class _Alloc> void _String_base<_Tp, _Alloc>::_M_throw_length_error() const
{
    __stl_throw_length_error("basic_string");
}
template <class _Tp, class _Alloc> __declspec(noinline) void _String_base<_Tp, _Alloc>::_M_throw_out_of_range() const
{                                                                                      // 0x00565360 (stand-in)
    __stl_throw_out_of_range("basic_string");
}

template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string : protected _String_base<_CharT, _Alloc> {
public:
    typedef _Traits traits_type;
    typedef unsigned int size_type;
    typedef const _CharT* const_pointer;
    typedef basic_string<_CharT, _Traits, _Alloc> _Self;
    typedef typename _String_base<_CharT, _Alloc>::allocator_type allocator_type;
    enum { npos = -1 };

    allocator_type get_allocator() const { return this->_M_end_of_storage; }
    basic_string(const _CharT* __s, const allocator_type& __a = allocator_type())
        : _String_base<_CharT, _Alloc>(__a)
    {
        _M_range_initialize(__s, __s + traits_type::length(__s));
    }
    basic_string(const _Self& __s) : _String_base<_CharT, _Alloc>(__s.get_allocator())
    {
        _M_range_initialize(__s._M_start, __s._M_finish);
    }
    template <class _InputIterator>
    basic_string(_InputIterator __f, _InputIterator __l, const allocator_type& __a = allocator_type())
        : _String_base<_CharT, _Alloc>(__a)
    {
        _M_initialize_dispatch(__f, __l, __false_type());
    }
    ~basic_string() { }

    size_type size() const { return this->_M_finish - this->_M_start; }
    size_type find(const _Self& __s, size_type __pos = 0) const { return find(__s._M_start, __pos, __s.size()); }
    size_type find(const _CharT* __s, size_type __pos, size_type __n) const
    {
        if (__pos + __n > size())
            return npos;
        else {
            const const_pointer __result =
                search((const _CharT*)this->_M_start + __pos, (const _CharT*)this->_M_finish,
                       __s, __s + __n, _Eq_traits<_Traits>());
            return __result != this->_M_finish ? __result - this->_M_start : npos;
        }
    }
    _Self substr(size_type __pos = 0, size_type __n = npos) const
    {
        if (__pos > size())
            this->_M_throw_out_of_range();
        return _Self(this->_M_start + __pos, this->_M_start + __pos + (min)(__n, size() - __pos));
    }

    template <class _InIter>
    void _M_initialize_dispatch(_InIter __f, _InIter __l, const __false_type&) { _M_range_initialize(__f, __l); }
    template <class _InputIter>
    void _M_range_initialize(_InputIter __f, _InputIter __l) { _M_range_initialize(__f, __l, forward_iterator_tag()); }
    template <class _ForwardIter>
    void _M_range_initialize(_ForwardIter __f, _ForwardIter __l, const forward_iterator_tag&)
    {                                                                                  // const char*: inline
        int __n = __l - __f;
        this->_M_allocate_block(__n + 1);
        this->_M_finish = uninitialized_copy(__f, __l, this->_M_start);
        _M_terminate_string();
    }
    __declspec(noinline) void _M_range_initialize(_CharT* __f, _CharT* __l, const forward_iterator_tag&)
    {                                                                                  // 0x00567D00 (stand-in)
        int __n = __l - __f;
        this->_M_allocate_block(__n + 1);
        this->_M_finish = uninitialized_copy(__f, __l, this->_M_start);
        _M_terminate_string();
    }
    void _M_construct_null(_CharT* __p) { *__p = _CharT(); }
    void _M_terminate_string() { _M_construct_null(this->_M_finish); }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                      // 0x00568E70 (stand-in)
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                      // 0x00568F10 (stand-in)
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;                                                                     // +0x00
    _Tp* _M_finish;                                                                    // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;                            // +0x08
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef typename _Vector_base<_Tp, _Alloc>::allocator_type allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    vector(size_type __n, const _Tp& __val, const allocator_type& __a = allocator_type());   // 0x00567560
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    iterator erase(iterator __first, iterator __last)
    {
        iterator __i = (iterator)__copy_trivial(__last, this->_M_finish, __first);
        this->_M_finish = __i;
        return __first;
    }
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x);                                 // 0x004122D0
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
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
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);               // 0x004110D0
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false);                                     // 0x0040F6B0
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};

template <class _Tp, class _Alloc>
__declspec(noinline) vector<_Tp, _Alloc>::vector(size_type __n, const _Tp& __val, const allocator_type& __a)
    : _Vector_base<_Tp, _Alloc>(__n, __a)
{                                                                                      // 0x00567560 (stand-in)
    this->_M_finish = fill_n(this->_M_start, __n, __val);
}

template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::resize(size_type __new_size, const _Tp& __x)
{                                                                                      // 0x004122D0 (stand-in)
    if (__new_size < size())
        erase(begin() + __new_size, end());
    else
        insert(end(), __new_size - size(), __x);
}

template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_fill_insert(iterator __pos, size_type __n, const _Tp& __x)
{                                                                                      // 0x004110D0 (stand-in)
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __pos;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __copy_trivial(this->_M_finish - __n, this->_M_finish, this->_M_finish);
                this->_M_finish += __n;
                memmove(__pos + __n, __pos, __old_finish - __n - __pos);
                fill_n(__pos, __n, __x_copy);
            }
            else {
                fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __copy_trivial(__pos, __old_finish, this->_M_finish);
                this->_M_finish += __elems_after;
                fill_n(__pos, __old_finish - __pos, __x_copy);
            }
        }
        else
            _M_insert_overflow(__pos, __x, __true_type(), __n);
    }
}

template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x,
                                                                  const __true_type&, size_type __fill_len,
                                                                  bool __atend)
{                                                                                      // 0x0040F6B0 (stand-in)
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);
    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
    __new_finish = fill_n(__new_finish, __fill_len, __x);
    if (!__atend)
        __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

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

}

typedef std::basic_string<char> CNetString;
typedef std::vector<CNetString> CNetStringVector;

namespace LHNetworkLib {
class LHHttp2 {
public:
    unsigned long GetDocumentSize();                                                   // 0x0083EA70
    int GetDocument(char* buffer);                                                     // 0x0083EAA0 (LH_HTTP_STATUS)
};
}

namespace {
int __fastcall CryptoMatic(const char* in, char* out, int length, const char* key, char mode);   // 0x0055E2C0
inline int __fastcall DecryptBuffer(const char* in, char* out, int length, const char* key)
{
    return CryptoMatic(in, out, length, key, 1);
}

__declspec(noinline) int __fastcall Explode(CNetString& text, const CNetString& separator, CNetStringVector& out)
{                                                                                      // 0x00560920 (stand-in)
    CNetString::size_type pos = 0;
    CNetString::size_type found;
    while ((found = text.find(separator, pos)) != CNetString::npos) {
        CNetString piece = text.substr(pos, found - pos);
        out.push_back(piece);
        pos = found + separator.size();
    }
    return out.size();
}
}

class CTCHeroOnlineScoreboard {
public:
    void*                     __vftable;                                               // +0x00
    void*                     ParentThing;                                             // +0x04
    long                      m_8;                                                     // +0x08
    LHNetworkLib::LHHttp2*    PHttp;                                                   // +0x0c (scoped_ptr<LHHttp2>)
    int GetDecryptedDocument(CNetStringVector& lines, const CCharString& key);
};

int CTCHeroOnlineScoreboard::GetDecryptedDocument(CNetStringVector& lines, const CCharString& key)
{
    unsigned long size = PHttp->GetDocumentSize();
    if (size != 0) {
        std::vector<char> document(size + 1, 0);
        PHttp->GetDocument(&document[0]);
        std::vector<char> decrypted;
        decrypted.resize(size + 1);
        DecryptBuffer(&document[0], &decrypted[0], size, key.c_str());
        CNetString text(decrypted.begin(), decrypted.end());
        return Explode(text, CNetString("\r\n"), lines);
    }
    return -1;
}
