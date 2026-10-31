// CWideString::Sprintf(const unsigned short* format, ...) -- wide printf into a stack buffer of 8000 characters (or a heap vector of wcslen(format) + 4000 when that is larger); on success the string is re-assigned from the text (left unassigned when empty)
// Grade: DIFFER(265v265) -- two scratch-register rotations only: the resize count `lea eax,[esi+0FA0h] ; push eax` (ours edx) and the _vsnwprintf argument loads (retail eax=format/edx=args/ecx=count, ours ecx/eax/edx). Lever found this session: the STLport vector<unsigned short> COMDAT bodies (_M_fill_insert / _M_insert_overflow / fill / uninitialized_fill_n, the landed 0x005149F0 / 0x005135D0 sources) in the TU fix the zero-store placement and the known-zero insert position. With STLport's two-branch resize (erase/insert) the argument registers match but the zero register becomes ebx and `this` ebp (269v265, probe c8); resize(const T&), erase returning an iterator, declaration orders, begin() for &v[0], inline operator= (199) were tried.
// Flags: /GS
// Leaf: ?Sprintf@CWideString@@QAAXPBGZZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099BA70, 265 bytes, `__cdecl` variadic member (`this` pushed first), plain `ret`;
//   0x3E94-byte frame through __chkstk 0x00BFEA30, /GS cookie 0x0139C8A8 checked by 0x00BFE9F9.
// Identity: Ego_r.pdb / FableWin.pdb ?Sprintf@CWideString@@QAAXPB_WZZ (wchar_t is unsigned short under the retail flags:
//   PBG, the spelling the landed caller 0x004945B0 binds); the PDB order Sprintf, SprintfDynamic matches retail 0x0099BA70
//   (stack buffer + heap fallback) and 0x0099BB80 (heap vector only). The catalog "CCharString::SetFromFormatV" and the
//   bound "_FmtWide_0099ba70" / "NGameText::CWideString::Format" spellings are wrong (NAME_FIXES.tsv).
// Callees: wcslen 0x00BFED94, _vsnwprintf 0x00BFED9A, free 0x00BFEA14, operator delete 0x00BFE9BC,
//   CWideString::AllocStringData(const unsigned short*, long) 0x0099B3C0, and the STLport vector<unsigned short> COMDATs
//   _M_fill_insert 0x005149F0 / _M_insert_overflow 0x005135D0 / uninitialized_fill_n 0x005123A0 / fill 0x0050E500.
// Shape: the STLport 4.6 vector<unsigned short> (_Vector_base + _STLP_alloc_proxy) is declared BEFORE wcslen (the three
//   zero stores precede the call) and resized through resize(n) -> insert(end(), n - size(), x). The vector COMDAT bodies are
//   instantiated in this TU as in retail (templates are per-TU; the landed 0x005149F0 / 0x005135D0 sources verbatim, noinline):
//   with the bodies visible VC7.1's mod-ref sees that `this` never escapes, so after wcslen it still knows the vector is empty
//   (pushes the known-zero position, no size() reload). The release is the inline UnassignString + operator=(const unsigned short*).
#include <stdarg.h>

extern "C" void* __cdecl malloc(unsigned int size);                                               // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                                       // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);                     // 0x00BFEAE6
extern "C" unsigned int __cdecl wcslen(const unsigned short* text);                               // 0x00BFED94
extern "C" int __cdecl _vsnwprintf(unsigned short* buffer, unsigned int count, const unsigned short* format, va_list args);   // 0x00BFED9A
void __cdecl operator delete(void* block);                                                        // 0x00BFE9BC

namespace std {
struct __true_type { __true_type() { } };

template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

inline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const int _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : __result;
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __true_type&)
{
    return (_ForwardIter)__copy_trivial(__first, __last, __result);
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __true_type&)
{
    return (_BidirectionalIter2)__copy_trivial_backward(__first, __last, __result);
}

template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for ( ; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __true_type&)
{
    return fill_n(__first, __n, __x);
}

#pragma auto_inline(off)
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{                                                                                 // 0x005123A0
    return __uninitialized_fill_n(__first, __n, __x, __true_type());
}

template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                 // 0x0050E500
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef _Alloc allocator_type;
    typedef __true_type _TrivialAss;
    typedef __true_type _IsPODType;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
protected:
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false);                               // 0x005135D0
public:
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);    // 0x005149F0
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void erase(iterator __first, iterator __last)
    {
        pointer __i = (pointer)__copy_trivial(__last, this->_M_finish, __first);
        this->_M_finish = __i;
    }
    void resize(size_type __new_size, _Tp __x) { insert(end(), __new_size - size(), __x); }
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&,
                                                                  size_type __fill_len, bool __atend)
{                                                                                 // 0x005135D0
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
__declspec(noinline) void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
{                                                                                 // 0x005149F0
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            } else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        } else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}
#pragma auto_inline(on)
}

class CWideStringData {                                 // 16 bytes: STLport basic_string<unsigned short> + owner count
public:
    unsigned short* _M_start;                           // +0x00
    unsigned short* _M_finish;                          // +0x04
    unsigned short* _M_end_of_storage;                  // +0x08
    long            NoRefs;                             // +0x0c
    ~CWideStringData() { if (_M_start) free(_M_start); }
};

class CWideString {
public:
    CWideStringData* PStringData;                       // +0x00
    CWideStringData* AllocStringData(const unsigned short* text, long length);   // 0x0099B3C0
    void UnassignString()                               // out-of-line copy 0x0099B4D0
    {
        CWideStringData* data = PStringData;
        if (data) {
            if (--data->NoRefs <= 0) {
                CWideStringData* p = PStringData;
                if (p)
                    delete p;
            }
            PStringData = 0;
        }
    }
    void __cdecl Sprintf(const unsigned short* format, ...);
};

void __cdecl CWideString::Sprintf(const unsigned short* format, ...)
{
    unsigned short localBuffer[8000];
    std::vector<unsigned short> heapBuffer;
    long size = wcslen(format);
    unsigned short* buffer;
    if (size > 4000) {
        heapBuffer.resize(size + 4000);
        buffer = &heapBuffer[0];
    } else
        buffer = localBuffer;
    size += 4000;
    va_list args;
    va_start(args, format);
    int count = _vsnwprintf(buffer, size - 1, format, args);
    buffer[size - 1] = 0;
    if (count <= size) {
        UnassignString();
        if (buffer && *buffer)
            PStringData = AllocStringData(buffer, -1);
    }
}
