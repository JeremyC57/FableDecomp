#include <cstdio>
static int g_baseCtors = 0, g_listCtors = 0; static void* g_lastOwner = 0;
class CThing;
struct CRetailAllocator { CRetailAllocator() {} };
struct CThingComponent { virtual void v00() {} unsigned char _pad_0x04[8]; CThingComponent(CThing* owner) { ++g_baseCtors; g_lastOwner = owner; } };
struct CRetailList { void* head; CRetailList(const CRetailAllocator& al = CRetailAllocator()) : head(&head) { ++g_listCtors; } };
class CTCParticleAttachmentTracker : public CThingComponent { public: virtual void v00() {} CRetailList m_list;  CTCParticleAttachmentTracker(CThing* owner) : CThingComponent(owner), m_list() {} };

int main()
{
    int owner; CTCParticleAttachmentTracker c((CThing*)&owner);
    if (g_baseCtors != 1 || g_listCtors != 1 || g_lastOwner != &owner || c.m_list.head != &c.m_list.head) { std::printf("004ded6f_TEST FAIL\n"); return 1; }
    if ((unsigned char*)&c.m_list - (unsigned char*)&c != 0xc) { std::printf("004ded6f_TEST FAIL\n"); return 1; }
    std::printf("004ded6f_TEST PASS\n");
    return 0;
}