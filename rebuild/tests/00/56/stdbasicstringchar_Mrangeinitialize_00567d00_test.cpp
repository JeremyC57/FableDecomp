// Behaviour test for 00567d00: src/00567d00.cpp copied verbatim; malloc/memmove are the CRT's, the noreturn length-error
// thrower is stubbed to throw (reached by a reversed range: block size 0).
// std::basic_string<char>::_M_range_initialize(const char*, const char*, const forward_iterator_tag&)
#include <stdio.h>
#include <string.h>
static int fails = 0;
static int g_lengthErrors = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// std::basic_string<char>::_M_range_initialize(const char* f, const char* l, const forward_iterator_tag&) -- STLport: allocate (l-f)+1 bytes, copy the range and NUL-terminate it
// Leaf: ?_M_range_initialize@?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00567D00, length 90 bytes (the int3 VC7.1 emits after the final noreturn
// call included), thiscall `ret 0Ch` (the empty forward-iterator tag is the 3rd argument).
// Callees: malloc @ 0x00BFEA0E (allocator::allocate), memmove @ 0x00BFEAE6 (__copy_trivial),
// _String_base::_M_throw_length_error() const @ 0x00565A30 (landed BasicStringRaise: puts("basic_string") + abort with
// exceptions off; called as a member, `mov ecx,edi`, noreturn: nothing follows the call).
// Identity: the narrow sibling of the landed std::basic_string<unsigned short>::_M_range_initialize 0x0099C670 (same
// STLport 4.x body with 1-byte elements: no `sar`, max_size 0xFFFFFFFE, 8-bit NUL). The 33 callers are the networking
// module's std::string users (Ego_r.pdb types them basic_string<char, char_traits<char>, allocator<char> >, e.g.
// ?toLowerCase@LHNetworkLib@@YI?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@ABV23@@Z) and the landed
// basic_string rows (operator+ 0x00569690 / 0x00841B70 / 0x00841C50, vector<string>::push_back 0x0056A2D0, ...), all of
// which already declare 0x00567D00 as _M_range_initialize. The STLport name is not in a PDB (they are Dinkumware builds).
// The earlier CNetString_Retail::RangeInitialize model and the manifest's "CTCElectrocutedEffect::InitFromStringRange"
// were descriptive / propagated labels.
// Shape: STLport 4.x verbatim: _M_allocate_block(n + 1) (`n <= max_size() + 1 && n > 0`, else the length error; VC7.1
// keeps the never-true `cmp esi,-1 ; ja`), finish = uninitialized_copy(f, l, start) through __copy_trivial, then
// _M_construct_null(finish).
namespace std {

typedef unsigned int size_t;

extern "C" void* __cdecl malloc(size_t size);                              // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dst, const void* src, size_t n);    // 0x00BFEAE6

struct forward_iterator_tag { };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

template<class _CharT> class char_traits { };

template<class _Tp> class allocator {
public:
    _Tp* allocate(size_t __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template<class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string {
public:
    typedef _CharT* pointer;
    typedef size_t  size_type;

    pointer _M_start;            // +0x00
    pointer _M_finish;           // +0x04
    pointer _M_end_of_storage;   // +0x08

    size_type max_size() const { return size_type(-1) / sizeof(_CharT) - 1; }
    __declspec(noreturn) void _M_throw_length_error() const;   // 0x00565A30

    void _M_allocate_block(size_type __n)
    {
        if (__n <= max_size() + 1 && __n > 0) {
            _Alloc a;
            _M_start = a.allocate(__n);
            _M_finish = _M_start;
            _M_end_of_storage = _M_start + __n;
        } else
            _M_throw_length_error();
    }
    void _M_construct_null(pointer __p) { *__p = 0; }

    void _M_range_initialize(const _CharT* __f, const _CharT* __l, const forward_iterator_tag&);
};

template<class _CharT, class _Traits, class _Alloc>
void basic_string<_CharT, _Traits, _Alloc>::_M_range_initialize(const _CharT* __f, const _CharT* __l,
                                                                 const forward_iterator_tag&)
{
    int __n = __l - __f;
    _M_allocate_block(__n + 1);
    _M_finish = (pointer)__copy_trivial(__f, __l, _M_start);
    _M_construct_null(_M_finish);
}

template void basic_string<char>::_M_range_initialize(const char*, const char*, const forward_iterator_tag&);

}

// ---- stubs ----
namespace std {
template<class _CharT, class _Traits, class _Alloc>
void basic_string<_CharT, _Traits, _Alloc>::_M_throw_length_error() const { ++g_lengthErrors; throw 1; }
}
typedef std::basic_string<char> string_t;
int main()
{
    const char text[] = "host.example";
    string_t s;
    s._M_range_initialize(text, text + 4, std::forward_iterator_tag());
    CHECK(s._M_start != 0); CHECK(s._M_finish == s._M_start + 4); CHECK(s._M_end_of_storage == s._M_start + 5);
    CHECK(memcmp(s._M_start, "host", 4) == 0 && s._M_start[4] == 0);
    // empty range: one byte for the terminator, nothing copied
    string_t e;
    e._M_range_initialize(text, text, std::forward_iterator_tag());
    CHECK(e._M_finish == e._M_start && e._M_end_of_storage == e._M_start + 1 && e._M_start[0] == 0);
    // whole string
    string_t w;
    w._M_range_initialize(text, text + strlen(text), std::forward_iterator_tag());
    CHECK(strcmp(w._M_start, text) == 0 && w._M_finish - w._M_start == (int)strlen(text));
    // reversed range (last = first - 1): the block size n + 1 is 0, so the length error is raised
    string_t z; bool caught = false;
    try { z._M_range_initialize(text + 3, text + 2, std::forward_iterator_tag()); } catch (int) { caught = true; }
    CHECK(caught && g_lengthErrors == 1);
    if (fails == 0) printf("00567d00_TEST PASS\n"); else printf("00567d00_TEST FAIL\n");
    return fails;
}
