#include <cstdio>
static int g_baseCtors = 0, g_listCtors = 0; static void* g_lastOwner = 0;
struct CThing;
struct CRetailAllocator { CRetailAllocator() {} };
struct CThingComponent { virtual void v00() {} unsigned char _pad_0x04[8]; CThingComponent(CThing* owner) { ++g_baseCtors; g_lastOwner = owner; } };
struct CRetailList { void* head; CRetailList(const CRetailAllocator& al = CRetailAllocator()) : head(&head) { ++g_listCtors; } };
struct CTCThingOwner : CThingComponent { virtual void v00() {} CRetailList m_list; bool CallOnDie; CTCThingOwner(CThing* owner) : CThingComponent(owner), m_list(), CallOnDie(false) {} };

int main()
{
    int owner; CTCThingOwner c((CThing*)&owner);
    if (g_baseCtors != 1 || g_listCtors != 1 || g_lastOwner != &owner || c.m_list.head != &c.m_list.head || c.CallOnDie) { std::printf("004ddaed_TEST FAIL\n"); return 1; }
    if ((unsigned char*)&c.m_list - (unsigned char*)&c != 0xc) { std::printf("004ddaed_TEST FAIL\n"); return 1; }
    std::printf("004ddaed_TEST PASS\n");
    return 0;
}