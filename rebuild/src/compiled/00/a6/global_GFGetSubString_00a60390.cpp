// GFGetSubString(const char* text, const char* sub) -- naive substring search: the address of the first occurrence of sub inside text, or 0
// Leaf: ?GFGetSubString@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A60390, 83 bytes, plain `ret` (PDB ?GFGetSubString@@YIPADPBD0@Z:
//   `char* __fastcall`, text in ecx, sub in edx). No callees.
// Shape: two index loops: for every start position i while text[i] != 0, advance j while sub[j], text[i + j] and their
//   equality hold (VC rotates the test of sub[j] to the loop bottom), and return `text + i` once sub[j] reached its
//   terminator; the miss path returns 0 (`xor eax,eax` before the last pop).
char* __fastcall GFGetSubString(const char* text, const char* sub)
{
    for (long i = 0; text[i] != 0; ++i) {
        long j = 0;
        while (sub[j] != 0 && text[i + j] != 0 && text[i + j] == sub[j])
            ++j;
        if (sub[j] == 0)
            return (char*)&text[i];
    }
    return 0;
}