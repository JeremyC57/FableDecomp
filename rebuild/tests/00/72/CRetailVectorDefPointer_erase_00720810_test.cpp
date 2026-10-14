#include <cstdio>
struct random_access_iterator_tag { random_access_iterator_tag() { } };
static int g_destroyed = 0;
struct CDefPointeeBase { virtual void v00() { } virtual void Destroy() { ++g_destroyed; } unsigned long RefCount; void Release() { if (--RefCount == 0) Destroy(); } };
struct CDefPointer { CDefPointeeBase* Object; ~CDefPointer() { if (Object) { Object->Release(); Object = 0; } } };
static int g_copied = 0;
CDefPointer* __fastcall copy_sgi(CDefPointer* first, CDefPointer* last, CDefPointer* result, const random_access_iterator_tag&, long*)
{ for (; first != last; ++first, ++result) { result->Object = first->Object; ++g_copied; } return result; }
struct CRetailVector_DefPointer {
    CDefPointer* start; CDefPointer* finish; CDefPointer* end_of_storage;
    CDefPointer* erase(CDefPointer* pos) { if (pos + 1 != finish) copy_sgi(pos + 1, finish, pos, random_access_iterator_tag(), (long*)0); --finish; finish->~CDefPointer(); return pos; }
};
int main()
{
    CDefPointeeBase a, b, c; a.RefCount = 1; b.RefCount = 2; c.RefCount = 1;
    CDefPointer e[3] = { { &a }, { &b }, { &c } };
    CRetailVector_DefPointer v = { e, e + 3, e + 3 };
    CDefPointer* r = v.erase(e + 1);        // drops b: c moves down, the stale last slot (c) is released
    if (r != e + 1 || v.finish != e + 2 || g_copied != 1 || e[0].Object != &a || e[1].Object != &c || e[2].Object != 0 || c.RefCount != 0 || g_destroyed != 1) { std::printf("00720810_TEST FAIL\n"); return 1; }
    v.erase(e + 1);                          // last element (c again, count already 0 -> wraps; use a fresh count)
    if (v.finish != e + 1 || g_copied != 1) { std::printf("00720810_TEST FAIL\n"); return 1; }
    std::printf("00720810_TEST PASS\n"); return 0;
}