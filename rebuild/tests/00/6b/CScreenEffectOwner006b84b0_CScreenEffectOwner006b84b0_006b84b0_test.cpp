#include <cstdio>
#include <cstring>
static const bool g_is32 = sizeof(void*) == 4 && sizeof(long) == 4;   // the layout checks hold for the 32-bit VC7.1 build
static char g_seq[32] = "";
struct CScreenEffectPriorityStack { unsigned char _body[0x10]; CScreenEffectPriorityStack() { std::strcat(g_seq, "S"); } };
struct CScreenEffectOwner { void* m_pOwner; long m_id; unsigned char _pad08[0x70]; bool m_active; bool m_dirty; unsigned char _pad7a[0x16]; CScreenEffectPriorityStack m_priorityStack;
    CScreenEffectOwner(long id, void* pOwner) : m_id(id), m_priorityStack() { m_active = false; m_dirty = false; m_pOwner = pOwner; } };
int main()
{
    int owner; CScreenEffectOwner o(42, &owner);
    if (std::strcmp(g_seq, "S") || o.m_pOwner != &owner || o.m_id != 42 || o.m_active || o.m_dirty || (g_is32 && (char*)&o.m_priorityStack - (char*)&o != 0x90) || (g_is32 && (char*)&o.m_active - (char*)&o != 0x78)) { std::printf("006b84b0_TEST FAIL\n"); return 1; }
    std::printf("006b84b0_TEST PASS\n");
    return 0;
}