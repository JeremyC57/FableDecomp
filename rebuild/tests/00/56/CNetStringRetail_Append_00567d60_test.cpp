// Behaviour test for 00567d60: src/00567d60.cpp copied verbatim; malloc/memmove/free are the CRT's and the noreturn length-error
// thrower is stubbed to throw, so the in-place copy, both growth sizes (max picks the old size or the count), the empty range
// and both length checks are observable. CNetString_Retail::Append(const char*, const char*, const SForwardIteratorTag&)
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
// CNetString_Retail::Append(const char* first, const char* last, const forward_iterator_tag&) -- STLport basic_string<char>::append(first, last, forward_iterator_tag): grow into a fresh malloc'd block (old text, new text, terminator) when the capacity is short, else copy in place behind the old terminator
// Leaf: Append
// Retail: Fable.exe (TLC, VC7.1) @ 0x00567D60, 281 bytes (the int3 VC7.1 emits after the final noreturn call included), `ret 0Ch` (returns *this). The catalog/manifest label
// "CTCElectrocutedEffect::ReplaceString" is propagated across byte-identical bodies (15 callers, all string appends: the HTTP URL
// code 0x0083FC4B.. and this module's 0x005687F8..).
// Callees: malloc @ 0x00BFEA0E (allocator<char>::allocate, zero length -> null), memmove @ 0x00BFEAE6 (__copy_trivial behind
// uninitialized_copy), free @ 0x00BFEA14 (_M_deallocate_block), _String_base::_M_throw_length_error() const @ 0x00565A30 (puts +
// abort with exceptions off; landed, one TU copy of three).
// Layout: the retail STLport string as the landed CNetString_Retail::Substr 0x00569330 (start, finish, end of storage; 12 bytes).
// Shape: STLport 4.x `append(_ForwardIter, _ForwardIter, const forward_iterator_tag&)` verbatim: max_size() is size_t(-1) - 1, the
// old size lives in the function's `push ecx` word and `n` / the new length in the dead `last` slot (max is the const-reference
// template); __copy_trivial tests its count before memmove (the second copy's test folds away: first != last is known).
extern "C" void* __cdecl malloc(unsigned int size);                               // 0x00BFEA0E
extern "C" void  __cdecl free(void* block);                                       // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

struct SForwardIteratorTag { SForwardIteratorTag() { } };

template <class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }

__forceinline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : ((char*)memmove(result, first, ((const char*)last - (const char*)first))) + ((const char*)last - (const char*)first);
}
__forceinline char* UninitializedCopy(const char* first, const char* last, char* result)
{
    return (char*)CopyTrivial(first, last, result);
}
inline void Distance(const char* first, const char* last, int& n) { n += last - first; }

struct CNetString_Retail {                         // 12 bytes: start, finish, end of storage
    char* m_start;
    char* m_finish;
    char* m_endOfStorage;
    __declspec(noreturn) void ThrowLengthError() const;   // 0x00565A30
    unsigned int size() const { return m_finish - m_start; }
    unsigned int max_size() const { return (unsigned int)-1 - 1; }
    unsigned int capacity() const { return (m_endOfStorage - m_start) - 1; }
    char* Allocate(unsigned int n) { return n != 0 ? (char*)malloc(n) : 0; }
    void DeallocateBlock() { if (m_start) free(m_start); }
    CNetString_Retail& Append(const char* first, const char* last, const SForwardIteratorTag& tag);
};

CNetString_Retail& CNetString_Retail::Append(const char* first, const char* last, const SForwardIteratorTag&)
{
    if (first != last) {
        const unsigned int oldSize = size();
        int n = 0;
        Distance(first, last, n);
        if ((unsigned int)n > max_size() || oldSize > max_size() - (unsigned int)n)
            ThrowLengthError();
        if (oldSize + n > capacity()) {
            const unsigned int len = oldSize + Max(oldSize, (unsigned int)n) + 1;
            char* newStart = Allocate(len);
            char* newFinish = newStart;
            newFinish = UninitializedCopy(m_start, m_finish, newStart);
            newFinish = UninitializedCopy(first, last, newFinish);
            *newFinish = 0;
            DeallocateBlock();
            m_start = newStart;
            m_finish = newFinish;
            m_endOfStorage = newStart + len;
        } else {
            const char* f1 = first;
            ++f1;
            UninitializedCopy(f1, last, m_finish + 1);
            m_finish[n] = 0;
            *m_finish = *first;
            m_finish += n;
        }
    }
    return *this;
}

