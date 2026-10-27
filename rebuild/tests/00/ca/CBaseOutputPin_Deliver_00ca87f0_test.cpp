#include <cstdio>
// CBaseOutputPin::Deliver: VFW_E_NOT_CONNECTED without an input pin, else m_pInputPin->Receive(pSample).
struct MemIn { void* last; virtual int Receive(void* s) { last = s; return 5; } };
struct OutPin { MemIn* m_pInputPin; int Deliver(void* s) { if (m_pInputPin == 0) return (int)0x80040209; return m_pInputPin->Receive(s); } };
int main()
{
    MemIn in; in.last = 0; OutPin p; int sample;
    p.m_pInputPin = 0;
    if (p.Deliver(&sample) != (int)0x80040209) { std::printf("00ca87f0_TEST FAIL\n"); return 1; }
    p.m_pInputPin = &in;
    if (p.Deliver(&sample) != 5 || in.last != &sample) { std::printf("00ca87f0_TEST FAIL\n"); return 1; }
    std::printf("00ca87f0_TEST PASS\n");
    return 0;
}