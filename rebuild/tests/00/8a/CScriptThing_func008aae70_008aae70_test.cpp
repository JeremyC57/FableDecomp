#include <cstdio>
#include <cstring>
// Behaviour of 0x008AAE70 (chunk 901 piece): returns the member at +0x38
struct CScriptThing_Owner_Retail {
    unsigned char _pad_0x00[0x38];
    long m_word38;
    long func_008aae70();
};
long CScriptThing_Owner_Retail::func_008aae70() { return m_word38; }
int main()
{
    CScriptThing_Owner_Retail o; std::memset(&o, 0xAB, sizeof o); o.m_word38 = 99;
    if (o.func_008aae70() != 99) { std::printf("008aae70_TEST FAIL\n"); return 1; }
    std::printf("008aae70_TEST PASS\n"); return 0;
}