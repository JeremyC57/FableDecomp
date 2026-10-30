// NScript::CQ_HobbeCaveScript::CheckIfGotTreasure() -- the Hobbe Cave quest's "CheckIfGotTreasure" spawned thread: each time the main chamber region is loaded, look up the two treasure chests HC_Treasure1 / HC_Treasure2 and, while the region stays loaded, count every chest the hero uses (GotTreasure) and have the sacrificial bandit comment on it (TEXT_QST_036_BANDIT_GOT_TREASURE_ONE / _TWO); runs until the thread is terminated
// Leaf: ?CheckIfGotTreasure@CQ_HobbeCaveScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D99A30, 911 bytes, thiscall, plain `ret`.
// Identity: Ego_r.pdb ?CheckIfGotTreasure@CQ_HobbeCaveScript@NScript@@QAEXXZ (0x00DB72C0); the HobbeCave spawner stores
//   0x00D99A30 at +0x34 of a CSpawnedFunc<CQ_HobbeCaveScript> (vtable 0x012D567C) it names "CheckIfGotTreasure"
//   (0x012D6488) at 0x00D9806F; `ret` with no arguments = QAEXXZ. Manifest label "Script_Bandit_Sacrifice" is wrong.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0,
//   CQ_HobbeCaveScript::MakeBanditComment 0x00D905C0, ~CScriptThing 0x004AA840 (the two named chests), CBaseClass dtor
//   0x0099A2E0 and operator delete 0x00BFE9BC (the inline ~CScriptThing of the bandit temporary); Game slots 7
//   NewScriptFrame, 12 IsRegionLoaded, 72 GetThingWithScriptName (ghidra_out/cgsi_slots_aligned.tsv); the chests'
//   CScriptThing slot 20 MsgIsUsedBy (inline wrapper, out of line 0x004AAB10).
// Data: "HobbeCaveMainChamber" 0x012D55EC, "HC_Treasure1" 0x012D65BC, "HC_Treasure2" 0x012D65AC, "SCRIPT_NAME_HERO"
//   0x0125D1C8, "BanditSacrifice" 0x012D5788, "TEXT_QST_036_BANDIT_GOT_TREASURE_ONE" 0x012D6584, "..._TWO" 0x012D655C.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, GotTreasure +0x58 (Ego_r +0x6C), BanditComment +0x54.
// Shape: ~CScriptThing is __forceinline (the GetThingWithScriptName temporary expands it); the two named chests are
//   destroyed out of line at every exit and at the end of their block, written under `#pragma inline_depth(0)`.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                          // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::MsgIsUsedBy(const CCharString& user) const { if (!PImp.Data) return false; return PImp.Data->MsgIsUsedBy(user); }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
namespace NScript {
class CQ_HobbeCaveScript : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    void*                     PMasterData;              // +0x44
    bool DoneIntro;                                     // +0x48
    bool SavedBeforeCaves;                              // +0x49
    bool BratInCutscene;                                // +0x4a
    bool MissionSucceeded;                              // +0x4b
    bool MissionFailed;                                 // +0x4c
    bool AlarmTriggered;                                // +0x4d
    bool KilledSideRoomHobbes;                          // +0x4e
    bool ReleaseBrat;                                   // +0x4f
    bool BratReturned;                                  // +0x50
    CTimer BanditComment;                               // +0x54
    long GotTreasure;                                   // +0x58
    long BanditHitCount;                                // +0x5c
    bool MakeBanditComment(const CCharString& line, const CScriptThing& bandit);   // 0x00D905C0
    void CheckIfGotTreasure();
};
}

void NScript::CQ_HobbeCaveScript::CheckIfGotTreasure()
{
    while (!IsActiveThreadTerminating()) {
        while (!Game->IsRegionLoaded("HobbeCaveMainChamber")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CScriptThing treasure1 = Game->GetThingWithScriptName("HC_Treasure1");
            CScriptThing treasure2 = Game->GetThingWithScriptName("HC_Treasure2");
            while (Game->IsRegionLoaded("HobbeCaveMainChamber")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                if (treasure1.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating())
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    ++GotTreasure;
                    MakeBanditComment("TEXT_QST_036_BANDIT_GOT_TREASURE_ONE", Game->GetThingWithScriptName("BanditSacrifice"));
                }
                if (treasure2.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating())
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    ++GotTreasure;
                    MakeBanditComment("TEXT_QST_036_BANDIT_GOT_TREASURE_TWO", Game->GetThingWithScriptName("BanditSacrifice"));
                }
            }
            if (IsActiveThreadTerminating())
#pragma inline_depth(0)
                return;
        }
#pragma inline_depth()
        Game->NewScriptFrame();
    }
}