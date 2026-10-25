#include <cstdio>
class CLoadLog {
public:
    virtual void v00() { }
    unsigned char _pad04[0x2c];
    bool m_ok;
    bool IsOk() const { return m_ok; }
};
int main()
{
    CLoadLog log; log.m_ok = true;
    bool a = log.IsOk();
    log.m_ok = false;
    if (!a || log.IsOk() || (char*)&log.m_ok - (char*)&log != 0x30) { std::printf("00c307c0_TEST FAIL\n"); return 1; }
    std::printf("00c307c0_TEST PASS\n");
    return 0;
}
