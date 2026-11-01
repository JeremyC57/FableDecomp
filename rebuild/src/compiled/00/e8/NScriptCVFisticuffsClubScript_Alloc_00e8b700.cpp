// NScript::CV_FisticuffsClubScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_FisticuffsClubScript(data, game)
// Leaf: ?Alloc@CV_FisticuffsClubScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E8B700, 40 bytes, __fastcall (data in ecx, the interface in edx).
// Identity (proven, chunk c673): the factory the script-info registration names for CV_FisticuffsClubScript; `push c8h` = the retail
//   sizeof, then the constructor 0x00E81F60 (??0CV_FisticuffsClubScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z, its own row)
//   on the new block; the exact PDB signature (as the landed Alloc rows, e.g. 0x00D61BB0 CQ_GuildTrainingWillScript::Alloc).
// Callees: operator new 0x00BFEA1A, CV_FisticuffsClubScript::CV_FisticuffsClubScript 0x00E81F60.
// Shape: retail bytes push esi ; push edi ; push c8h ; mov esi,edx ; mov edi,ecx ; call operator new ; test ; je ; push esi ;
//   push edi ; mov ecx,eax ; call ctor ; ... -- the constructor out of line, the null-tested `new`.
// Naming wave 16: landed as the free __fastcall Create_CClass_00e81f60_00e8b700(void*, void*) on a CClass_00e81f60 stand-in;
//   respelled on the PDB static member (name-only: the bytes are unchanged).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_FisticuffsClubScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_FisticuffsClubScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);   // 0x00E81F60
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members[0xc8 - 0x48];                 // +0x48 (the script's own members; retail sizeof 0xc8)
};
CScriptBase* __fastcall CV_FisticuffsClubScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_FisticuffsClubScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CV_FisticuffsClubScript) == 0xc8 ? 1 : -1];
