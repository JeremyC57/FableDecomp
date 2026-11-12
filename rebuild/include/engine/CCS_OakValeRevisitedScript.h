#ifndef FABLE_ENGINE_CCS_OAKVALEREVISITEDSCRIPT_H
#define FABLE_ENGINE_CCS_OAKVALEREVISITEDSCRIPT_H
// NEW header (agentFAM08). NScript::CCS_OakValeRevisitedScript -- the "CS_OakValeRevisited" quest script. Ego_r.pdb names: ??0 (PAVCScriptDataBase,
// AAVCGameScriptInterfaceBase), RegisterMain, OakValeFire, OakVale, Main, ??1, Alloc. Base CLevelScriptBase<CQ_SunnyvaleMasterData>: the
// script-info registration 0x00CDA1E1..0x00CDA227 pairs "CS_OakValeRevisited" with the script allocator 0x00EE8BE0 and the master-data
// allocator 0x00CDBD20 (CQ_SunnyvaleMasterData, `new` 0x144). Retail: allocator `new` 0xDC; constructor 0x00EE8100, vtable 0x012F53C0
// (??_G 0x00EE8C10, ??1 0x00EE8C30), RegisterMain 0x00EE8210, OakValeFire 0x00EE8870.
// Layout: the Ego_r.pdb record (sizeof 0xFC, members from 0x5C with Dinkumware containers) on the retail 0x48-byte CLevelScriptBase with
// 12-byte STLport vector / map: FirePoint +0x48, Fires +0x54, OakValeFlag +0x60, v1..v7 +0x6C..+0xCC -> 0xDC. Opaque map member (real type
// in the comment). Needs FABLE_RETAIL_NSCRIPT.
#include "rebuild_abi.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CCS_OakValeRevisitedScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CCS_OakValeRevisitedScript(CScriptDataBase*, CGameScriptInterfaceBase&);
    virtual ~CCS_OakValeRevisitedScript();
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&);
    virtual void RegisterMain();
    virtual void Main();
    void OakValeFire();
    void OakVale();
    std::vector<CScriptThing, std::allocator<CScriptThing> > FirePoint;   // +0x48
    std::vector<CScriptThing, std::allocator<CScriptThing> > Fires;       // +0x54
    unsigned char OakValeFlag[0xc];                                        // +0x60 map<CCharString,bool> (retail 12-byte STLport map)
    CScriptGameResourceObjectScriptedThingBase v1;                         // +0x6c
    CScriptGameResourceObjectScriptedThingBase v2;                         // +0x7c
    CScriptGameResourceObjectScriptedThingBase v3;                         // +0x8c
    CScriptGameResourceObjectScriptedThingBase v4;                         // +0x9c
    CScriptGameResourceObjectScriptedThingBase v5;                         // +0xac
    CScriptGameResourceObjectScriptedThingBase v6;                         // +0xbc
    CScriptGameResourceObjectScriptedThingBase v7;                         // +0xcc
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CCS_OakValeRevisitedScript) == 0xdc);
#endif
