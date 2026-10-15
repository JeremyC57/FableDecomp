#include <cstdio>
#include <cstring>
// Behaviour of 0x0088B7E0 (chunk 901 piece): 0 without a script, else the script's slot-6 priority
struct CScript_Retail {
    virtual long v00() { return 0; }
    virtual long v01() { return 1; }
    virtual long v02() { return 2; }
    virtual long v03() { return 3; }
    virtual long v04() { return 4; }
    virtual long v05() { return 5; }
    virtual long GetPriority() { return 77; }
};
struct CScriptThing_Host_Retail {
    unsigned char _pad_0x00[0x38];
    CScript_Retail* m_pCurrentScript;
    long m_scriptWord3C;
    long GetCurrentScriptPriority();
};
long CScriptThing_Host_Retail::GetCurrentScriptPriority() { if (m_pCurrentScript) return m_pCurrentScript->GetPriority(); return 0; }
int main()
{
    CScript_Retail script; CScriptThing_Host_Retail h; h.m_pCurrentScript = 0;
    long a = h.GetCurrentScriptPriority(); h.m_pCurrentScript = &script; long b = h.GetCurrentScriptPriority();
    if (a != 0 || b != 77) { std::printf("0088b7e0_TEST FAIL\n"); return 1; }
    std::printf("0088b7e0_TEST PASS\n"); return 0;
}
