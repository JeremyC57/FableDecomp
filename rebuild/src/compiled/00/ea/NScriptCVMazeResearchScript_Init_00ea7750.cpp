// NScript::CV_MazeResearchScript::Init() -- reset the script state: SwordTaken = false, BookRead = false
// Leaf: ?Init@CV_MazeResearchScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA7750, 9 bytes, thiscall, plain `ret`.
// Identity: outer script vtable 012ee444 slot 3 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00ea7800 (slot 2) registers 2 entity scripts whose PDB Alloc outer is CV_MazeResearchScript; PDB ?Init@CV_MazeResearchScript@NScript@@; refs=1; was sub_00ea7750 (wave3/labels_alloc.tsv); PDB ?Init@CV_MazeResearchScript@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Clear() on a `struct T` pad model (00/ea/gapscan_sub00ea7750_00ea7750.cpp, `?Clear@T@@QAEXXZ`); same bytes.
// Model: engine/CLevelScriptBase.h (Game +0x40, MasterScriptData +0x44); the members this body stores are named from
//   Ego_r.pdb CV_MazeResearchScript at +0x14 (the retail script base is 0x14 smaller), the rest is not modelled here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_MazeResearchScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Init();                          // slot 3
    bool SwordTaken;                    // +0x48 (Ego_r.pdb +0x5c)
    bool BookRead;                      // +0x49 (Ego_r.pdb +0x5d)
};
void CV_MazeResearchScript::Init()
{
    SwordTaken = false;
    BookRead = false;
}
}
