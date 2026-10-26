#include <cstdio>
#include <cstdlib>
static int g_strings = 0, g_frees = 0, g_baseDtors = 0, g_deletes = 0;
class CThing;
class CCharString { public: void* m_pStringData; ~CCharString() { ++g_strings; } };
struct SContainerItem { long first; CCharString second; };
struct CRetailVector_ContainerItem {
    SContainerItem* Start; SContainerItem* Finish; SContainerItem* EndOfStorage;
    static void Destroy(SContainerItem* first, SContainerItem* last) { for (; first != last; ++first) first->~SContainerItem(); }
    ~CRetailVector_ContainerItem() { Destroy(Start, Finish); if (Start) { ++g_frees; std::free(Start); } }
};
class CTCBase { public: CThing* ParentThing; ~CTCBase() { ++g_baseDtors; } };
class CTCContainer { public:
    CTCBase Base; CRetailVector_ContainerItem ContainerItems;
    void* operator new(size_t n) { return std::malloc(n); }
    void operator delete(void* p) { ++g_deletes; std::free(p); }
};
int main()
{
    CTCContainer* c = new CTCContainer;
    c->Base.ParentThing = 0;
    c->ContainerItems.Start = (SContainerItem*)std::malloc(4 * sizeof(SContainerItem));
    c->ContainerItems.Finish = c->ContainerItems.Start + 3;
    c->ContainerItems.EndOfStorage = c->ContainerItems.Start + 4;
    delete c;
    if (g_strings != 3 || g_frees != 1 || g_baseDtors != 1 || g_deletes != 1) { std::printf("0074fde0_TEST FAIL\n"); return 1; }
    CTCContainer* e = new CTCContainer;                    /* empty vector: no free */
    e->ContainerItems.Start = 0; e->ContainerItems.Finish = 0; e->ContainerItems.EndOfStorage = 0;
    delete e;
    if (g_strings != 3 || g_frees != 1 || g_baseDtors != 2 || g_deletes != 2) { std::printf("0074fde0_TEST FAIL\n"); return 1; }
    std::printf("0074fde0_TEST PASS\n");
    return 0;
}