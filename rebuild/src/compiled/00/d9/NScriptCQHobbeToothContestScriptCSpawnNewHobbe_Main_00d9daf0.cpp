// NScript::CQ_HobbeToothContestScript::CSpawnNewHobbe::Main() -- the hobbe-tooth contest spawner: wait out the drinking hobbes, then until the outro starts, each frame count the live Q_HobbeToothHobbe things and, while neither the hero nor the whisper is within 10 of the spawner and fewer than three hobbes are about, spawn a grunt (3 in 4), else a spellcaster (3 in 4), else a lieutenant at the spawner, set it on the nearer of whisper and hero, and pause 8 s
// Leaf: ?Main@CSpawnNewHobbe@CQ_HobbeToothContestScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D9DAF0, 1693 bytes, thiscall, plain `ret` (two epilogue copies), frame 0x94.
// Identity (proven): Ego_r.pdb ?Main@CSpawnNewHobbe@CQ_HobbeToothContestScript@NScript@@EAEXXZ; slot 1 of the CSpawnNewHobbe entity
//   vtable 0x012D6BB8 stored by the landed Alloc 0x00D9CE80 (GetParentScript 0x00D9CF20 slot 3).
// Callees: IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CBaseClass 0x0099A2D0 / ~ 0x0099A2E0 (CScriptThing
//   default ctor and dtor expanded), ~CScriptThing 0x004AA840, ~vector<CScriptThing> 0x008AC970 (exit pad; expanded with free 0x00BFEA14
//   at the loop end), rand 0x00BFEB16, IsDistanceBetweenThingsUnder 0x00CBE2FF / GetSquaredDistanceBetweenThings 0x00CBE583 (__fastcall,
//   both PDBs), CGameScriptInterfaceBase slots 7 NewScriptFrame, 70 GetHero, 72 GetThingWithScriptName, 75 GetAllThingsWithScriptName,
//   91 CreateCreature, 376 Pause, 451 GiveThingBestEnemyTarget; CScriptThing::GetPos (slot 6).
// Layout: engine/CQ_HobbeToothContestScript.h (opt-in CSPAWNNEWHOBBE): ParentClass +0x14 (CS_OutroBegin +0x59, DrinkingHobbesNotHit +0x5a).
// Shape: the spawn test is one `&&` chain over the hero / whisper distance temporaries and the vector size (flag-guarded temporary
//   dtors, result in bl); the three CreateCreature arms assign one default-built NewHobbe. Retail expands the CScriptThing assignment
//   (and its CCountedPointer ShareData body) in all three arms: a row-local `__forceinline CScriptThing::operator=` (the established
//   chunk 805 lever, as the landed 0x00D52E90 / 0x00D8F040 / 0x00D7D8D0) does that; with the implicit or a plain-inline operator=
//   VC7.1 calls ??4CScriptThing in arms 2 and 3. The vector dtor is left inline (expanded at the loop end, called at the exit pad as
//   retail); a declared-only ~vector specialization moved the CS_OutroBegin exit tail.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQ_HOBBETOOTHCONTESTSCRIPT_CSPAWNNEWHOBBE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_HobbeToothContestScript.h"
#include "engine/C3DVector.h"
#include "engine/fable_script_distance.h"
#include "stlport/fable_containers.h"
float FABLE_FASTCALL GetSquaredDistanceBetweenThings(const CScriptThing&, const CScriptThing&);   // 0x00CBE583
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
FABLE_FORCEINLINE CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }

void NScript::CQ_HobbeToothContestScript::CSpawnNewHobbe::Main()
{
    while (ParentClass.DrinkingHobbesNotHit) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass.CS_OutroBegin) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        std::vector<CScriptThing> AllSpawnedHobbes;
        Game->GetAllThingsWithScriptName("Q_HobbeToothHobbe", AllSpawnedHobbes);
        if (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 10.0f)
            && !IsDistanceBetweenThingsUnder(Game->GetThingWithScriptName("Q_HobbeToothWhisper"), Me, 10.0f)
            && AllSpawnedHobbes.size() < 3) {
            if (IsActiveThreadTerminating())
                return;
            CScriptThing NewHobbe;
            if (rand() % 4 > 0) {
                if (IsActiveThreadTerminating())
                    return;
                NewHobbe = Game->CreateCreature("CREATURE_HOBBE_GRUNT", Me.GetPos(), "Q_HobbeToothHobbe", false);
            }
            else if (rand() % 4 > 0) {
                if (IsActiveThreadTerminating())
                    return;
                NewHobbe = Game->CreateCreature("CREATURE_HOBBE_SPELLCASTER", Me.GetPos(), "Q_HobbeToothHobbe", false);
            }
            else {
                if (IsActiveThreadTerminating())
                    return;
                NewHobbe = Game->CreateCreature("CREATURE_HOBBE_LIEUTENANT", Me.GetPos(), "Q_HobbeToothHobbe", false);
            }
            float HeroDistance = GetSquaredDistanceBetweenThings(NewHobbe, Game->GetHero());
            float WhisperDistance = GetSquaredDistanceBetweenThings(NewHobbe, Game->GetThingWithScriptName("Q_HobbeToothWhisper"));
            if (WhisperDistance < HeroDistance) {
                if (IsActiveThreadTerminating())
                    return;
                Game->GiveThingBestEnemyTarget(NewHobbe, Game->GetThingWithScriptName("Q_HobbeToothWhisper"));
            }
            else {
                if (IsActiveThreadTerminating())
                    return;
                Game->GiveThingBestEnemyTarget(NewHobbe, Game->GetHero());
            }
            Game->Pause(8.0f);
        }
    }
    if (IsActiveThreadTerminating())
        return;
}