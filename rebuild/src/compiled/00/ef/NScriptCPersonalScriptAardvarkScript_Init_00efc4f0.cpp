// NScript::CPersonalScript_AardvarkScript::Init() -- reset the script state: ScriptVar = 0
// Leaf: ?Init@CPersonalScript_AardvarkScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFC4F0, 8 bytes, thiscall, plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00efc8d0 at 00cda60e with name "PersonalScript_Aardvark"; Alloc news + stores script vtable 012f6ca8; PDB ?Alloc@CPersonalScript_AardvarkScript@NScript@@SIPAVCScriptBase@@; script vtable slot 3 (slot 1 pushes "Main" 0x012C2F94); PDB Init; refs=1; was sub_00efc4f0 (wave3/labels_alloc.tsv); PDB ?Init@CPersonalScript_AardvarkScript@NScript@@UAEXXZ in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Clear() on a `struct T` pad model (00/ef/gapscan_sub00efc4f0_00efc4f0.cpp, `?Clear@T@@QAEXXZ`); same bytes.
// Model: engine/CLevelScriptBase.h (CLevelScriptBase<CQ_SunnyvaleMasterData>, the quest-script default) (Game +0x40, MasterScriptData +0x44); the members this body stores are named from
//   Ego_r.pdb CPersonalScript_AardvarkScript at +0x14 (the retail script base is 0x14 smaller), the rest is not modelled here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CPersonalScript_AardvarkScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Init();                          // slot 3
    long ScriptVar;                     // +0x48 (Ego_r.pdb +0x5c)
};
void CPersonalScript_AardvarkScript::Init()
{
    ScriptVar = 0;
}
}
