// NScript::CV_GhostGrannyNecklaceScript::CGhostGrannyNecklace::Init() -- the necklace entity starts in brain state 1
// Leaf: ?Init@CGhostGrannyNecklace@CV_GhostGrannyNecklaceScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E8BDE0, 8 bytes, thiscall: mov dword ptr [ecx+1Ch],1 ; ret.
// Identity: slot 2 (Init, engine/CEntityScriptBase.h) of the entity vtable 0x012EBB14 that the landed Alloc 0x00E8BD30 stores
//   (slot 0 ??_G 0x00E8BDF0, slot 1 Main 0x00E8DFA0, slot 3 GetParentScript 0x00E8BDD0, slot 4 OnPersist 0x00E8D2A0 all landed);
//   Ego_r.pdb / FableWin.pdb ?Init@CGhostGrannyNecklace@CV_GhostGrannyNecklaceScript@NScript@@EAEXXZ (private virtual).
// Layout: Ego_r.pdb CGhostGrannyNecklace (32 bytes): ParentClass +0x14, PMasterData +0x18, long BrainState +0x1C (entity
//   offsets are not shifted: CEntityScriptBase is 0x14 in both builds). Class model: the landed Alloc / ??_G rows.
class CGameScriptInterfaceBase;
class CScriptDataBase;

// 0x00CDEE00 is CCountedPointer<T>::DeleteData (engine/CCountedPointer.h; reloc_bindings binds both spellings)

#include "engine/CCountedPointer.h"


// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_GhostGrannyNecklaceScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CGhostGrannyNecklace : public CEntityScriptBase {
    public:
        CV_GhostGrannyNecklaceScript* ParentClass;      // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long BrainState;                                // +0x1C
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_GhostGrannyNecklaceScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CGhostGrannyNecklace@CV_GhostGrannyNecklaceScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CV_GhostGrannyNecklaceScript::CGhostGrannyNecklace::Init()
{
    BrainState = 1;
}
}