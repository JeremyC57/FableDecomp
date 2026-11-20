// CSoundPlayParameters::CSoundPlayParameters() -- constructor: initialises through the out-of-line Initialise @ 0x00C1BC90
// Leaf: ??0CSoundPlayParameters@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1BD10, oracle length 12 bytes.
// Retail bytes:  push esi ; mov esi, ecx ; call 0xc1bc90 ; mov eax, esi ; pop esi ; ret
// Identity (descriptive; libpk has no PDB): the 0x14-byte sound play-parameter record's default constructor. Its callers
//   construct a CSoundPlayParameters (CStreamLoadHandler 0x00C267E0 binds ??0CSoundPlayParameters@@QAE@XZ; StartEvent
//   0x00C12940 builds its local the same way and calls Initialise 0x00C1BC90 itself when no source block is given, binding
//   ?Initialise@CSoundPlayParameters@@QAEXXZ). The catalog already names it so; CClass_00c1bd10 was a placeholder.
class CSoundPlayParameters
{ public:
    unsigned char _body[0x14];
    void Initialise();                     // 0x00C1BC90
    CSoundPlayParameters();
};

CSoundPlayParameters::CSoundPlayParameters()
{
    Initialise();
}

#include <cstdio>
static int g_n = 0;
void CSoundPlayParameters::Initialise() { ++g_n; _body[0] = 0x5a; }
int main()
{
    CSoundPlayParameters p; if (g_n != 1 || p._body[0] != 0x5a) { std::printf("00c1bd10_TEST FAIL\n"); return 1; }

    std::printf("00c1bd10_TEST PASS\n");
    return 0;
}
