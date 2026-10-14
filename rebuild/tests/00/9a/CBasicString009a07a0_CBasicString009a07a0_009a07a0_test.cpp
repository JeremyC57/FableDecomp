#include <cstdio>
#include <new>
static unsigned long g_count = 0; static char g_fill = 0;
#pragma pack(push, 1)
struct CBasicString { char* m_pData; unsigned long m_length; unsigned long m_capacity; unsigned char m_flags; long m_references; void SetString(unsigned long c, char f) { g_count = c; g_fill = f; }
    CBasicString(unsigned long c, char f) { m_capacity &= 0x80000000; m_pData = 0; m_length = 0; m_flags |= 1; SetString(c, f); m_references = 1; } };
#pragma pack(pop)
int main() { unsigned char raw[sizeof(CBasicString)]; for (unsigned i = 0; i < sizeof raw; ++i) raw[i] = 0xff; CBasicString* s = new (raw) CBasicString(5, 'x');
    if (s->m_pData != 0 || s->m_length != 0 || s->m_capacity != 0x80000000 || s->m_flags != 0xff || s->m_references != 1 || g_count != 5 || g_fill != 'x') { std::printf("009a07a0_TEST FAIL\n"); return 1; } std::printf("009a07a0_TEST PASS\n"); return 0; }