#include <cstdio>
// Behaviour model of 0x00C31990: first word cleared, argument kept.
struct CValueHolder {
    void* m_pFirst; void* m_pValue;
    CValueHolder(void* value) : m_pFirst(0), m_pValue(value) {}
};
int main()
{
    int v = 3;
    CValueHolder h(&v);
    if (h.m_pFirst != 0 || h.m_pValue != &v) { std::printf("00c31990_TEST FAIL\n"); return 1; }
    std::printf("00c31990_TEST PASS\n");
    return 0;
}