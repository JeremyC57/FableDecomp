// NScript::CV_BanditCampPathScript::Init() -- reset the script state: OverheardBanditConversation = false
// Leaf: ?Init@CV_BanditCampPathScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E46C30, 5 bytes, thiscall, plain `ret`.
// Identity: outer script vtable 012e4a9c slot 3 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00e46d30 (slot 2) registers 1 entity scripts whose PDB Alloc outer is CV_BanditCampPathScript; PDB ?Init@CV_BanditCampPathScript@NScript@@; refs=1; was sub_00e46c30 (wave3/labels_alloc.tsv); PDB ?Init@CV_BanditCampPathScript@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Clear() on a `struct T` pad model (00/e4/gapscan_sub00e46c30_00e46c30.cpp, `?Clear@T@@QAEXXZ`); same bytes.
// Model: engine/CLevelScriptBase.h (Game +0x40, MasterScriptData +0x44); the members this body stores are named from
//   Ego_r.pdb CV_BanditCampPathScript at +0x14 (the retail script base is 0x14 smaller), the rest is not modelled here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BanditCampPathScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Init();                          // slot 3
    bool OverheardBanditConversation;   // +0x48 (Ego_r.pdb +0x5c)
};
void CV_BanditCampPathScript::Init()
{
    OverheardBanditConversation = false;
}
}
