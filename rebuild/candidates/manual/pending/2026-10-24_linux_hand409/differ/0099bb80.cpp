// CWideString::SprintfDynamic(const wchar_t* format, ...) -- format into a heap buffer sized wcslen(format) + 4096 characters and take the text when it fitted
// Grade: DIFFER(199v197) -- one instruction: retail forms the buffer size with `lea esi,[eax+0x1000]` after the wcslen call, ours with `mov esi,eax ; add esi,0x1000`; every other instruction (the folded resize, the cached begin() in edi, the inline operator=(const wchar_t*) and ~vector) is relocation-identical. 4 grades.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099BB80, 197 bytes. FableWin.pdb ?SprintfDynamic@CWideString@@QAAXPB_WZZ: a
// `__cdecl` variadic member (this on the stack), plain `ret`.
// Callees: wcslen @ 0x00BFED94, _vsnwprintf @ 0x00BFED9A, free @ 0x00BFEA14 (the vector's and the string's buffers),
// operator delete @ 0x00BFE9BC (the 16-byte CWideStringData), vector<unsigned short>::_M_fill_insert(iterator,
// size_type, const unsigned short&) @ 0x005149F0, CWideString::AllocStringData(const wchar_t*, long) @ 0x0099B3C0.
// Label correction: the manifest's "CCharString_FromWideFormatted" is propagated; the PDB names CWideString::
// SprintfDynamic.
// Shape: an STLport vector<unsigned short> (three zero stores) resized through resize(n, _Tp()) -- size() is known to
// be 0, so only insert(end(), n, x) -> _M_fill_insert(0, n, &zero) survives; the size is a signed int (`jg` against
// the _vsnwprintf count); the last element is zeroed through the cached begin(); `*this = &buffer[0]` is the inline
// operator=(const wchar_t*) -- UnassignString through a local copy of the block pointer, then the null/empty test and
// AllocStringData(text, -1) -- and ~vector frees the cached begin() behind its null test.
// Leaf: ?SprintfDynamic@CWideString
#include <stdarg.h>

typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

extern "C" void* __cdecl malloc(unsigned int size);                                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                                      // 0x00BFEA14
extern "C" unsigned int __cdecl wcslen(const wchar16* text);                                     // 0x00BFED94
extern "C" int __cdecl _vsnwprintf(wchar16* buffer, unsigned int count, const wchar16* format, va_list args);   // 0x00BFED9A

namespace _STL {
template <class T> class allocator {                    // STLport allocator over malloc/free
public:
    allocator() { }
    void deallocate(T* p, unsigned int) { free(p); }
};
template <class T, class A> class _STLP_alloc_proxy : public A {
public:
    T* _M_data;
    _STLP_alloc_proxy(const A& a, T* p) : A(a), _M_data(p) { }
};
template <class T, class A> class _Vector_base {
public:
    T* _M_start;                                        // +0x00
    T* _M_finish;                                       // +0x04
    _STLP_alloc_proxy<T, A> _M_end_of_storage;          // +0x08
    _Vector_base(const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    typedef unsigned int size_type;
    vector() : _Vector_base<T, A>(A()) { }
    T* begin() { return this->_M_start; }
    T* end() { return this->_M_finish; }
    size_type size() const { return this->_M_finish - this->_M_start; }
    T& operator[](size_type n) { return *(begin() + n); }
    void _M_fill_insert(T* position, size_type n, const T& x);   // 0x005149F0 (same-TU stand-in below)
    void insert(T* position, size_type n, const T& x) { _M_fill_insert(position, n, x); }
    T* erase(T* first, T* last)
    {
        T* i = first;
        for (T* p = last; p != this->_M_finish; ++p, ++i)
            *i = *p;
        this->_M_finish = i;
        return first;
    }
    void resize(size_type newSize, const T& x)
    {
        if (newSize < size())
            erase(begin() + newSize, end());
        else
            insert(end(), newSize - size(), x);
    }
    void resize(size_type newSize) { resize(newSize, T()); }
};

// Stand-in for the out-of-line 0x005149F0 (another TU in retail): a visible body that never lets `this` escape is what
// lets VC7.1 keep the buffer pointer in a register across the later calls, as retail's whole-program view did.
#pragma auto_inline(off)
template <class T, class A>
__declspec(noinline) void vector<T, A>::_M_fill_insert(T* position, size_type n, const T& x)
{
    if (n == 0)
        return;
    T value = x;
    size_type before = position - this->_M_start;
    size_type after = this->_M_finish - position;
    T* block = (T*)malloc((before + n + after) * sizeof(T));
    size_type i;
    for (i = 0; i < before; ++i)
        block[i] = this->_M_start[i];
    for (i = 0; i < n; ++i)
        block[before + i] = value;
    for (i = 0; i < after; ++i)
        block[before + n + i] = position[i];
    if (this->_M_start != 0)
        free(this->_M_start);
    this->_M_start = block;
    this->_M_finish = block + before + n + after;
    this->_M_end_of_storage._M_data = this->_M_finish;
}
#pragma auto_inline(on)
}

struct CBasicStringW {                                  // STLport basic_string<wchar_t>, 12 bytes
    wchar16* _M_start;                                  // +0x00
    wchar16* _M_finish;                                 // +0x04
    wchar16* _M_end_of_storage;                         // +0x08
    ~CBasicStringW()
    {
        if (_M_start != 0)
            free(_M_start);
    }
};
struct CWideStringData {                                // 16 bytes
    CBasicStringW Data;                                 // +0x00
    long          NoRefs;                               // +0x0c
};

class CWideString
{
public:
    CWideStringData* PStringData;                       // +0x00
    __forceinline void UnassignString()                 // out-of-line copy 0x0099B4D0
    {
        CWideStringData* data = PStringData;
        if (data != 0) {
            if (--data->NoRefs <= 0)
                delete PStringData;
            PStringData = 0;
        }
    }
    CWideStringData* AllocStringData(const wchar16* text, long length);   // 0x0099B3C0
    __forceinline CWideString& operator=(const wchar16* text)            // out-of-line copy 0x0099B800
    {
        UnassignString();
        if (text != 0 && *text != 0)
            PStringData = AllocStringData(text, -1);
        return *this;
    }
    void SprintfDynamic(const wchar16* format, ...);
};

void CWideString::SprintfDynamic(const wchar16* format, ...)
{
    _STL::vector<wchar16> buffer;
    int size = wcslen(format) + 0x1000;
    buffer.resize(size);
    va_list args;
    va_start(args, format);
    int count = _vsnwprintf(&buffer[0], size - 1, format, args);
    buffer[size - 1] = 0;
    if (count <= size)
        *this = &buffer[0];
    va_end(args);
}
