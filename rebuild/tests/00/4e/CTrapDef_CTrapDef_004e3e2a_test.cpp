#include <cstdio>
#include <cstring>
struct CDefBase { virtual void v00() {} unsigned char _body[0x24]; CDefBase() {} };
struct CTrapTrigger { unsigned char _body[0x18]; CTrapTrigger() { std::memset(_body, 0x11, sizeof(_body)); } };
struct CTrapDef : CDefBase { CTrapTrigger m_trigger; long m_trapIndex; virtual void v00() {} CTrapDef(); };
CTrapDef::CTrapDef() : CDefBase(), m_trigger() { m_trapIndex = -1; }
int main()
{
    CTrapDef d;
    if (d.m_trapIndex != -1 || d.m_trigger._body[0] != 0x11 || d.m_trigger._body[0x17] != 0x11) { std::printf("004e3e2a_TEST FAIL\n"); return 1; }
    std::printf("004e3e2a_TEST PASS\n");
    return 0;
}