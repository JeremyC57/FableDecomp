// CScriptThing::func_00ca5540() -- sleep for the +0xf0 interval (100 ns units -> ms), or yield with Sleep(0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA5540, length 44 bytes (48 with padding); no direct reference found.
// Part of the catalogue row 00ca5510 (skipped ledger, chunk 901); the manifest class is propagated, so `this` is
// modelled locally. Import: KERNEL32!Sleep (IAT 0x0143FE1C).
// Shape: the interval loaded into ecx and tested signed (`jle`), the signed divide by 10000 as the magic multiply
// 0x68DB8BAD + `sar edx,12` + sign fix-up, each arm its own `push ; call [Sleep] ; ret`.
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);

struct CLibraryObject_Retail {
    unsigned char _pad_0x00[0xf0];
    long          m_interval;                  // +0xf0 (100 ns units)
    void func_00ca5540();
};

void CLibraryObject_Retail::func_00ca5540()
{
    if (m_interval > 0)
        Sleep(m_interval / 10000);
    else
        Sleep(0);
}
