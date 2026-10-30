// NScript::CV_MayorsInvitationScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script factory: new CV_MayorsInvitationScript(data, game) with the constructor inlined (CScriptBase, Game / MasterScriptData, the own vtable, the empty destinationHSP script thing)
// Leaf: ?Alloc@CV_MayorsInvitationScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EBCC10, 84 bytes (+ int3 padding to 96), static `__fastcall` (data in ecx, game in edx), plain `ret`.
// Identity (proven): NScript::CV_MayorsInvitationScript::Alloc; evidence: the inlined constructor stores the RTTI-less vtable 0x012F0528 (also stored by this class's J036 constructor 0x00EB6B90), whose Main (slot 2, 0x00EB6D70) registers the entity scripts LadyGrey, WiseVillager, MI_InfoKeeper, all nested in Ego_r.pdb NScript::CV_MayorsInvitationScript and whose OnPersist (slot 4, 0x00EB98A0) transfers WhichQuest / SecretsLearned / HeroLearnedSecret / HeroHasDefeatedThunder / HeroHasLostToThunder / HeroReturnedFromParanoidQuest, members of only CV_MayorsInvitationScript; the empty script thing lands at +0x60 = Ego_r CV_MayorsInvitationScript::destinationHSP - 0x14 and `new` asks 0x6C (sizeof Ego_r 0x80 - 0x14); this factory is stored at 0x00CD912D (CScriptInfoManager registration 0x00CD52D0); the Ego_r/FableWin.pdb name sets hold ?Alloc@CV_MayorsInvitationScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Label: the manifest's "Alloc" is generic (family J037: six one-CScriptThing quest-script factories).
// Callees: operator new 0x00BFEA1A (0x6C bytes), CScriptBase::CScriptBase 0x00CB8110 (??0CScriptBase@@QAE@XZ), CBaseClass::CBaseClass
//   0x0099A2D0 (??0CBaseClass@@QAE@XZ; the inline CScriptThing() then stores its vtable 0x01238C8C and zeroes PImp).
// Layout: Ego_r.pdb CV_MayorsInvitationScript (0x80; retail 0x6C = Ego_r - 0x14): CLevelScriptBase<T> {Game, MasterScriptData} at
//   retail +0x40 / +0x44, destinationHSP (CScriptThing) Ego_r +0x74 -> retail +0x60.
// Model: the J036 constructor of the same class made inline (agent952 H037 shape); local classes -- Review at landing.
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
class CV_MayorsInvitationScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_MayorsInvitationScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
        : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CV_MayorsInvitationScript();
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members[0x60 - 0x48];          // POD members up to the script thing
    CScriptThing  destinationHSP;                            // +0x60
};
CScriptBase* __fastcall CV_MayorsInvitationScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CV_MayorsInvitationScript(data, game);
}
}