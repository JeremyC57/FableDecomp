// SendControlKeys(unsigned int vk, unsigned int num) -- DXUT ImeUi.cpp: send `num` WM_KEYDOWNs of vk to the current window, then one WM_KEYUP
// Grade: DIFFER(84v83) -- LTCG custom convention: retail takes num in EAX (`mov esi,eax`) and vk in EDI (unsaved); ours takes vk/num in ecx/edx and saves registers
// Leaf: ?SendControlKeys@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB26B0, 83 bytes. File-static in ImeUi.cpp (FableWin.pdb
// "SendControlKeys"); callers 0x00CB273E / 0x00CB27C4 / 0x00CB27DC (CancelCompString) and 0x00CB4CA9. The manifest
// label "ImeUi_SendCompositionEvents" is descriptive.
// Shape: the DXUT body with the inline SendKeyMsg of this SDK drop (lParam 0); the `= false` stores inside the loop
// are dead and only the last one survives.
struct HWND__ { int unused; };   typedef HWND__* HWND;

extern long (__stdcall* _SendMessage)(HWND hwnd, unsigned int msg, unsigned int wp, long lp);   // 0x0143E8C4
extern bool bIsSendingKeyMessage;                   // 0x0143E8AD
extern HWND g_hwndCurr;                             // 0x0143CF0C

inline long SendKeyMsg(HWND hwnd, unsigned int msg, unsigned int wp)
{
    bIsSendingKeyMessage = true;
    long lRc = _SendMessage(hwnd, msg, wp, 0);
    bIsSendingKeyMessage = false;
    return lRc;
}
#define SendKeyMsg_DOWN(hwnd,vk)    SendKeyMsg(hwnd, 0x0100 /* WM_KEYDOWN */, vk)
#define SendKeyMsg_UP(hwnd,vk)      SendKeyMsg(hwnd, 0x0101 /* WM_KEYUP */, vk)

void __fastcall SendControlKeys(unsigned int vk, unsigned int num)
{
    if (num == 0)
        return;
    for (unsigned int i = 0; i < num; i++)
    {
        SendKeyMsg_DOWN(g_hwndCurr, vk);
    }
    SendKeyMsg_UP(g_hwndCurr, vk);
}
