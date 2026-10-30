// NScript::CQ_WhiteBalverineWWScript::CQ_WhiteBalverineWWScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script constructor: CScriptBase, the level-script Game / MasterScriptData pointers, the own vtable, then the empty WhiteBalverine script thing
// Leaf: ??0CQ_WhiteBalverineWWScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E18440, 60 bytes (+ int3 padding to 64), thiscall `ret 8`.
// Identity (proven): NScript::CQ_WhiteBalverineWWScript::CQ_WhiteBalverineWWScript(CScriptDataBase*, CGameScriptInterfaceBase&); evidence: the body stores the RTTI-less vtable 0x012E0CE4, whose Main (slot 2, 0x00E18630) registers the entity scripts KG_Chief, WBWW_WhiteBalverine, WBWW_SoldierBalverine, WBWW_WhiteBalverine, all nested in Ego_r.pdb NScript::CQ_WhiteBalverineWWScript; the empty script thing lands at +0x50 = Ego_r CQ_WhiteBalverineWWScript::WhiteBalverine (CScriptThing, +0x64) - 0x14; the Ego_r/FableWin.pdb name sets hold ??0CQ_WhiteBalverineWWScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Label: the manifest's "CIcicleData" is propagated across family J036 (six one-CScriptThing quest-script constructors).
// Callees: CScriptBase::CScriptBase 0x00CB8110 (??0CScriptBase@@QAE@XZ), CBaseClass::CBaseClass 0x0099A2D0 (??0CBaseClass@@QAE@XZ;
//   the inline CScriptThing() then stores its vtable 0x01238C8C and zeroes PImp).
// Layout: Ego_r.pdb CQ_WhiteBalverineWWScript: CLevelScriptBase<T> {CGameScriptInterfaceBase* Game, T* MasterScriptData} at retail +0x40 / +0x44,
//   WhiteBalverine (CScriptThing) Ego_r +0x64 -> retail +0x50 (quest-script members sit 0x14 lower in retail).
// Model: agent952 H037 (the one-CTimer constructors) with a CScriptThing member; local classes -- Review at landing.
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x0
    CCPPointerInfo* Info;                               // +0x4
    CCountedPointer() : Data(0), Info(0) { }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass {               // vtable 0x01238C8C, 0x0C bytes
public:
    CCountedPointer<CScriptThing> PImp;                 // +0x04
    CScriptThing() { }
};
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_WhiteBalverineWWScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_WhiteBalverineWWScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQ_WhiteBalverineWWScript();
    unsigned char _members[0x50 - 0x48];          // POD members up to the script thing
    CScriptThing  WhiteBalverine;                            // +0x50
};
CQ_WhiteBalverineWWScript::CQ_WhiteBalverineWWScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}