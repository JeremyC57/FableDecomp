struct CGameScriptInterface;
extern unsigned char g_countBetMoneyDown;

void __fastcall CGameScriptInterface_SetCountBetMoneyDown(CGameScriptInterface* self, void* edx, bool count)
{
    (void)self;
    (void)edx;
    g_countBetMoneyDown = (unsigned char)count;
}