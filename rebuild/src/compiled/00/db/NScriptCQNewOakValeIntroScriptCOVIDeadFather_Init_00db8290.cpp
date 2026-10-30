// NScript::CQ_NewOakValeIntroScript::COVI_DeadFather::Init() -- give the dead father the quest-core mini-map orb and make him not pushable by the hero
// Leaf: ?Init@COVI_DeadFather@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DB8290, 100 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012D957C, which the entity Alloc 0x00DB81B0 (`NScript::CQ_NewOakValeIntroScript::COVI_DeadFather::Alloc`) stores; ?Init@COVI_DeadFather@CQ_NewOakValeIntroScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00db8290" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; interface slots 350 MiniMapAddMarker, 844 SetIsPushableByHero(CScriptThing, bool) (by value: the copy is built in the argument slot).
// Data: "HUD_ORB_QUEST_CORE" @ 0x01258AB0; vtables CBaseClass 0x01231710, CScriptThing 0x01238C8C.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets.
// Shape: the inline CScriptThing copy ctor stores the CBaseClass then the CScriptThing vtable and adds a counted-pointer reference.
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
class CQ_NewOakValeIntroScript {
public:
    class COVI_DeadFather;
};
class CQ_NewOakValeIntroScript::COVI_DeadFather : public CEntityScriptBase {
public:
    CQ_NewOakValeIntroScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
private:
    virtual void Init();
};
}

void NScript::CQ_NewOakValeIntroScript::COVI_DeadFather::Init()
{
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    Game->SetIsPushableByHero(Me, false);
}