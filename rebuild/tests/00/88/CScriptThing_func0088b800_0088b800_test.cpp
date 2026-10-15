#include <cstdio>
#include <cstring>
// Behaviour of 0x0088B800 (chunk 901 piece): returns the member at +0x3c
struct CScriptThing_Host_Retail {
    unsigned char _pad_0x00[0x3c];
    long m_scriptWord3C;
    long func_0088b800();
};
long CScriptThing_Host_Retail::func_0088b800() { return m_scriptWord3C; }
int main()
{
    CScriptThing_Host_Retail o; std::memset(&o, 0xAB, sizeof o); o.m_scriptWord3C = 0x1234;
    if (o.func_0088b800() != 0x1234) { std::printf("0088b800_TEST FAIL\n"); return 1; }
    std::printf("0088b800_TEST PASS\n"); return 0;
}