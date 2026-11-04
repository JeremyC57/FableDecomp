// NScript::CV_HeroDollsScript::Init() -- reset the script state: HeroDollScriptEnded = false, KidIntroDone = false
// Leaf: ?Init@CV_HeroDollsScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E96E60, 9 bytes, thiscall, plain `ret`.
// Identity: outer script vtable 012ece7c slot 3 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00e96f10 (slot 2) registers 1 entity scripts whose PDB Alloc outer is CV_HeroDollsScript; PDB ?Init@CV_HeroDollsScript@NScript@@; refs=1; was sub_00e96e60 (wave3/labels_alloc.tsv); PDB ?Init@CV_HeroDollsScript@NScript@@UAEXXZ in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Clear() on a `struct T` pad model (00/e9/gapscan_sub00e96e60_00e96e60.cpp, `?Clear@T@@QAEXXZ`); same bytes.
// Model: engine/CLevelScriptBase.h (Game +0x40, MasterScriptData +0x44); the members this body stores are named from
//   Ego_r.pdb CV_HeroDollsScript at +0x14 (the retail script base is 0x14 smaller), the rest is not modelled here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_HeroDollsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Init();                          // slot 3
    bool HeroDollScriptEnded;           // +0x48 (Ego_r.pdb +0x5c)
    bool KidIntroDone;                  // +0x49 (Ego_r.pdb +0x5d)
};
void CV_HeroDollsScript::Init()
{
    HeroDollScriptEnded = false;
    KidIntroDone = false;
}
}
