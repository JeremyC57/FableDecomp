// NScript::CQ_SecretPassageScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQ_SecretPassageScript(data, game)
// Leaf: ?Alloc@CQ_SecretPassageScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF1080, 40 bytes, __fastcall (data in ecx, the interface in edx).
// Identity (proven, chunk c678): new(0x12C) + ctor 0x00DEC7F0 (??0CQ_SecretPassageScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z,
//   its own row); Ego_r.pdb / FableWin.pdb list this exact symbol.
// Callees: operator new 0x00BFEA1A, CQ_SecretPassageScript::CQ_SecretPassageScript 0x00DEC7F0.
// Shape: retail bytes push esi ; push edi ; push 0x12c ; mov esi,edx ; mov edi,ecx ; call operator new ; add esp,4 ; test eax,eax ;
//   je N ; push esi ; push edi ; mov ecx,eax ; call ctor ; pop edi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Naming wave 18: landed as the free __fastcall Create_CHeroMorphDef_00df1080(void*, void*) on a CHeroMorphDef stand-in; respelled on the PDB static
//   member (name-only: the bytes are unchanged). Base as the landed 0x00CFA2F0 Alloc row models it (CLevelScriptBase<CQ_SunnyvaleMasterData>:
//   the master data class of the five Ego_r instantiations that the game quests use).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_SecretPassageScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_SecretPassageScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);   // 0x00DEC7F0
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members[0x12c - 0x48];                // +0x48 (the script's own members; retail sizeof 0x12C)
};
CScriptBase* __fastcall CQ_SecretPassageScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_SecretPassageScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CQ_SecretPassageScript) == 0x12c ? 1 : -1];
