// NScript::CV_ExposeMayorScript::~CV_ExposeMayorScript() -- implicit quest-script destructor: destroy Evidence, then seh_Rhodri, seh_Sheriff, seh_LadyGrey and seh_Suspicious in reverse order, then ~CScriptBase
// Leaf: ??1CV_ExposeMayorScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7B2E0, 261 bytes, thiscall, tail jump to ~CScriptBase 0x00CBD510; sole caller the
//   scalar deleting dtor 0x00E7B2C0 (slot 0 of the RTTI-less quest vtable 0x012E96CC).
// Identity (proven): vtable 0x012E96CC is stored only by the ctor 0x00E755F0, whose only caller is the Alloc 0x00E7B290
//   (`push 0A0h ; call new ; ... call 0x00E755F0`); CScriptInfoManager::RegisterScripts stores that Alloc at 0x00CD8667 right
//   after `push "S_VEM"` (0x012F7734); refs/script_recovery/conversion_queue.tsv agrees (V_ExposeMayor / S_VEM / 0x00E7B290).
//   Ego_r.pdb carries ??0CV_ExposeMayorScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z. No vtable
//   store in the body = compiler-generated dtor. Catalog label right.
// Layout: Ego_r.pdb CV_ExposeMayorScript at retail offsets (Ego_r - 0x14, no STLport container): Path +0x48 (PATH_ID),
//   StageNumber +0x4c (STAGE_IN_PATH), six bools +0x50..+0x55, seh_Suspicious +0x58, seh_LadyGrey +0x68, seh_Sheriff +0x78,
//   seh_Rhodri +0x88, Evidence +0x98, MaxFlashDistFromStables +0x9c (0xA0 = the Alloc size).
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunk 665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt"). No
//   engine/CCharString.h exists: CCharString is the one-pointer local model the landed quest rows use.
// Callees: ~CCharString 0x0099EAE0, the seh_* implicit dtors inline (PImp release: DeleteFunc slot, operator delete
//   0x00BFE9BC; vtable 0x0126008C store, ~CBaseObject 0x0099A430), ~CScriptBase 0x00CBD510.
// TU: the PDB ctor is defined (retail keeps it out of line at 0x00E755F0), which emits the vtable, ??_G and this implicit dtor.
void operator delete(void* p);                          // 0x00BFE9BC
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
template <class T> class CLevelScriptBase : public CScriptBase {
public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
    T* GetMasterData();
    CGameScriptInterfaceBase* Game;                                      // +0x40
    T*                        MasterScriptData;                          // +0x44
};
class CCharStringData;
class CCharString { public:
    ~CCharString();                                                      // 0x0099EAE0
    CCharStringData* PStringData;
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ExposeMayorScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum PATH_ID { };                                                    // Ego_r nested enum (Transfer<PATH_ID>); enumerators not in the dump
    enum STAGE_IN_PATH { };                                              // Ego_r nested enum (Transfer<STAGE_IN_PATH>)
    CV_ExposeMayorScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    PATH_ID                                    Path;                     // +0x48
    STAGE_IN_PATH                              StageNumber;              // +0x4c
    bool                                       HeroFlashedLightsCorrectly;   // +0x50
    bool                                       PrisonerTalkedAboutLamp;  // +0x51
    bool                                       ResetLamp;                // +0x52
    bool                                       SaidYesToBecomeMayor;     // +0x53
    bool                                       QuestCardGiven;           // +0x54
    bool                                       MayorExposeGhostCutscenePlayed;   // +0x55
    CScriptGameResourceObjectScriptedThingBase seh_Suspicious;           // +0x58
    CScriptGameResourceObjectScriptedThingBase seh_LadyGrey;             // +0x68
    CScriptGameResourceObjectScriptedThingBase seh_Sheriff;              // +0x78
    CScriptGameResourceObjectScriptedThingBase seh_Rhodri;               // +0x88
    CCharString                                Evidence;                 // +0x98
    float                                      MaxFlashDistFromStables;  // +0x9c
};
CV_ExposeMayorScript::CV_ExposeMayorScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00E755F0
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_ExposeMayorScript) == 0xA0);