// ---- stubs ----
static int g_lengthErrors = 0;
void CNetString_Retail::ThrowLengthError() const { ++g_lengthErrors; throw 1; }
static void Make(CNetString_Retail& s, const char* text, unsigned int cap)   // cap counts the terminator's byte
{
    unsigned int n = (unsigned int)strlen(text);
    s.m_start = (char*)malloc(cap); memcpy(s.m_start, text, n + 1);
    s.m_finish = s.m_start + n; s.m_endOfStorage = s.m_start + cap;
}
int main()
{
    const char src[] = "XYZW-0123456789";
    // empty range: nothing changes, *this is returned
    CNetString_Retail e; Make(e, "ab", 3); char* e0 = e.m_start;
    CHECK(&e.Append(src, src, SForwardIteratorTag()) == &e);
    CHECK(e.m_start == e0 && e.m_finish == e0 + 2 && e.m_endOfStorage == e0 + 3 && strcmp(e.m_start, "ab") == 0);
    // fits: copied in place behind the old text, terminator moved, the block kept
    CNetString_Retail f; Make(f, "ab", 16); char* f0 = f.m_start;
    CHECK(&f.Append(src, src + 3, SForwardIteratorTag()) == &f);
    CHECK(f.m_start == f0 && f.m_finish == f0 + 5 && f.m_endOfStorage == f0 + 16 && strcmp(f.m_start, "abXYZ") == 0);
    // one-char range in place (the tail copy is empty)
    CHECK(&f.Append(src + 4, src + 5, SForwardIteratorTag()) == &f);
    CHECK(f.m_finish == f0 + 6 && strcmp(f.m_start, "abXYZ-") == 0);
    // exactly full capacity still fits (old + n == capacity)
    CNetString_Retail x; Make(x, "ab", 6);
    x.Append(src, src + 3, SForwardIteratorTag());
    CHECK(x.m_finish == x.m_start + 5 && x.m_endOfStorage == x.m_start + 6 && strcmp(x.m_start, "abXYZ") == 0);
    // grows, count larger than the old size: len = old + n + 1
    CNetString_Retail g; Make(g, "ab", 3);
    CHECK(&g.Append(src, src + 5, SForwardIteratorTag()) == &g);
    CHECK(g.m_endOfStorage - g.m_start == 2 + 5 + 1); CHECK(g.m_finish - g.m_start == 7); CHECK(strcmp(g.m_start, "abXYZW-") == 0);
    // grows, old size larger than the count: len = old + old + 1
    CNetString_Retail h; Make(h, "hello", 6);
    h.Append(src + 5, src + 7, SForwardIteratorTag());
    CHECK(h.m_endOfStorage - h.m_start == 5 + 5 + 1); CHECK(h.m_finish - h.m_start == 7); CHECK(strcmp(h.m_start, "hello01") == 0);
    // length errors: a count above max_size(), and an old size that would overflow max_size() - n
    CNetString_Retail l; Make(l, "ab", 16); char* l0 = l.m_start; bool caught = false;
    try { l.Append(src + 10, src + 9, SForwardIteratorTag()); } catch (int) { caught = true; }
    CHECK(caught && g_lengthErrors == 1);
    caught = false;
    try { l.Append(src + 10, src + 7, SForwardIteratorTag()); } catch (int) { caught = true; }
    CHECK(caught && g_lengthErrors == 2);
    CHECK(l.m_start == l0 && l.m_finish == l0 + 2 && strcmp(l.m_start, "ab") == 0);
    if (fails == 0) printf("00567d60_TEST PASS\n"); else printf("00567d60_TEST FAIL\n");
    return fails;
}