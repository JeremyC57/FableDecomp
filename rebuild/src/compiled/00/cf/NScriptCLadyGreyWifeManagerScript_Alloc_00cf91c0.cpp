// NScript::CLadyGreyWifeManagerScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CLadyGreyWifeManagerScript(data, game)
// Leaf: ?Alloc@CLadyGreyWifeManagerScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CF91C0, 40 bytes, __fastcall (data in ecx, the interface in edx).
// Identity (proven, chunk c681): new(0x94) + ctor 0x00CF88C0 (??0CLadyGreyWifeManagerScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z,
//   its own row); Ego_r.pdb / FableWin.pdb list this exact symbol.
// Callees: operator new 0x00BFEA1A, CLadyGreyWifeManagerScript::CLadyGreyWifeManagerScript 0x00CF88C0.
// Shape: retail bytes push esi ; push edi ; push 0x94 ; mov esi,edx ; mov edi,ecx ; call operator new ; add esp,4 ; test eax,eax ;
//   je N ; push esi ; push edi ; mov ecx,eax ; call ctor ; pop edi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Naming wave 18: landed as the free __fastcall Create_CLadyGreyWifeManagerScript_00cf91c0(void*, void*) on a CLadyGreyWifeManagerScript(void*, void*) stand-in; respelled on the PDB static
//   member (name-only: the bytes are unchanged). Base as the landed 0x00CFA2F0 Alloc row models it (CLevelScriptBase<CQ_SunnyvaleMasterData>:
//   the master data class of the five Ego_r instantiations that the game quests use).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CLadyGreyWifeManagerScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CLadyGreyWifeManagerScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);   // 0x00CF88C0
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members[0x94 - 0x48];                // +0x48 (the script's own members; retail sizeof 0x94)
};
CScriptBase* __fastcall CLadyGreyWifeManagerScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CLadyGreyWifeManagerScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CLadyGreyWifeManagerScript) == 0x94 ? 1 : -1];
