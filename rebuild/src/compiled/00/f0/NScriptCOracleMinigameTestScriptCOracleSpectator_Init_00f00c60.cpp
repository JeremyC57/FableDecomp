// NScript::COracleMinigameTestScript::COracleSpectator::Init() -- the spectator's next line index starts at 10
// Leaf: ?Init@COracleSpectator@COracleMinigameTestScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F00C60, 8 bytes, thiscall: mov dword ptr [ecx+1Ch],0Ah ; ret.
// Identity: slot 2 (Init, engine/CEntityScriptBase.h) of the entity vtable 0x012F7274 that the landed Alloc 0x00F00BB0 stores
//   (slot 0 ??_G 0x00F00C70, slot 3 GetParentScript 0x00F00C50 landed); Ego_r.pdb / FableWin.pdb
//   ?Init@COracleSpectator@COracleMinigameTestScript@NScript@@EAEXXZ (private virtual).
// Layout: Ego_r.pdb COracleSpectator (32 bytes): ParentClass +0x14, PMasterData +0x18, long NextLine +0x1C.
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
class COracleMinigameTestScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class COracleSpectator : public CEntityScriptBase {
    public:
        COracleMinigameTestScript* ParentClass;      // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long NextLine;                                  // +0x1C
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(COracleMinigameTestScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@COracleSpectator@COracleMinigameTestScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void COracleMinigameTestScript::COracleSpectator::Init()
{
    NextLine = 10;
}
}