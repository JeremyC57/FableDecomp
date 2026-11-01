// NScript::CQ_HangingTreeGoodScript::CHTG_Audience::~CHTG_Audience() -- implicit entity-script destructor: destroy PrisonerGuy, ExecutionerGuy and ChopBlockMarker (CScriptThing) in reverse order, then ~CEntityScriptBase
// Leaf: ??1CHTG_Audience@CQ_HangingTreeGoodScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D728F0, 185 bytes, tail jump to ~CEntityScriptBase 0x00F35B40; sole caller the scalar
//   deleting dtor 0x00D728D0 (slot 0 of the RTTI-less entity vtable 0x012D292C).
// Identity (proven): vtable 0x012D292C is stored only by the ctor 0x00D72160, whose only caller is the factory 0x00D720E0
//   (`new` 0x40 = sizeof Ego_r CHTG_Audience); CQ_HangingTreeGoodScript::Main 0x00D6F4B0 (slot 2 of the quest vtable 0x012D260C,
//   whose ctor 0x00D6EF20 is reached only from the Alloc registered as "Q_HangingTreeGood" / "S_QHTG") stores that factory in the
//   CEntityScriptBinding record named "HTG_Audience" (string 0x012D2688, site 0x00D6F856). Ego_r.pdb and FableWin.pdb carry
//   ??1CHTG_Audience@CQ_HangingTreeGoodScript@NScript@@UAE@XZ; no vtable store in the body = compiler-generated dtor.
// Layout: Ego_r.pdb CQ_HangingTreeGoodScript::CHTG_Audience (0x40, same as retail): CEntityScriptBase 0x14, ParentClass +0x14,
//   PMasterData +0x18, ChopBlockMarker +0x1c, ExecutionerGuy +0x28, PrisonerGuy +0x34.
// Callees: ~CScriptThing inline (vtable 0x01238C8C store, CCountedPointer<CScriptThing> release: DeleteFunc fastcall + operator
//   delete 0x00BFE9BC, then ~CBaseClass 0x0099A2E0), ~CEntityScriptBase 0x00F35B40.
// TU: the PDB ctor is defined (retail keeps it out of line at 0x00D72160), which emits the vtable, ??_G and this implicit dtor.
void operator delete(void* p);                          // 0x00BFE9BC
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() {}
#include "engine/CEntityScriptBase.h"
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HangingTreeGoodScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CHTG_Audience : public CEntityScriptBase {
    public:
        CQ_HangingTreeGoodScript* ParentClass;          // +0x14
        CQ_SunnyvaleMasterData*   PMasterData;          // +0x18
        CScriptThing              ChopBlockMarker;      // +0x1c
        CScriptThing              ExecutionerGuy;       // +0x28
        CScriptThing              PrisonerGuy;          // +0x34
        CHTG_Audience(CQ_HangingTreeGoodScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
        virtual void Main();
    public:
        virtual CScriptBase* GetParentScript() const;
    };
};
CQ_HangingTreeGoodScript::CHTG_Audience::CHTG_Audience(CQ_HangingTreeGoodScript& parent, CScriptDataBase* data, const CScriptThing& me)   // 0x00D72160
    : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
}
