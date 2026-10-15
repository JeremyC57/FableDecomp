#include <cstdio>
#include <cstring>
#include <new>
struct SSettings_0041c930 {
    float m_scale0, m_scale1;
    long m_8, m_c, m_10, m_14, m_18, m_id;
    SSettings_0041c930() : m_scale0(1.0f), m_scale1(1.0f), m_8(0), m_c(1), m_14(0), m_18(0), m_id(-1) { m_10 = 1; }
};
int main()
{
    static unsigned char buf[sizeof(SSettings_0041c930)];
    std::memset(buf, 0xCD, sizeof(buf));
    SSettings_0041c930* s = new (buf) SSettings_0041c930();
    bool ok = s->m_scale0 == 1.0f && s->m_scale1 == 1.0f && s->m_8 == 0 && s->m_c == 1 && s->m_10 == 1
        && s->m_14 == 0 && s->m_18 == 0 && s->m_id == -1;
    if (!ok) { std::printf("0041c930_TEST FAIL\n"); return 1; }
    std::printf("0041c930_TEST PASS\n");
    return 0;
}
