#include <stdio.h>
// Links the source object: ImeUi_GetCaretStatus reads g_bInitialized and g_szCompositionString[0] (DXUT ImeUi.cpp).
bool g_bInitialized = false;
unsigned short g_szCompositionString[256];

bool __fastcall ImeUi_GetCaretStatus();

int main()
{
    int fails = 0;
    g_bInitialized = false; g_szCompositionString[0] = 0;
    if (ImeUi_GetCaretStatus() != true) fails++;
    g_bInitialized = true; g_szCompositionString[0] = 0;
    if (ImeUi_GetCaretStatus() != true) fails++;
    g_bInitialized = false; g_szCompositionString[0] = 5;
    if (ImeUi_GetCaretStatus() != true) fails++;
    g_bInitialized = true; g_szCompositionString[0] = 5;
    if (ImeUi_GetCaretStatus() != false) fails++;
    if (fails == 0) printf("MELEEWEAPONCLASS_OK\n");
    else printf("FAIL %d\n", fails);
    return fails == 0 ? 0 : 1;
}
