// Test for 0x00A3E420 CSoundManagerLH::GetNextEmitterID_00a3e420: hands out the current emitter id and post-increments it.
#include <stdio.h>
class CSoundManagerLH {
public:
    int GetNextEmitterID_00a3e420() { return m_nextEmitterID++; }
    unsigned char _pad_0x00[0x50];
    int m_nextEmitterID;
};
int main()
{
    CSoundManagerLH m; m.m_nextEmitterID = 5;
    int a = m.GetNextEmitterID_00a3e420(); int b = m.GetNextEmitterID_00a3e420();
    bool ok = a == 5 && b == 6 && m.m_nextEmitterID == 7;
    puts(ok ? "00a3e420_TEST PASS" : "00a3e420_TEST FAIL");
    return ok ? 0 : 1;
}