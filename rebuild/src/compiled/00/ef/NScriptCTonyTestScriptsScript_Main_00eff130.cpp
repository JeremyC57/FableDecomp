// NScript::CTonyTestScriptsScript::Main() -- the TonyTestScripts debug quest main: bind the "Dummy" entity script, post-add the scripted entities (script and interface), then hand the hero the guild quest cards (AddQuestsToGuild)
// Leaf: ?Main@CTonyTestScriptsScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFF130, 144 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012F6F90 [dtor 0x00EFF7B0, RegisterMain 0x00EFF090 (landed
//   NScript::CTonyTestScriptsScript::RegisterMain), Main 0x00EFF130], stored by the landed ctor 0x00EFF060 (site 0x00EFF078);
//   the row is referenced only from that slot (0x012F6F98). Ego_r.pdb name set: ?Main@CTonyTestScriptsScript@NScript@@UAEXXZ.
//   The binding factory 0x00EFF3F0 is the landed CTonyTestScriptsScript::CDummy::Alloc, paired with its "Dummy" literal
//   (0x012F6F0C); the closing call 0x00EFF1C0 is the only other CTonyTestScriptsScript member in the Ego_r.pdb name set,
//   ?AddQuestsToGuild@CTonyTestScriptsScript@NScript@@QAEXXZ (landed 0x00EFF1C0). Catalog `sub_00eff130`.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930, AddQuestsToGuild 0x00EFF1C0;
//   vtable CEntityScriptBinding<CTonyTestScriptsScript> 0x012F6FC0; CGameScriptInterfaceBase slot 64 PostAddScriptedEntities.
// Layout: the retail NScript headers (FABLE_RETAIL_NSCRIPT: CLevelScriptBase<T> : CScriptBase, Game +0x40;
//   CEntityScriptBinding<T>, 0x1C). The master-data type CQ_SunnyvaleMasterData is the landed CDummy::Alloc 0x00EFF3F0 choice
//   (probable; Main does not touch it).
// Model: the landed NScript::CV_TalentlessBardScript::Main 0x00ED7830 (binding record, conditional-temporary flag in ebx).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int bytes);         // 0x00BFEA1A (MSVCR71)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CGameScriptInterfaceBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CTonyTestScriptsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CDummy {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CTonyTestScriptsScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EFF3F0
    };
    virtual void Main();
    void AddQuestsToGuild();                            // 0x00EFF1C0
};

void CTonyTestScriptsScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CTonyTestScriptsScript>("Dummy", this, &CDummy::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddQuestsToGuild();
}
}   // namespace NScript