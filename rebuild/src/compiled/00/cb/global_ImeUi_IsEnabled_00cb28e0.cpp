// ImeUi_IsEnabled() -- returns the IME-enabled flag byte @ 0x0143E890
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
