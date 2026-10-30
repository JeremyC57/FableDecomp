// ImeUi_SetCaretPosition(unsigned int, unsigned int) -- stores the caret position when ImeUi is initialised
// Leaf: ?ImeUi_SetCaretPosition@@YIXII@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB2860, oracle length 22 bytes.
// Retail bytes:  mov al,[0x143e8ae] ; test al,al ; je ret ; mov [0x143cd2c],ecx ; mov [0x143cd30],edx ; ret
// Identity (naming wave 11): Ego_r.pdb 0x00C92CE0 + FableWin.pdb 0x0357E930 ?ImeUi_SetCaretPosition@@YIXII@Z (DXUT ImeUi.cpp);
// the only retail caller is NUISystem::CIME::SetEditBoxCaretPos 0x00404B80. It was landed as CDrawMouseCursor::Draw (a
// propagated label). The flag @ 0x0143E8AE is ImeUi's initialised byte; the two words @ 0x0143CD2C / 0x0143CD30 the caret x / y.
extern unsigned char g_cursorEnabled;
extern void* g_cursorThis;
extern void* g_cursorArg;
unsigned char g_cursorEnabled;
void* g_cursorThis;
void* g_cursorArg;
void __fastcall ImeUi_SetCaretPosition(unsigned int x, unsigned int y)
{
    if (g_cursorEnabled) {
        g_cursorThis = (void*)x;
        g_cursorArg = (void*)y;
    }
}
