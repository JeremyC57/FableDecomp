// ImeUi_SetCaretPosition(unsigned int, unsigned int) -- stores the caret position when ImeUi is initialised
// Leaf: ?ImeUi_SetCaretPosition@@YIXII@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB2860, oracle length 22 bytes.
// Retail bytes:  mov al,[0x143e8ae] ; test al,al ; je ret ; mov [0x143cd2c],ecx ; mov [0x143cd30],edx ; ret
// Identity (naming wave 11): Ego_r.pdb 0x00C92CE0 + FableWin.pdb 0x0357E930 ?ImeUi_SetCaretPosition@@YIXII@Z (DXUT ImeUi.cpp);
// the only retail caller is NUISystem::CIME::SetEditBoxCaretPos 0x00404B80. It was landed as CDrawMouseCursor::Draw (a
// propagated label). The flag @ 0x0143E8AE is ImeUi's g_bInitialized; 0x0143CD2C / 0x0143CD30 are g_CaretInfo.caretX / caretY
// (DXUT ImeUi.cpp CompStringAttribute, the layout landed 0x00CB2880 uses; Codex pass 12 respell 2026-11-03, was g_cursorThis /
// g_cursorArg / g_cursorEnabled).
class CImeUiFont_Base;
struct tagRECT { long left, top, right, bottom; };
struct CompStringAttribute {                        // ImeUi.cpp
    unsigned int     caretX;                        // +0x00
    unsigned int     caretY;                        // +0x04
    CImeUiFont_Base* pFont;                         // +0x08
    unsigned long    colorComp;                     // +0x0c
    unsigned long    colorCand;                     // +0x10
    tagRECT          margins;                       // +0x14
};
extern bool                g_bInitialized;          // 0x0143E8AE
extern CompStringAttribute g_CaretInfo;             // 0x0143CD2C
void __fastcall ImeUi_SetCaretPosition(unsigned int x, unsigned int y)
{
    if (!g_bInitialized)
        return;
    g_CaretInfo.caretX = x;
    g_CaretInfo.caretY = y;
}
