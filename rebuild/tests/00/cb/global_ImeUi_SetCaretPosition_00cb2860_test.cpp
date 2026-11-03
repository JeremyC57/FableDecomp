#include <cstdio>
// Links the source object: ImeUi_SetCaretPosition stores x / y into g_CaretInfo when g_bInitialized (DXUT ImeUi.cpp).
class CImeUiFont_Base;
struct tagRECT { long left, top, right, bottom; };
struct CompStringAttribute { unsigned int caretX, caretY; CImeUiFont_Base* pFont; unsigned long colorComp, colorCand; tagRECT margins; };
bool g_bInitialized;
CompStringAttribute g_CaretInfo;
void __fastcall ImeUi_SetCaretPosition(unsigned int x, unsigned int y);
int main()
{
    g_bInitialized = false; g_CaretInfo.caretX = 0; g_CaretInfo.caretY = 0;
    ImeUi_SetCaretPosition(0x12, 0x34);
    if (g_CaretInfo.caretX != 0 || g_CaretInfo.caretY != 0) { printf("FAIL disabled\n"); return 1; }
    g_bInitialized = true;
    ImeUi_SetCaretPosition(0x12, 0x34);
    if (g_CaretInfo.caretX != 0x12 || g_CaretInfo.caretY != 0x34) { printf("FAIL enabled\n"); return 2; }
    printf("OK_00cb2860\n");
    return 0;
}
