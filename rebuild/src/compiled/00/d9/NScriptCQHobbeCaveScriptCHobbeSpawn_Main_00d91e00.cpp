// NScript::CQ_HobbeCaveScript::CHobbeSpawn::Main() -- Hobbe Cave alarm spawner: take the number of hobbes to generate from the def (after the brat's release) or from the marker's data string (less the def's side-room reduction once the side room is cleared), capped by the quest's HobbesToSpawn; then, while any are left and the alarm is raised, every HC_HobbeSpawnDelay seconds wait until fewer than HC_HobbeMaxNumberInRegion "HobbeRoaming" hobbes are alive and, outside cutscenes, off camera and more than 8 from the hero, spawn a "HobbeRoaming" hobbe grunt nearby set on the hero; finally take one from the quest's HobbesToSpawn
// Leaf: ?Main@CHobbeSpawn@CQ_HobbeCaveScript@NScript@@EAEXXZ
// Inline-budget: folded `&& true` on the terminate check at the top of the dead-hobbe scan loop -- it halves that return arm's
//   static frequency so its ~vector<CScriptThing> is called (retail 0x00D92020 `jne 0x00D922B7`, the 0x008AC970 pad) while the
//   check after the loop keeps the expanded copy (retail 0x00D9207A -> 0x00D922CA). c2 inline trace (tools/inl.sh, c2.dll
//   0x1078D1AA): ~vector c1 44, threshold 74; that site scores 64 without the fold (expanded) and 74 with it (called); all
//   thirteen other ~vector sites and all fifteen ~CTimer sites already take retail's decision (CTimer called only at the
//   spawn's terminate exit, retail 0x00D922FF). Retail's construct for that one site is unproven.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D91E00, 1399 bytes, thiscall, plain `ret` (six epilogue copies), frame 0x2C.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?Main@CHobbeSpawn@CQ_HobbeCaveScript@NScript@@EAEXXZ (private virtual, entity
//   slot 1); the landed CHobbeSpawn Alloc 0x00D8F310 / scalar deleting dtor 0x00D8F3D0 are the same entity class (Ego_r.pdb layout
//   ParentClass +0x14, PMasterData +0x18, NumberToGenerate +0x1c).
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30; pDSTGame 0x0143E8F8 slots 87 RegisterTimer / 88 DeregisterTimer /
//   89 SetTimer / 90 GetTimer (CTimer ctor, dtor, operator=, GetTime inline; ~CTimer 0x00CD4470 out of line at one exit);
//   Game slots 7 NewScriptFrame, 70 GetHero, 75 GetAllThingsWithScriptName, 92 CreateCreatureNearby, 421 IsCameraPosOnScreen,
//   451 GiveThingBestEnemyTarget; CScriptThing slots 3 GetDataString, 6 GetPos, 76 IsDead; GFCharStringToInt 0x0099E7F0
//   (__fastcall, both PDBs); IsDistanceBetweenThingsOver 0x00CBE3EA; vector<CScriptThing> erase(first, last) 0x008AC9D0 (clear)
//   and ~vector 0x008AC970 (expanded with free 0x00BFEA14 at four exits); ~CScriptThing 0x004AA840; CCharString 0x0099EBF0 / ~ 0x0099EAE0.
// Data: pGlobalScriptDef 0x0143E90C HC_SpawnCountPostReleaseGSon +0xca0, HC_ReduceCountPostSideRoom +0xca4, HC_HobbeSpawnDelay +0xcbc,
//   HC_HobbeMaxNumberInRegion +0xd08 (engine/CScriptDef.h); CQ_HobbeCaveScript AlarmTriggered +0x4d, KilledSideRoomHobbes +0x4e,
//   ReleaseBrat +0x4f, HobbesToSpawn +0x64, InCutscene +0x68 (engine/CQ_HobbeCaveScript.h). Literals "HobbeRoaming" 0x012D5808,
//   "CREATURE_HOBBE_GRUNT" 0x012D5BD4.
// Shape: the generated-script form (TRetVoid _Fail_Val; `return _Fail_Val.Return();`): with it the first three terminate checks
//   keep their expanded DeregisterTimer exits in line (0x00D91E31 / 0x00D91E95) as retail; the plain `return;` moves them out of
//   line. The vector is cleared through the out-of-line erase(first, last) (FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CQ_HOBBECAVESCRIPT_CHOBBESPAWN_MAIN
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_HobbeCaveScript.h"
#include "engine/CTimer.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/fable_script_distance.h"
#include "engine/TRetVoid.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
long FABLE_FASTCALL GFCharStringToInt(const CCharString&);   // 0x0099E7F0
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

void NScript::CQ_HobbeCaveScript::CHobbeSpawn::Main()
{
    TRetVoid _Fail_Val;
    CTimer SpawnTimer;
    if (ParentClass.ReleaseBrat) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        NumberToGenerate = pGlobalScriptDef->HC_SpawnCountPostReleaseGSon;
    } else if (ParentClass.KilledSideRoomHobbes) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        NumberToGenerate = GFCharStringToInt(Me.GetDataString()) - pGlobalScriptDef->HC_ReduceCountPostSideRoom;
    } else {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        NumberToGenerate = GFCharStringToInt(Me.GetDataString());
    }
    if (NumberToGenerate > ParentClass.HobbesToSpawn) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        NumberToGenerate = ParentClass.HobbesToSpawn;
    }
    std::vector<CScriptThing> Hobbes;
    while (NumberToGenerate > 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (ParentClass.AlarmTriggered) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (SpawnTimer.GetTime() == 0) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game->GetAllThingsWithScriptName("HobbeRoaming", Hobbes);
                while (Hobbes.size() > (unsigned long)pGlobalScriptDef->HC_HobbeMaxNumberInRegion) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    bool OneDied = false;
                    while (!OneDied) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        for (unsigned long i = 0; i < Hobbes.size(); ++i) {
                            if (IsActiveThreadTerminating() && true)
                                return _Fail_Val.Return();
                            if (Hobbes[i].IsDead()) {
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                                OneDied = true;
                            }
                        }
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    Hobbes.clear();
                    Game->GetAllThingsWithScriptName("HobbeRoaming", Hobbes);
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (!ParentClass.InCutscene && !Game->IsCameraPosOnScreen(Me.GetPos()) && IsDistanceBetweenThingsOver(Me, Game->GetHero(), 8.0f)) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    CScriptThing Hobbe = Game->CreateCreatureNearby("CREATURE_HOBBE_GRUNT", Me.GetPos(), 5.0f, "HobbeRoaming", false);
                    Game->GiveThingBestEnemyTarget(Hobbe, Game->GetHero());
                    NumberToGenerate--;
                }
                SpawnTimer = pGlobalScriptDef->HC_HobbeSpawnDelay;
            }
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    if (ParentClass.HobbesToSpawn > 1) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        ParentClass.HobbesToSpawn--;
    }
}