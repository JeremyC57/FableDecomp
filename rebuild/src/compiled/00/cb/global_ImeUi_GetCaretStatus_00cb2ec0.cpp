// ImeUi_GetCaretStatus() -- true unless ImeUi is initialised and the composition string is non-empty
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
