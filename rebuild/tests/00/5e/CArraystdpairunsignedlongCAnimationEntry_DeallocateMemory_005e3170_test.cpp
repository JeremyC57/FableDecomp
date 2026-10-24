#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#endif
static int g_freed = 0;
static int g_destroyed = 0;
static void TestFree(void* p) { ++g_freed; std::free(p); }

struct SAnimationPair {                         // std::pair<unsigned long, CAnimationEntry>, 0x14 bytes
    unsigned long first;
    long          second[4];                    // CAnimationEntry (four words, landed 0x005DEDC0)
};
struct SCopyTag { SCopyTag() { } };
struct __false_type { __false_type() { } };

SAnimationPair* __fastcall CopyDown(SAnimationPair* first, SAnimationPair* last, SAnimationPair* result, const SCopyTag& tag, long* distance);   // 0x005E18B0
void __fastcall DestroyRange(SAnimationPair* first, SAnimationPair* last, const __false_type& tag);   // 0x00454404

template<class T> struct CArray {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long capacity() const { return (unsigned long)(_M_end_of_storage - _M_start); }
    void erase(T* f, T* l)
    {
        T* i = CopyDown(l, _M_finish, f, SCopyTag(), (long*)0);
        DestroyRange(i, _M_finish, __false_type());
        _M_finish = i;
    }
    void DeallocateMemory();
};

template<class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }

template<> void CArray<SAnimationPair>::DeallocateMemory()
{
    if (capacity() > 0) {
        erase(_M_start, _M_finish);
        SAnimationPair* p = Exchange(_M_start, (SAnimationPair*)0);
        SAnimationPair* l = Exchange(_M_finish, (SAnimationPair*)0);
        _M_end_of_storage = 0;
        DestroyRange(p, l, __false_type());
        if (p)
            TestFree(p);
    }
}


SAnimationPair* __fastcall CopyDown(SAnimationPair* first, SAnimationPair* last, SAnimationPair* result, const SCopyTag& tag, long* distance)
{
    for (; first != last; ++first, ++result)
        *result = *first;
    return result;
}
void __fastcall DestroyRange(SAnimationPair* first, SAnimationPair* last, const __false_type& tag) { g_destroyed += (int)(last - first); }
int main()
{
    CArray<SAnimationPair> a;
    a._M_start = (SAnimationPair*)std::malloc(5 * sizeof(SAnimationPair));
    a._M_finish = a._M_start + 3;
    a._M_end_of_storage = a._M_start + 5;
    a.DeallocateMemory();
    if (a._M_start != 0 || a._M_finish != 0 || a._M_end_of_storage != 0 || g_freed != 1 || g_destroyed != 3) { std::printf("005e3170_TEST FAIL\n"); return 1; }
    a.DeallocateMemory();                       // no capacity: nothing to do
    if (g_freed != 1 || g_destroyed != 3) { std::printf("005e3170_TEST FAIL\n"); return 1; }
    std::printf("005e3170_TEST PASS\n");
    return 0;
}