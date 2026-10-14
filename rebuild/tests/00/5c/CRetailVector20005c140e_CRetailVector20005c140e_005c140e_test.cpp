#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SScalarTag { SScalarTag() { } };
struct SElement20_005c140e { unsigned char _body[20]; };
static SElement20_005c140e g_store[4]; static int g_init = -1; static int g_copied = 0;
static SElement20_005c140e* UninitCopy_005c140e(SElement20_005c140e* first, SElement20_005c140e* last, SElement20_005c140e* dest, const SScalarTag&) { while (first != last) { *dest++ = *first++; ++g_copied; } return dest; }
struct CRetailVector20_005c140e { SElement20_005c140e* m_pFirst; SElement20_005c140e* m_pLast; SElement20_005c140e* m_pEnd;
    void Initialize(long count, const SScalarTag&) { g_init = (int)count; m_pFirst = g_store; m_pLast = g_store; m_pEnd = g_store + count; }
    CRetailVector20_005c140e() { }
    CRetailVector20_005c140e(const CRetailVector20_005c140e& other) { Initialize((long)((char*)other.m_pLast - (char*)other.m_pFirst) / 20, SScalarTag()); m_pLast = UninitCopy_005c140e(other.m_pFirst, other.m_pLast, m_pFirst, SScalarTag()); } };
int main() {
    SElement20_005c140e src[3]; std::memset(src, 7, sizeof src); src[2]._body[3] = 9; CRetailVector20_005c140e v; v.m_pFirst = src; v.m_pLast = src + 3; v.m_pEnd = src + 3;
    CRetailVector20_005c140e c(v); if (g_init != 3 || g_copied != 3 || c.m_pFirst != g_store || c.m_pLast != g_store + 3 || g_store[2]._body[3] != 9 || g_store[0]._body[0] != 7) { std::printf("005c140e_TEST FAIL\n"); return 1; }
    std::printf("005c140e_TEST PASS\n"); return 0;
}