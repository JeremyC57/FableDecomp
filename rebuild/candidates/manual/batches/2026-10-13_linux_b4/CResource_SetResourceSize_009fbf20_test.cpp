#include <cstdio>

struct List { unsigned char pad[0x24]; unsigned long Allocated; };
struct R { void* vt; void* p; List* ResourceList; void* a; void* b; unsigned long ResourceSize;
    void Set(unsigned long s) { List* l = ResourceList; if (l) { l->Allocated -= ResourceSize; ResourceList->Allocated += s; } ResourceSize = s; } };
int main()
{
    List l; l.Allocated = 100; R r; r.ResourceList = &l; r.ResourceSize = 30; r.Set(50); if (l.Allocated != 120 || r.ResourceSize != 50) { std::printf("009fbf20_TEST FAIL\n"); return 1; }
    r.ResourceList = 0; r.Set(7); if (l.Allocated != 120 || r.ResourceSize != 7) { std::printf("009fbf20_TEST FAIL\n"); return 1; }
    std::printf("009fbf20_TEST PASS\n");
    return 0;
}
