// NScript::CQ_TentacleKrakenBossFightScript::OnPersist(CPersistContext& context) -- saves/loads KrakenAutoSaveCheckPoint by name (default false)
// Leaf: ?OnPersist@CQ_TentacleKrakenBossFightScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF4210, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): outer script vtable 012de510 slot 4 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00df4240 (slot 2) registers 3 entity scripts whose PDB Alloc outer is CQ_TentacleKrakenBossFightScript; PDB ?OnPersist@CQ_TentacleKrakenBossFightScript@NScript@@; refs=1; was _unattributed::PersistTransfer_KrakenAutoSaveCheckPoint_00df4210.
//   ?OnPersist@CQ_TentacleKrakenBossFightScript@NScript@@UAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00df4210::PersistTransfer_KrakenAutoSaveCheckPoint_00df4210(CPersistContext* context) -- persists the "KrakenAutoSaveCheckPoint" flag at +0x4a Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF4210, oracle ... (catalog stand-in _unattributed).
// Class: the class model of the landed constructor row 0x00DF40F0 (rebuild/src/compiled/00/df/NScriptCQTentacleKrakenBossFightScript_CQTentacleKrakenBossFightScript_00df40f0.cpp); the PDB OnPersist declaration added; its bool KrakenAutoSaveCheckPoint; members KrakenAutoSaveCheckPoint +0x4a added at the offsets of the landed byte-matching source (the persisted keys are their names). CPersistContext: engine/CPersistContext.h. Only this member is graded.
#include "engine/CPersistContext.h"
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() { }
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_TentacleKrakenBossFightScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_TentacleKrakenBossFightScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQ_TentacleKrakenBossFightScript();
    unsigned char _pad048[0x4A - 0x48];   // members not touched here
    bool KrakenAutoSaveCheckPoint;   // +0x4a (persisted under this name; offset from the landed byte-matching OnPersist)
    unsigned char _pad04b[0x4C - 0x4B];
    CScriptThing  TheKraken;                            // +0x4C
public:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CQ_TentacleKrakenBossFightScript@NScript@@UAEXAAVCPersistContext@@@Z (slot 4)
};
CQ_TentacleKrakenBossFightScript::CQ_TentacleKrakenBossFightScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}

void NScript::CQ_TentacleKrakenBossFightScript::OnPersist(CPersistContext& context)
{
    context.Transfer("KrakenAutoSaveCheckPoint", KrakenAutoSaveCheckPoint, false);
}
