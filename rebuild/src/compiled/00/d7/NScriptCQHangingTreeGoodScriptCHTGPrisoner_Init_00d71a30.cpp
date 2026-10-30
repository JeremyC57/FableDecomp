// NScript::CQ_HangingTreeGoodScript::CHTG_Prisoner::Init() -- put the quest-core orb on Me's mini-map marker and clear GuardsCalledAfterMe
// Leaf: ?Init@CHTG_Prisoner@CQ_HangingTreeGoodScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D71A30, 56 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012D28A0, which the entity Alloc 0x00D71980 (`NScript::CQ_HangingTreeGoodScript::CHTG_Prisoner::Alloc`) stores; ?Init@CHTG_Prisoner@CQ_HangingTreeGoodScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d71a30" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; interface slot 350 MiniMapAddMarker.
// Data: "HUD_ORB_QUEST_CORE" @ 0x01258AB0.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets. CHTG_Prisoner: ChopBlockMarker +0x1c, ReturningToParty +0x28, GuardsCalledAfterMe +0x29.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HangingTreeGoodScript {
public:
    class CHTG_Prisoner;
};
class CQ_HangingTreeGoodScript::CHTG_Prisoner : public CEntityScriptBase {
public:
    CQ_HangingTreeGoodScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    CScriptThing              ChopBlockMarker;            // +0x1c
    bool                      ReturningToParty;           // +0x28
    bool                      GuardsCalledAfterMe;        // +0x29
private:
    virtual void Init();
};
}

void NScript::CQ_HangingTreeGoodScript::CHTG_Prisoner::Init()
{
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    GuardsCalledAfterMe = false;
}