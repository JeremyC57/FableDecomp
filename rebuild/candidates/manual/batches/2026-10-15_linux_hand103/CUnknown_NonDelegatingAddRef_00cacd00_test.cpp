#include <cstdio>
// Behaviour test for CUnknown::NonDelegatingAddRef (0x00CACD00): the model without the Win32 import.
static long TestInterlockedIncrement(long* addend) { return ++*addend; }
#define max(a, b) (((a) > (b)) ? (a) : (b))
struct CUnknown {
    void* m_pUnknown;
    long m_cRef;
    unsigned long NonDelegatingAddRef();
};
unsigned long CUnknown::NonDelegatingAddRef()
{
    TestInterlockedIncrement(&m_cRef);
    return max((unsigned long)m_cRef, 1ul);
}
int main()
{
    CUnknown u; u.m_pUnknown = 0; u.m_cRef = 0;
    if (u.NonDelegatingAddRef() != 1 || u.m_cRef != 1) { std::printf("00cacd00_TEST FAIL\n"); return 1; }
    if (u.NonDelegatingAddRef() != 2 || u.m_cRef != 2) { std::printf("00cacd00_TEST FAIL\n"); return 1; }
    u.m_cRef = -1;   // a count that reaches 0 clamps to 1
    if (u.NonDelegatingAddRef() != 1 || u.m_cRef != 0) { std::printf("00cacd00_TEST FAIL\n"); return 1; }
    std::printf("00cacd00_TEST PASS\n");
    return 0;
}
