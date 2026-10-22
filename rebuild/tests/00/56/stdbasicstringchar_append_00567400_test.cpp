// Behaviour test for 00567400: src/00567400.cpp copied verbatim; malloc/memmove/free are the CRT's and the noreturn length-error
// thrower is stubbed to throw, so the in-place copy, both growth sizes (max picks the old size or the count), the empty range
// and both length checks are observable. std::basic_string<char>::append(const char*, const char*, const forward_iterator_tag&)
#ifndef _MSC_VER
#define __declspec(x)
#define __forceinline inline
#define __cdecl
#endif
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// std::basic_string<char>::append(const char* first, const char* last, const forward_iterator_tag&) -- STLport range append: grow into a fresh max(size, n)+size+1 block when the range does not fit, else copy it after the old end and move the terminator
// Leaf: append
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00567400, 281 bytes (the int3 VC7.1 emits after the final noreturn call included), `ret 0Ch`. Label correction: the
//   catalog row "CTCElectrocutedEffect::InsertStringRange" is propagated; the body is the STLport
//   `basic_string<char>::append(_ForwardIter, _ForwardIter, const forward_iterator_tag&)` instantiation for
//   `const char*` (16 callers in two string-building TUs). A second copy of the same body sits at 0x00567D60.
// Callees: malloc 0x00BFEA0E (allocator<char>::allocate, null for 0), memmove 0x00BFEAE6 (__ucopy_trivial),
//   free 0x00BFEA14 (_M_deallocate_block), basic_string::_M_throw_length_error 0x00565A30 (the exceptions-off
//   puts("basic_string"); abort() trampoline, called through `this`; never returns).
// Layout: STLport _String_base<char> {_M_start, _M_finish, _M_end_of_storage} (12 bytes).
// Shape: the STLport 4.6 source as written: size()/max_size()/capacity() inline, `(max)(old, n)` through references
//   (the `lea ecx,[esp+..]` pick between the spilled size and the reused argument slot), __ucopy_trivial's
//   `last == first ? result : memmove(..) + n` for both copies (the second test folds away against the outer
//   `first != last`), _M_construct_null as a byte store; the length error is `__declspec(noreturn)`.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void  __cdecl free(void* p);                                          // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6

namespace std {

struct forward_iterator_tag { };

template <class _Iter> inline int distance(const _Iter& __first, const _Iter& __last) { return __last - __first; }

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

__forceinline void* __ucopy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

class basic_string {
public:
    typedef unsigned int size_type;
    typedef int difference_type;
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;

    size_type size() const { return _M_finish - _M_start; }
    size_type max_size() const { return size_type(-1) - 1; }
    size_type capacity() const { return (_M_end_of_storage - _M_start) - 1; }
    char* allocate(size_type __n) { return __n != 0 ? (char*)malloc(__n) : 0; }
    void _M_deallocate_block() { if (_M_start) free(_M_start); }
    void _M_construct_null(char* __p) { *__p = 0; }
    __declspec(noreturn) void _M_throw_length_error() const;                     // 0x00565A30

    template <class _ForwardIter>
    basic_string& append(_ForwardIter __first, _ForwardIter __last, const forward_iterator_tag&)
    {
        if (__first != __last) {
            const size_type __old_size = size();
            difference_type __n = distance(__first, __last);
            if ((size_type)__n > max_size() || __old_size > max_size() - (size_type)__n)
                _M_throw_length_error();
            if (__old_size + __n > capacity()) {
                const size_type __len = __old_size + (max)(__old_size, (size_type)__n) + 1;
                char* __new_start = allocate(__len);
                char* __new_finish = __new_start;
                __new_finish = (char*)__ucopy_trivial(_M_start, _M_finish, __new_start);
                __new_finish = (char*)__ucopy_trivial(__first, __last, __new_finish);
                _M_construct_null(__new_finish);
                _M_deallocate_block();
                _M_start = __new_start;
                _M_finish = __new_finish;
                _M_end_of_storage = __new_start + __len;
            }
            else {
                _ForwardIter __f1 = __first;
                ++__f1;
                __ucopy_trivial(__f1, __last, _M_finish + 1);
                _M_construct_null(_M_finish + __n);
                *_M_finish = *__first;
                _M_finish += __n;
            }
        }
        return *this;
    }
};

template basic_string& basic_string::append<const char*>(const char*, const char*, const forward_iterator_tag&);

}

