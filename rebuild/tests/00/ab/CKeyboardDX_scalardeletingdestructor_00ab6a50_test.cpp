#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[128] = ""; struct IDevice { virtual unsigned long Release() { std::strcat(g_seq, "R"); return 0; } virtual long Acquire() { std::strcat(g_seq, "A"); return 0; } virtual long Unacquire() { std::strcat(g_seq, "U"); return 0; } };
struct CKeyboard { virtual ~CKeyboard() { std::strcat(g_seq, "K"); } unsigned char _body[0x3414]; };
struct CKeyboardDX : CKeyboard { IDevice* DKeyboard; unsigned char KeyStates[0x100]; virtual ~CKeyboardDX() { DKeyboard->Unacquire(); DKeyboard->Release(); } };

int main()
{
    IDevice dev; CKeyboardDX* k = new CKeyboardDX; k->DKeyboard = &dev; CKeyboard* base = k; delete base; if (std::strcmp(g_seq, "URK")) { std::printf("00ab6a50_TEST FAIL\n"); return 1; }
    std::printf("00ab6a50_TEST PASS\n");
    return 0;
}