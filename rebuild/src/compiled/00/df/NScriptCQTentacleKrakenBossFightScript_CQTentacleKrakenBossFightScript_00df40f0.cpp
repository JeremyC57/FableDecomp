// NScript::CQ_TentacleKrakenBossFightScript::CQ_TentacleKrakenBossFightScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script constructor: CScriptBase, the level-script Game / MasterScriptData pointers, the own vtable, then the empty TheKraken script thing
// Leaf: ??0CQ_TentacleKrakenBossFightScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF40F0, 60 bytes (+ int3 padding to 64), thiscall `ret 8`.
// Identity (proven): NScript::CQ_TentacleKrakenBossFightScript::CQ_TentacleKrakenBossFightScript(CScriptDataBase*, CGameScriptInterfaceBase&); evidence: the body stores the RTTI-less vtable 0x012DE510, whose Main (slot 2, 0x00DF4240) registers the entity scripts KrakenMinion, M_Experience, TheKraken, all nested in Ego_r.pdb NScript::CQ_TentacleKrakenBossFightScript and whose OnPersist (slot 4, 0x00DF4210) transfers KrakenAutoSaveCheckPoint, members of only CQ_TentacleKrakenBossFightScript; the empty script thing lands at +0x4C = Ego_r CQ_TentacleKrakenBossFightScript::TheKraken (CScriptThing, +0x60) - 0x14; the Ego_r/FableWin.pdb name sets hold ??0CQ_TentacleKrakenBossFightScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Label: the manifest's "CIcicleData" is propagated across family J036 (six one-CScriptThing quest-script constructors).
// Callees: CScriptBase::CScriptBase 0x00CB8110 (??0CScriptBase@@QAE@XZ), CBaseClass::CBaseClass 0x0099A2D0 (??0CBaseClass@@QAE@XZ;
//   the inline CScriptThing() then stores its vtable 0x01238C8C and zeroes PImp).
// Layout: Ego_r.pdb CQ_TentacleKrakenBossFightScript: CLevelScriptBase<T> {CGameScriptInterfaceBase* Game, T* MasterScriptData} at retail +0x40 / +0x44,
//   TheKraken (CScriptThing) Ego_r +0x60 -> retail +0x4C (quest-script members sit 0x14 lower in retail).
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
class CQ_TentacleKrakenBossFightScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_TentacleKrakenBossFightScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQ_TentacleKrakenBossFightScript();
    unsigned char _members[0x4C - 0x48];          // POD members up to the script thing
    CScriptThing  TheKraken;                            // +0x4C
};
CQ_TentacleKrakenBossFightScript::CQ_TentacleKrakenBossFightScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}