#include <cstdio>
#include <new>
static int g_lists = 0;
struct CMessageEventList { void* head; CMessageEventList() : head((void*)0x77) { ++g_lists; } };
struct CMessageListOwner { long m_state; short m_index; short m_count; CMessageEventList m_messages; CMessageListOwner() : m_state(0), m_index(0), m_count(0) {} };
int main()
{
    unsigned char raw[sizeof(CMessageListOwner)]; for (int i = 0; i < (int)sizeof raw; ++i) raw[i] = 0xee;
    CMessageListOwner* o = new (raw) CMessageListOwner();
    if (sizeof(CMessageListOwner) != 12 || o->m_state != 0 || o->m_index != 0 || o->m_count != 0 || o->m_messages.head != (void*)0x77 || g_lists != 1) { std::printf("00c203a0_TEST FAIL\n"); return 1; }
    std::printf("00c203a0_TEST PASS\n");
    return 0;
}