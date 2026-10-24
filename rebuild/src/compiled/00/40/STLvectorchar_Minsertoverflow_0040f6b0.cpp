// _STL::vector<char>::_M_insert_overflow(char* position, const char& x, const __true_type&, unsigned int fill_len, bool atend) -- POD grow path: allocate old_size + max(old_size, fill_len), copy the head, fill fill_len copies of x, copy the tail unless atend, free the old block
// Leaf: ?_M_insert_overflow@?$vector@D
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040F6B0, 218 bytes (thiscall, `ret 0x14`: position, x, tag, fill_len,
// atend). Callees: malloc 0x00BFEA0E (allocate, skipped for a zero length), memmove 0x00BFEAE6 (the inline
// __copy_trivial, skipped for an empty range), free 0x00BFEA14 (_M_clear when the old block is non-null).
// Identity: the only caller is the overflow arm of the char fill-insert at 0x004110D0 (labels_external966_corrections
// 2026-10-23: vector<char>::_M_fill_insert); the manifest/forge label CString_FillRange is a propagated placeholder.
// STLport 4.x vector.c verbatim for the __true_type flavour: `(max)(old_size, fill_len)` picks by reference
// (`lea ecx,[fill_len]` / `lea ecx,[old_size]`), fill_n(char*) is memset (inline `rep stosd ; rep stosb` with the
// byte broadcast through bl/bh) returning first + n, _M_set stores start, finish, start + len.
// Unlike the unsigned-char twin 0x00485F3A/0x00485B6C (a size TU calling the out-of-line __fastcall
// __copy_trivial 0x00405BA0), this TU is a speed TU and inlines __copy_trivial as memmove + n.
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* block);
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);
extern "C" void* __cdecl memset(void* dest, int value, unsigned int count);
#pragma intrinsic(memset)

namespace _STL {

struct __true_type { };

template <class _Tp> inline const _Tp& (max)(const _Tp& a, const _Tp& b) { return a < b ? b : a; }

inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : ((char*)memmove(result, first, (const char*)last - (const char*)first)) + ((const char*)last - (const char*)first);
}

inline void fill(char* first, char* last, const char& value)
{
    char tmp = value;
    memset(first, (unsigned char)tmp, last - first);
}

inline char* fill_n(char* first, unsigned int n, const char& value)
{
    fill(first, first + n, value);
    return first + n;
}

template <class _Tp> class vector
{
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp* allocate(unsigned int n) { return n != 0 ? (_Tp*)malloc(n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* p) { if (p) free(p); }
    void _M_clear() { deallocate(_M_start); }
    void _M_set(_Tp* s, _Tp* f, _Tp* e) { _M_start = s; _M_finish = f; _M_end_of_storage = e; }

    void _M_insert_overflow(_Tp* position, const _Tp& x, const __true_type&, unsigned int fill_len, bool atend = false);
};

template <class _Tp>
void vector<_Tp>::_M_insert_overflow(_Tp* position, const _Tp& x, const __true_type&, unsigned int fill_len, bool atend)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (max)(old_size, fill_len);

    _Tp* new_start = allocate(len);
    _Tp* new_finish = (_Tp*)__copy_trivial(_M_start, position, new_start);
    new_finish = fill_n(new_finish, fill_len, x);
    if (!atend)
        new_finish = (_Tp*)__copy_trivial(position, _M_finish, new_finish);
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}

template void vector<char>::_M_insert_overflow(char*, const char&, const __true_type&, unsigned int, bool);

}