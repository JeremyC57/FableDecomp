// Test for 0x00C3F460 CCodaVoice::ClearField0C: the word is zeroed and 0 returned.
#include <cstdio>
struct CVoice { int m_0c; long ClearField0C(long) { m_0c = 0; return 0; } };
int main()
{
    CVoice v; v.m_0c = 42;
    if (v.ClearField0C(5) != 0 || v.m_0c != 0) { std::printf("00c3f460_TEST FAIL\n"); return 1; }
    std::printf("00c3f460_TEST PASS\n");
    return 0;
}