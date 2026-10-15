#include <cstdio>
#include <cstring>
// Behaviour of 0x008AAE60 (chunk 901 piece): returns the member at +0x7c
struct CScriptThing_Owner_Retail {
    unsigned char _pad_0x00[0x7c];
    long m_word7C;
    long func_008aae60();
};
long CScriptThing_Owner_Retail::func_008aae60() { return m_word7C; }
int main()
{
    CScriptThing_Owner_Retail o; std::memset(&o, 0xAB, sizeof o); o.m_word7C = -5;
    if (o.func_008aae60() != -5) { std::printf("008aae60_TEST FAIL\n"); return 1; }
    std::printf("008aae60_TEST PASS\n"); return 0;
}