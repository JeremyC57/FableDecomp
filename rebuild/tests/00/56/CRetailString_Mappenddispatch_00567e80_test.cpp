#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#endif
extern "C" int __cdecl printf(const char* format, ...);
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* block);
// CRetailString::_M_append_dispatch(const char*, const char*, const __false_type&) -- STLport string append dispatch for a non-integer iterator pair: append(first, last, iterator category)
// Leaf: _M_append_dispatch
// Retail: Fable.exe (TLC, VC7.1) @ 0x00567E80, 13 bytes. Callees: append(first, last, const forward_iterator_tag&)
// @ 0x00567400 (label "CTCElectrocutedEffect::InsertStringRange" is propagated; it is the STLport range
// append: grows with operator new 0x00BFEA0E, two memmoves, frees with `free`, `ret 0Ch`).
// Unreferenced in the image (recovered from a mis-cut catalog row).
// Shape: a sibling tail jump -- the arguments stay in place, the empty iterator-category temporary is homed
// in the dead `last` slot and its address overwrites the dead tag slot (`lea eax,[esp+8] ; mov [esp+0Ch],eax ; jmp`).
struct __false_type { __false_type() {} };
struct forward_iterator_tag { forward_iterator_tag() {} };

struct CRetailString {
    char* _M_start;                   // +0
    char* _M_finish;                  // +4
    char* _M_end_of_storage;          // +8
    template <class Iter, class Tag> CRetailString& append(Iter first, Iter last, const Tag&);   // 0x00567400
    CRetailString& _M_append_dispatch(const char* first, const char* last, const __false_type&);
};

void* __cdecl GrowStringStorage(CRetailString* s, unsigned int extra);   // stand-in for the growth path
// Same-TU body of the STLport range append 0x00567400 (category tag unused).
template <class Iter, class Tag> __declspec(noinline) CRetailString& CRetailString::append(Iter first, Iter last, const Tag&)
{
    if (first != last) {
        unsigned int n = (unsigned int)(last - first);
        if ((unsigned int)(_M_end_of_storage - _M_finish) <= n)
            GrowStringStorage(this, n);
        for (; first != last; ++first)
            *_M_finish++ = *first;
        *_M_finish = 0;
    }
    return *this;
}

CRetailString& CRetailString::_M_append_dispatch(const char* first, const char* last, const __false_type&)
{
    return append(first, last, forward_iterator_tag());
}

// ---- behaviour-test stubs ----
static int g_grows;
void* __cdecl GrowStringStorage(CRetailString* s, unsigned int extra)
{
    ++g_grows;
    unsigned int used = (unsigned int)(s->_M_finish - s->_M_start), cap = (used + extra + 1) * 2, i;
    char* p = (char*)malloc(cap);
    for (i = 0; i < used; ++i) p[i] = s->_M_start[i];
    if (s->_M_start) free(s->_M_start);
    s->_M_start = p; s->_M_finish = p + used; s->_M_end_of_storage = p + cap;
    return p;
}
static int Same(const CRetailString& s, const char* t)
{
    const char* p = s._M_start;
    for (; p != s._M_finish; ++p, ++t) if (*p != *t) return 0;
    return *t == 0 && *s._M_finish == 0;
}
int main()
{
    int ok = 1;
    CRetailString s; s._M_start = s._M_finish = s._M_end_of_storage = 0;
    static const char a[] = "hello", b[] = " world";
    __false_type tag;
    CRetailString& r = s._M_append_dispatch(a, a + 5, tag);
    if (&r != &s || !Same(s, "hello") || g_grows != 1) { ok = 0; printf("case1\n"); }
    s._M_append_dispatch(b, b + 6, tag);
    if (!Same(s, "hello world")) { ok = 0; printf("case2\n"); }
    s._M_append_dispatch(b, b, tag);
    if (!Same(s, "hello world")) { ok = 0; printf("case3\n"); }
    if (ok) printf("00567e80_TEST PASS\n");
    return ok ? 0 : 1;
}