// NScript::CQ_TentacleKrakenBossFightScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script factory: new CQ_TentacleKrakenBossFightScript(data, game) with the constructor inlined (CScriptBase, Game / MasterScriptData, the own vtable, the empty TheKraken script thing)
// Leaf: ?Alloc@CQ_TentacleKrakenBossFightScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF5C00, 84 bytes (+ int3 padding to 96), static `__fastcall` (data in ecx, game in edx), plain `ret`.
// Identity (proven): NScript::CQ_TentacleKrakenBossFightScript::Alloc; evidence: the inlined constructor stores the RTTI-less vtable 0x012DE510 (also stored by this class's J036 constructor 0x00DF40F0), whose Main (slot 2, 0x00DF4240) registers the entity scripts KrakenMinion, M_Experience, TheKraken, all nested in Ego_r.pdb NScript::CQ_TentacleKrakenBossFightScript and whose OnPersist (slot 4, 0x00DF4210) transfers KrakenAutoSaveCheckPoint, members of only CQ_TentacleKrakenBossFightScript; the empty script thing lands at +0x4C = Ego_r CQ_TentacleKrakenBossFightScript::TheKraken - 0x14 and `new` asks 0x58 (sizeof Ego_r 0x6C - 0x14); this factory is stored at 0x00CD761E (CScriptInfoManager registration 0x00CD52D0); the Ego_r/FableWin.pdb name sets hold ?Alloc@CQ_TentacleKrakenBossFightScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Label: the manifest's "Alloc" is generic (family J037: six one-CScriptThing quest-script factories).
// Callees: operator new 0x00BFEA1A (0x58 bytes), CScriptBase::CScriptBase 0x00CB8110 (??0CScriptBase@@QAE@XZ), CBaseClass::CBaseClass
//   0x0099A2D0 (??0CBaseClass@@QAE@XZ; the inline CScriptThing() then stores its vtable 0x01238C8C and zeroes PImp).
// Layout: Ego_r.pdb CQ_TentacleKrakenBossFightScript (0x6C; retail 0x58 = Ego_r - 0x14): CLevelScriptBase<T> {Game, MasterScriptData} at
//   retail +0x40 / +0x44, TheKraken (CScriptThing) Ego_r +0x60 -> retail +0x4C.
// Model: the J036 constructor of the same class made inline (agent952 H037 shape); local classes -- Review at landing.
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
    CQ_TentacleKrakenBossFightScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
        : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQ_TentacleKrakenBossFightScript();
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members[0x4C - 0x48];          // POD members up to the script thing
    CScriptThing  TheKraken;                            // +0x4C
};
CScriptBase* __fastcall CQ_TentacleKrakenBossFightScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_TentacleKrakenBossFightScript(data, game);
}
}