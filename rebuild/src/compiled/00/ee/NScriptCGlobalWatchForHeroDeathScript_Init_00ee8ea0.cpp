// NScript::CGlobal_WatchForHeroDeathScript::Init() -- empty: retail is a lone `ret`
// Leaf: ?Init@CGlobal_WatchForHeroDeathScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE8EA0, 1 bytes, thiscall, plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00ee90a0 at 00cd9a4a with name "Global_WatchForHeroDeath"; Alloc news + stores script vtable 012f5470; PDB ?Alloc@CGlobal_WatchForHeroDeathScript@NScript@@SIPAVCScriptBase@@; script vtable slot 3 (slot 1 pushes "Main" 0x012C2F94); PDB Init; refs=1; was sub_00ee8ea0 (wave3/labels_alloc.tsv); PDB ?Init@CGlobal_WatchForHeroDeathScript@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ee/gapscan_sub00ee8ea0_00ee8ea0.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CScriptBase.h + Game +0x40 (PDB ctor CGlobal_WatchForHeroDeathScript(CGameScriptInterfaceBase&): no master data); Init declared public virtual as the PDB spells it.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CGlobal_WatchForHeroDeathScript : public CScriptBase {
public:
    virtual void Init();                          // slot 3
    CGameScriptInterfaceBase* Game;         // +0x40 (the ctor's CGameScriptInterfaceBase&)
};
void CGlobal_WatchForHeroDeathScript::Init()
{
}
}
