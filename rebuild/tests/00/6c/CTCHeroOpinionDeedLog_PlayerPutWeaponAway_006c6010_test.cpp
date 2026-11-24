#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_n = 0; static long g_deed[4];
struct CDeedOwner_006c6010 { unsigned char _pad[0x28]; long m_deedIndex; void RemoveDeedStillSearchingForWitnesses(long d) { g_deed[g_n++] = d; } void DestroyDeed(long d) { g_deed[g_n++] = d; } void RemoveCrimeFromVillage(long d) { g_deed[g_n++] = d; }
    void ClearDeed() { if (m_deedIndex != -1) { RemoveDeedStillSearchingForWitnesses(m_deedIndex); DestroyDeed(m_deedIndex); RemoveCrimeFromVillage(m_deedIndex); m_deedIndex = -1; } } };
int main() {
    CDeedOwner_006c6010 d; d.m_deedIndex = -1; d.ClearDeed(); if (g_n) { std::printf("006c6010_TEST FAIL\n"); return 1; }
    d.m_deedIndex = 5; d.ClearDeed(); if (g_n != 3 || g_deed[0] != 5 || g_deed[2] != 5 || d.m_deedIndex != -1) { std::printf("006c6010_TEST FAIL\n"); return 1; }
    std::printf("006c6010_TEST PASS\n"); return 0;
}