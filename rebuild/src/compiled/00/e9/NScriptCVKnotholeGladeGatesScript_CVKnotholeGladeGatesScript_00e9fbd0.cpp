// NScript::CV_KnotholeGladeGatesScript::CV_KnotholeGladeGatesScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script constructor: CScriptBase, the level-script Game / MasterScriptData pointers, the own vtable, then the empty TheGate script thing
// Leaf: ??0CV_KnotholeGladeGatesScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E9FBD0, 60 bytes (+ int3 padding to 64), thiscall `ret 8`.
// Identity (proven): NScript::CV_KnotholeGladeGatesScript::CV_KnotholeGladeGatesScript(CScriptDataBase*, CGameScriptInterfaceBase&); evidence: the body stores the RTTI-less vtable 0x012ED9D4, whose Main (slot 2, 0x00E9FCD0) registers the entity scripts KGG_VillagerOutside, KGG_SurpriseBalverine, all nested in Ego_r.pdb NScript::CV_KnotholeGladeGatesScript; the empty script thing lands at +0x5C = Ego_r CV_KnotholeGladeGatesScript::TheGate (CScriptThing, +0x70) - 0x14; the Ego_r/FableWin.pdb name sets hold ??0CV_KnotholeGladeGatesScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Label: the manifest's "CIcicleData" is propagated across family J036 (six one-CScriptThing quest-script constructors).
// Callees: CScriptBase::CScriptBase 0x00CB8110 (??0CScriptBase@@QAE@XZ), CBaseClass::CBaseClass 0x0099A2D0 (??0CBaseClass@@QAE@XZ;
//   the inline CScriptThing() then stores its vtable 0x01238C8C and zeroes PImp).
// Layout: Ego_r.pdb CV_KnotholeGladeGatesScript: CLevelScriptBase<T> {CGameScriptInterfaceBase* Game, T* MasterScriptData} at retail +0x40 / +0x44,
//   TheGate (CScriptThing) Ego_r +0x70 -> retail +0x5C (quest-script members sit 0x14 lower in retail).
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
class CV_KnotholeGladeGatesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_KnotholeGladeGatesScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CV_KnotholeGladeGatesScript();
    unsigned char _members[0x5C - 0x48];          // POD members up to the script thing
    CScriptThing  TheGate;                            // +0x5C
};
CV_KnotholeGladeGatesScript::CV_KnotholeGladeGatesScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}