// SetDSTGame(CGameScriptInterfaceBase& game) -- stores the game script interface into pDSTGame @ 0x0143E8F8 (fastcall)
// Leaf: ?SetDSTGame@@YIXAAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBE0C0, oracle length 7 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov dword ptr [0x143e8f8], ecx ; ret 
// Names (agentNF22): Ego_r.pdb + FableWin.pdb ?SetDSTGame@@YIXAAVCGameScriptInterfaceBase@@@Z (the catalog label) is the only
// writer of ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (Codex pass 21 NAME_FIXES; ESTABLISHED_SPELLINGS); the reference
// argument arrives in ecx and is stored as the pointer. Was SetCurrentThingManager(void*) storing g_value_0143e8f8.
class CGameScriptInterfaceBase;
extern CGameScriptInterfaceBase* pDSTGame;          // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A

void __fastcall SetDSTGame(CGameScriptInterfaceBase& game)
{
    pDSTGame = &game;
}
