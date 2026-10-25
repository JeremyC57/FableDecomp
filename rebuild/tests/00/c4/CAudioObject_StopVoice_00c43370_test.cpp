// Behaviour test for 00c43370: CAudioObject::StopVoice -- forwards the 64-bit time to the voice's slot-12 virtual
#include <cstdio>
static unsigned __int64 g_time = 0; static int g_calls = 0; static const void* g_voice = 0;
class CVoice {
public:
    virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { } virtual void v05() { }
    virtual void v06() { } virtual void v07() { } virtual void v08() { } virtual void v09() { } virtual void v10() { } virtual void v11() { }
    virtual void StopAt(unsigned __int64 time) { ++g_calls; g_time = time; g_voice = this; }
};
class CAudioObject {
public:
    void* m_vptr; unsigned char m_pad04[0x50 - 4]; CVoice* m_pVoice;
    void StopVoice(unsigned __int64 time) { m_pVoice->StopAt(time); }
};
int main()
{
    CVoice voice; CAudioObject o; o.m_pVoice = &voice;
    o.StopVoice(0x0000000700000003ULL);
    int ok = g_calls == 1 && g_time == 0x0000000700000003ULL && g_voice == &voice;
    std::printf(ok ? "00c43370_TEST PASS\n" : "00c43370_TEST FAIL\n");
    return ok ? 0 : 1;
}
