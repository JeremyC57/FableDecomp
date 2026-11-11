// Behaviour test for CEnvelope_012b37dc::Release (0x00C35460): releasing the envelope runs the (virtual) Stop, which marks it done.
#include <stdio.h>
struct CEnvelope_012b37dc {
    long m_loopsLeft; unsigned long m_state;
    CEnvelope_012b37dc() : m_loopsLeft(5), m_state(1) { }
    virtual ~CEnvelope_012b37dc() { }
    virtual void Release();
    virtual void Stop() { m_state = 3; m_loopsLeft = 0; }
};
void CEnvelope_012b37dc::Release()
{
    Stop();
}
struct Counting : CEnvelope_012b37dc {
    int stops;
    Counting() : stops(0) { }
    virtual void Stop() { ++stops; }
};
int main(void)
{
    CEnvelope_012b37dc e; e.Release();
    Counting c; c.Release();
    if (e.m_state != 3 || e.m_loopsLeft != 0 || c.stops != 1 || c.m_state != 1) { printf("00c35460_TEST FAIL\n"); return 1; }
    printf("00c35460_TEST PASS\n");
    return 0;
}