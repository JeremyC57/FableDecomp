// NScript::CQ_AmbushTradersScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQ_AmbushTradersScript(data, game)
// Leaf: ?Alloc@CQ_AmbushTradersScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CFA2F0, 40 bytes, __fastcall (data in ecx, the interface in edx).
// Identity (proven, chunk c677): new(0x1D0) + ctor 0x00CF9350 (??0CQ_AmbushTradersScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z,
//   its own row), registered as Q_AmbushTraders (S_QAT); Ego_r.pdb lists this exact symbol (ego_r.exe.signatures.tsv 0x00F8E7E0).
// Callees: operator new 0x00BFEA1A, CQ_AmbushTradersScript::CQ_AmbushTradersScript 0x00CF9350.
// Shape: retail bytes push esi ; push edi ; push 0x1d0 ; mov esi,edx ; mov edi,ecx ; call operator new ; add esp,4 ; test eax,eax ;
//   je N ; push esi ; push edi ; mov ecx,eax ; call ctor ; pop edi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Naming wave 17: landed as the free __fastcall Create_CClass_00cf9350_00cfa2f0(void*, void*) on a CClass_00cf9350 stand-in;
//   respelled on the PDB static member (name-only: the bytes are unchanged). Base as the landed ctor row 0x00CF9350 models it.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_AmbushTradersScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_AmbushTradersScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);   // 0x00CF9350
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members[0x1d0 - 0x48];                // +0x48 (the script's own members; retail sizeof 0x1D0)
};
CScriptBase* __fastcall CQ_AmbushTradersScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_AmbushTradersScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CQ_AmbushTradersScript) == 0x1d0 ? 1 : -1];
