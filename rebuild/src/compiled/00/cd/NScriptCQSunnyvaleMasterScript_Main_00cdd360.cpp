// NScript::CQ_SunnyvaleMasterScript::Main() -- run script frames until the thread is told to terminate
// Leaf: ?Main@CQ_SunnyvaleMasterScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDD360, 24 bytes, thiscall, plain `ret`; calls Game (+0x40) slot 7 NewScriptFrame (+0x1c), CScriptBase::IsActiveThreadTerminating 0x00CB7940.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00cdd550 at 00cd5340 with name "Q_SunnyvaleMaster"; Alloc news + stores script vtable 012c2f64; PDB ?Alloc@CQ_SunnyvaleMasterScript@NScript@@SIPAVCScriptBase@@; script vtable slot 2 (slot 1 pushes "Main" 0x012C2F94); PDB Main; refs=1; was CVtable_012c2f64::vfunc_2 (wave3/labels_alloc.tsv); PDB ?Main@CQ_SunnyvaleMasterScript@NScript@@UAEXXZ in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?CQ_SunnyvaleMasterScript_Main@@YIXPAUCQ_SunnyvaleMasterScript@@@Z` on a pad model (00/cd/CQSunnyvaleMasterScript_Main_00cdd360.cpp); same bytes.
// Model: engine/CLevelScriptBase.h (Game +0x40) + engine/CGameScriptInterfaceBase.h; Main public virtual as the PDB spells it.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_SunnyvaleMasterScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Main();                          // slot 2
};
void CQ_SunnyvaleMasterScript::Main()
{
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
}
