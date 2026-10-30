// NScript::CQ_PrisonWardenGameScript::CQ_PrisonWardenGameScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script constructor: CScriptBase, the level-script Game / MasterScriptData pointers, the own vtable, then the empty SThingBookOpened script thing
// Leaf: ??0CQ_PrisonWardenGameScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DDDC90, 60 bytes (+ int3 padding to 64), thiscall `ret 8`.
// Identity (proven): NScript::CQ_PrisonWardenGameScript::CQ_PrisonWardenGameScript(CScriptDataBase*, CGameScriptInterfaceBase&); evidence: the body stores the RTTI-less vtable 0x012DCB40, whose Main (slot 2, 0x00DDDE20) registers the entity scripts PrisonWarden, SecretBook, ExitWardensOffice, DoNotTarget, all nested in Ego_r.pdb NScript::CQ_PrisonWardenGameScript; the empty script thing lands at +0x50 = Ego_r CQ_PrisonWardenGameScript::SThingBookOpened (CScriptThing, +0x64) - 0x14; the Ego_r/FableWin.pdb name sets hold ??0CQ_PrisonWardenGameScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Label: the manifest's "CIcicleData" is propagated across family J036 (six one-CScriptThing quest-script constructors).
// Callees: CScriptBase::CScriptBase 0x00CB8110 (??0CScriptBase@@QAE@XZ), CBaseClass::CBaseClass 0x0099A2D0 (??0CBaseClass@@QAE@XZ;
//   the inline CScriptThing() then stores its vtable 0x01238C8C and zeroes PImp).
// Layout: Ego_r.pdb CQ_PrisonWardenGameScript: CLevelScriptBase<T> {CGameScriptInterfaceBase* Game, T* MasterScriptData} at retail +0x40 / +0x44,
//   SThingBookOpened (CScriptThing) Ego_r +0x64 -> retail +0x50 (quest-script members sit 0x14 lower in retail).
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
class CScriptBase {
public:
    CScriptBase();                                      // 0x00CB8110
    virtual ~CScriptBase();
    unsigned char _base[0x40 - 4];                      // Threads, EntityScripts, EntityScriptBindings, LastWorldUpdateFrame,
                                                        // PActiveThread, RuntimeScriptedEntityPersist
};
template <class TMasterData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TMasterData*              MasterScriptData;         // +0x44
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game)
        : Game(&game), MasterScriptData((TMasterData*)data) { }
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_PrisonWardenGameScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_PrisonWardenGameScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQ_PrisonWardenGameScript();
    unsigned char _members[0x50 - 0x48];          // POD members up to the script thing
    CScriptThing  SThingBookOpened;                            // +0x50
};
CQ_PrisonWardenGameScript::CQ_PrisonWardenGameScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}