// ---- stubs ----
static int g_lengthErrors = 0;
void std::basic_string::_M_throw_length_error() const { ++g_lengthErrors; throw 1; }
static void Make(std::basic_string& s, const char* text, unsigned int cap)   // cap counts the terminator's byte
{
    unsigned int n = (unsigned int)strlen(text);
    s._M_start = (char*)malloc(cap); memcpy(s._M_start, text, n + 1);
    s._M_finish = s._M_start + n; s._M_end_of_storage = s._M_start + cap;
}
int main()
{
    const char src[] = "XYZW-0123456789";
    // empty range: nothing changes, *this is returned
    std::basic_string e; Make(e, "ab", 3); char* e0 = e._M_start;
    CHECK(&e.append(src, src, std::forward_iterator_tag()) == &e);
    CHECK(e._M_start == e0 && e._M_finish == e0 + 2 && e._M_end_of_storage == e0 + 3 && strcmp(e._M_start, "ab") == 0);
    // fits: copied in place behind the old text, terminator moved, the block kept
    std::basic_string f; Make(f, "ab", 16); char* f0 = f._M_start;
    CHECK(&f.append(src, src + 3, std::forward_iterator_tag()) == &f);
    CHECK(f._M_start == f0 && f._M_finish == f0 + 5 && f._M_end_of_storage == f0 + 16 && strcmp(f._M_start, "abXYZ") == 0);
    // one-char range in place (the tail copy is empty)
    CHECK(&f.append(src + 4, src + 5, std::forward_iterator_tag()) == &f);
    CHECK(f._M_finish == f0 + 6 && strcmp(f._M_start, "abXYZ-") == 0);
    // exactly full capacity still fits (old + n == capacity)
    std::basic_string x; Make(x, "ab", 6);
    x.append(src, src + 3, std::forward_iterator_tag());
    CHECK(x._M_finish == x._M_start + 5 && x._M_end_of_storage == x._M_start + 6 && strcmp(x._M_start, "abXYZ") == 0);
    // grows, count larger than the old size: len = old + n + 1
    std::basic_string g; Make(g, "ab", 3);
    CHECK(&g.append(src, src + 5, std::forward_iterator_tag()) == &g);
    CHECK(g._M_end_of_storage - g._M_start == 2 + 5 + 1); CHECK(g._M_finish - g._M_start == 7); CHECK(strcmp(g._M_start, "abXYZW-") == 0);
    // grows, old size larger than the count: len = old + old + 1
    std::basic_string h; Make(h, "hello", 6);
    h.append(src + 5, src + 7, std::forward_iterator_tag());
    CHECK(h._M_end_of_storage - h._M_start == 5 + 5 + 1); CHECK(h._M_finish - h._M_start == 7); CHECK(strcmp(h._M_start, "hello01") == 0);
    // length errors: a count above max_size(), and an old size that would overflow max_size() - n
    std::basic_string l; Make(l, "ab", 16); char* l0 = l._M_start; bool caught = false;
    try { l.append(src + 10, src + 9, std::forward_iterator_tag()); } catch (int) { caught = true; }
    CHECK(caught && g_lengthErrors == 1);
    caught = false;
    try { l.append(src + 10, src + 7, std::forward_iterator_tag()); } catch (int) { caught = true; }
    CHECK(caught && g_lengthErrors == 2);
    CHECK(l._M_start == l0 && l._M_finish == l0 + 2 && strcmp(l._M_start, "ab") == 0);
    if (fails == 0) printf("00567400_TEST PASS\n"); else printf("00567400_TEST FAIL\n");
    return fails;
}