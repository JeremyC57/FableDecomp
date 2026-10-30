# sub-wave chunk (hand): the ImeUi defining rows (DXUT ImeUi.cpp linked into Fable.exe) the NUISystem::CIME rows call. Each body keeps
# its bytes; only the function's name / signature change to the Ego_r.pdb + FableWin.pdb spelling (two lists each, below).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import _rw, _w
def put(a, k, text):
    p, rel, t = _rw(a, k)
    if t == text: print('already', a, k); return
    _w(p, text); print('ok', a, k)
# ---- 0x00CB28E0 ?ImeUi_IsEnabled@@YI_NXZ (Ego_r.pdb 0x00C92E80 + FableWin.pdb 0x0357EE10): its only retail caller is
#      NUISystem::CIME::IsIMEActive 0x00404C50 (`call 0x00CB28E0`); the row's CAIStateGroup_Freeze::IsRunnable spelling is a label
#      propagated across the byte-identical body of the real IsRunnable 0x009224E0 (which defines that symbol too).
put('00cb28e0', 'src', '''// ImeUi_IsEnabled() -- returns the IME-enabled flag byte @ 0x0143E890
// Leaf: ?ImeUi_IsEnabled@@YI_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB28E0, oracle length 6 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov al, byte ptr [0x143e890] ; ret
// Identity (naming wave 11): Ego_r.pdb 0x00C92E80 + FableWin.pdb 0x0357EE10 ?ImeUi_IsEnabled@@YI_NXZ (DXUT ImeUi.cpp, static
// __fastcall here as in both PDBs); the only retail caller is NUISystem::CIME::IsIMEActive 0x00404C50. It was landed as
// CAIStateGroup_Freeze::IsRunnable, a label propagated from the byte-identical body at 0x009224E0.
extern unsigned char g_flag_0143e890;         // 0x0143E890 (the IME-enabled flag, a bool)
bool __fastcall ImeUi_IsEnabled()
{
    return *(bool*)&g_flag_0143e890;
}
''')
# ---- 0x00CB2860 ?ImeUi_SetCaretPosition@@YIXII@Z (Ego_r.pdb 0x00C92CE0 + FableWin.pdb 0x0357E930): its only retail caller is
#      NUISystem::CIME::SetEditBoxCaretPos 0x00404B80 (`call 0x00CB2860`, x in ecx / y in edx); the CDrawMouseCursor::Draw label was
#      propagated. The data words keep their bound spellings (no PDB data names for ImeUi.cpp's statics).
put('00cb2860', 'src', '''// ImeUi_SetCaretPosition(unsigned int, unsigned int) -- stores the caret position when ImeUi is initialised
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
''')
# ---- 0x00CB2EC0 ?ImeUi_GetCaretStatus@@YI_NXZ (Ego_r.pdb 0x00C94CD0 + FableWin.pdb 0x035814B0): NUISystem::CIME::ShouldEditBoxShowCaret
#      0x00404BA0 returns it when the IME is initialised; the CTCWeapon::IsMeleeWeaponClass label was propagated (the real one is
#      0x005DA510). `return !(a && b)` gives retail's xor eax,eax / mov eax,1 (an if/return-false form gives xor al,al / mov al,1).
put('00cb2ec0', 'src', '''// ImeUi_GetCaretStatus() -- true unless ImeUi is initialised and the composition string is non-empty
// Leaf: ?ImeUi_GetCaretStatus@@YI_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB2EC0, oracle length 28 bytes.
// Retail bytes:  mov al,[0x143e8ae] ; test al,al ; je L1 ; cmp word ptr [0x143cf10],0 ; je L1 ; xor eax,eax ; ret ; L1: mov eax,1 ; ret
// Identity (naming wave 11): Ego_r.pdb 0x00C94CD0 + FableWin.pdb 0x035814B0 ?ImeUi_GetCaretStatus@@YI_NXZ (DXUT ImeUi.cpp);
// NUISystem::CIME::ShouldEditBoxShowCaret 0x00404BA0 returns it. It was landed as IsMeleeWeaponClass (a propagated label; the
// real CTCWeapon::IsMeleeWeaponClass is 0x005DA510). The byte @ 0x0143E8AE is ImeUi's initialised flag, the word @ 0x0143CF10
// the first character of the wide composition string.
extern unsigned char g_flagByte;   // byte ptr [0x143e8ae]
extern unsigned short g_flagWord;  // word ptr [0x143cf10]
bool __fastcall ImeUi_GetCaretStatus()
{
    return !(g_flagByte && g_flagWord);
}
''')
put('00cb2ec0', 'tst', '''#include <stdio.h>

unsigned char g_flagByte = 0;
unsigned short g_flagWord = 0;

bool __fastcall ImeUi_GetCaretStatus();

int main()
{
    int fails = 0;

    // both zero -> true
    g_flagByte = 0; g_flagWord = 0;
    if (ImeUi_GetCaretStatus() != true) fails++;

    // byte set, word zero -> true
    g_flagByte = 1; g_flagWord = 0;
    if (ImeUi_GetCaretStatus() != true) fails++;

    // byte zero, word set -> true
    g_flagByte = 0; g_flagWord = 5;
    if (ImeUi_GetCaretStatus() != true) fails++;

    // both set -> false
    g_flagByte = 1; g_flagWord = 5;
    if (ImeUi_GetCaretStatus() != false) fails++;

    if (fails == 0) printf("MELEEWEAPONCLASS_OK\\n");
    else printf("FAIL %d\\n", fails);
    return fails == 0 ? 0 : 1;
}''')
