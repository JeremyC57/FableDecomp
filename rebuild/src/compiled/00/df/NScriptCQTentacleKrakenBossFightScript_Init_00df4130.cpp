// NScript::CQ_TentacleKrakenBossFightScript::Init() -- clear MissionSucceeded, MissionFailed, KrakenAutoSaveCheckPoint and the master data's MotherDodgingKraken
// Leaf: ?Init@CQ_TentacleKrakenBossFightScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF4130, 21 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 (Init) of the quest vtable 0x012DE510 (slot 1 = `NScript::CQ_TentacleKrakenBossFightScript::RegisterMain`), stored by the quest ctor 0x00DF40F0; ?Init@CQ_TentacleKrakenBossFightScript@NScript@@UAEXXZ is in Ego_r.pdb and FableWin.pdb. The quest Alloc 0x00DF5C00 stores the same vtable.
// Label: the manifest's "sub_00df4130" is a gap-scan placeholder.
// Layout: retail CScriptBase is 0x40 bytes (Ego_r.pdb 0x54), then the interface +0x40 and the master data +0x44; the quest's own members sit 0x14 below their Ego_r.pdb offsets until the first STL container. MissionSucceeded +0x48, MissionFailed +0x49, KrakenAutoSaveCheckPoint +0x4a; CQ_SunnyvaleMasterData::MotherDodgingKraken +0x84.
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
class CGameScriptInterfaceBase;
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_SunnyvaleMasterData {
public:
    unsigned char _pad000[0x84];
    bool MotherDodgingKraken;                           // +0x84 (Ego_r.pdb offset, same in retail)
};
class CQ_TentacleKrakenBossFightScript : public CScriptBase {
public:
    virtual void Init();
    CGameScriptInterfaceBase* Game;                     // +0x40 (the ctor's CGameScriptInterfaceBase&; descriptive name)
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44 (the ctor's CScriptDataBase*; descriptive name)
    bool MissionSucceeded;                              // +0x48
    bool MissionFailed;                                 // +0x49
    bool KrakenAutoSaveCheckPoint;                      // +0x4a
};
}

void NScript::CQ_TentacleKrakenBossFightScript::Init()
{
    MissionSucceeded = false;
    MissionFailed = false;
    KrakenAutoSaveCheckPoint = false;
    PMasterData->MotherDodgingKraken = false;
}