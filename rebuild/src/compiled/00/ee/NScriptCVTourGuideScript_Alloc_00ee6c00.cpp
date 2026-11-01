// NScript::CV_TourGuideScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CV_TourGuideScript(data, game)
// Leaf: ?Alloc@CV_TourGuideScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EE6C00, 40 bytes, __fastcall (data in ecx, the interface in edx).
// Identity (proven, chunk c673): the factory the script-info registration names for CV_TourGuideScript; `push 178h` = the retail
//   sizeof, then the constructor 0x00EE41F0 (??0CV_TourGuideScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z, its own row)
//   on the new block; the exact PDB signature (as the landed Alloc rows, e.g. 0x00D61BB0 CQ_GuildTrainingWillScript::Alloc).
// Callees: operator new 0x00BFEA1A, CV_TourGuideScript::CV_TourGuideScript 0x00EE41F0.
// Shape: retail bytes push esi ; push edi ; push 178h ; mov esi,edx ; mov edi,ecx ; call operator new ; test ; je ; push esi ;
//   push edi ; mov ecx,eax ; call ctor ; ... -- the constructor out of line, the null-tested `new`.
// Naming wave 16: landed as the free __fastcall Create_CClass_00ee41f0_00ee6c00(void*, void*) on a CClass_00ee41f0 stand-in;
//   respelled on the PDB static member (name-only: the bytes are unchanged).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TourGuideScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_TourGuideScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);   // 0x00EE41F0
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members[0x178 - 0x48];                 // +0x48 (the script's own members; retail sizeof 0x178)
};
CScriptBase* __fastcall CV_TourGuideScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_TourGuideScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CV_TourGuideScript) == 0x178 ? 1 : -1];
