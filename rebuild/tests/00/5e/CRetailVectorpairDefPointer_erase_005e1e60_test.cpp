#include <cstdio>
struct random_access_iterator_tag { random_access_iterator_tag() { } };
static int g_destroyed = 0;
struct CDefPointeeBase { virtual void v00() { } virtual void Destroy() { ++g_destroyed; } unsigned long RefCount; void Release() { if (--RefCount == 0) Destroy(); } };
struct CDefPointer { CDefPointeeBase* Object; ~CDefPointer() { if (Object) { Object->Release(); Object = 0; } } };
struct SPairDefPointer { long first; CDefPointer second; };
static int g_copied = 0;
SPairDefPointer* __fastcall copy_sgi(SPairDefPointer* first, SPairDefPointer* last, SPairDefPointer* result, const random_access_iterator_tag&, long*)
{ for (; first != last; ++first, ++result) { result->first = first->first; result->second.Object = first->second.Object; ++g_copied; } return result; }
struct CRetailVector_pairDefPointer {
    SPairDefPointer* start; SPairDefPointer* finish; SPairDefPointer* end_of_storage;
    SPairDefPointer* erase(SPairDefPointer* pos) { if (pos + 1 != finish) copy_sgi(pos + 1, finish, pos, random_access_iterator_tag(), (long*)0); --finish; finish->~SPairDefPointer(); return pos; }
};
int main()
{
    CDefPointeeBase a, b, c; a.RefCount = 1; b.RefCount = 2; c.RefCount = 1;
    SPairDefPointer e[3] = { { 1, { &a } }, { 2, { &b } }, { 3, { &c } } };
    CRetailVector_pairDefPointer v = { e, e + 3, e + 3 };
    SPairDefPointer* r = v.erase(e);        // drops {1,a}: the tail moves down, the stale last slot (now c) is released
    if (r != e || v.finish != e + 2 || g_copied != 2 || e[0].first != 2 || e[0].second.Object != &b || e[1].first != 3 || e[2].second.Object != 0 || c.RefCount != 0 || g_destroyed != 1) { std::printf("005e1e60_TEST FAIL\n"); return 1; }
    v.erase(e + 1);                          // last element: no copy, {3,c} slot already null, nothing released
    if (v.finish != e + 1 || g_copied != 2 || g_destroyed != 1) { std::printf("005e1e60_TEST FAIL\n"); return 1; }
    std::printf("005e1e60_TEST PASS\n"); return 0;
}