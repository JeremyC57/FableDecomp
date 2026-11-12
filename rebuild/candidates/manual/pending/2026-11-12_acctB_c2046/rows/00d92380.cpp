// NScript::CQ_HobbeCaveScript::CHobbeTorrent::Main() -- hobbe torrent: once the nymph is angry or the brat is released, keeps spawning random hobbes ("HobbeRoaming") near itself off-screen until NumberToGenerate is used up, never more than HC_HobbeMaxNumberInRegion alive, then removes itself
// Leaf: ?Main@CHobbeTorrent@CQ_HobbeCaveScript@NScript@@EAEXXZ
// Inline-budget: shared header opt-in FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE (STLport erase(first, last) declared, not defined) --
//   retail calls vector<CScriptThing>::erase 0x008AC9D0 for `hobbes.clear()` (0x00D92554); with the inline body VC7.1 expands it (c2
//   inline trace tools/inl t01: c1 67 at static frequency 5.21, score 66 < 74). No loop spelling of the dead-hobbe wait lowers the
//   site's frequency enough (do-while 65, while(true)+break 64, if+do-while 68, `|| false` 72; it needs f <= ~2.1). The landed
//   CDFSummonsCreatures::Main 0x00E74D70 calls the same COMDAT through the same opt-in.
// Identity: Ego_r.pdb / FableWin.pdb ?Main@CHobbeTorrent@CQ_HobbeCaveScript@NScript@@EAEXXZ (private virtual); retail vtable 0x012D5960
//   (stored by the landed Alloc 0x00D8F3F0) slot 1 = 0x00D92380 (.rdata ref 0x012D5964).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D92380, 1468 bytes incl. the 5-entry jump table 0x00D9291C and the 12-byte index table
//   0x00D92930 (ends 0x00D9293C; int3 padding, then the uncatalogued CGranny::Main 0x00D92940, vtable 0x012D59E4 slot 1).
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30; CCharString 0x0099EBF0 / ~ 0x0099EAE0 / operator=(const char*)
//   0x0099EFE0; ~CScriptThing 0x004AA840; vector<CScriptThing>::erase(first, last) 0x008AC9D0 and ~vector 0x008AC970 (one exit);
//   IsDistanceBetweenThingsOver 0x00CBE3EA (__fastcall); rand 0x00BFEB16; pDSTGame 0x0143E8F8 timer slots 87..90 (inline CTimer);
//   pGlobalScriptDef 0x0143E90C (HC_HobbeMaxNumberInRegion +0xD08); CGameScriptInterfaceBase slots 7 NewScriptFrame, 70 GetHero,
//   75 GetAllThingsWithScriptName, 92 CreateCreatureNearby, 108 RemoveThing, 421 IsCameraPosOnScreen; CScriptThing slots 6 GetPos,
//   76 IsDead. Strings "HobbeRoaming" 0x012D5808, "CREATURE_HOBBE_GRUNT" 0x012D5BD4, "..._SPELLCASTER" 0x012D5C40,
//   "..._LIEUTENANT" 0x012D5C24, "..._GRUNT_SPIKE" 0x012D5C08, "..._GRUNT_HAMMER" 0x012D5BEC.
// Layout: engine/CQ_HobbeCaveScript.h (overlay opt-in FABLE_RETAIL_CQ_HOBBECAVESCRIPT_CHOBBETORRENT): parent ReleaseBrat +0x4f,
//   NymphAngry +0x6c (Ego_r.pdb - 0x14); NumberToGenerate +0x1c.
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CQ_HOBBECAVESCRIPT_CHOBBETORRENT
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include <stdlib.h>
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CQ_HobbeCaveScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CTimer.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_def_global.h"
#include "stlport/fable_containers.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

void NScript::CQ_HobbeCaveScript::CHobbeTorrent::Main()
{
    while (!ParentClass->NymphAngry && !ParentClass->ReleaseBrat) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CTimer spawn_timer;
    std::vector<CScriptThing> hobbes;
    while (NumberToGenerate > 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Game->GetAllThingsWithScriptName("HobbeRoaming", hobbes);
        while (hobbes.size() > pGlobalScriptDef->HC_HobbeMaxNumberInRegion) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            bool one_dead = false;
            while (!one_dead) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                for (unsigned long i = 0; i < hobbes.size(); ++i) {
                    if (IsActiveThreadTerminating()) return;
                    if (hobbes[i].IsDead()) {
                        if (IsActiveThreadTerminating()) return;
                        one_dead = true;
                    }
                }
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            hobbes.clear();
            Game->GetAllThingsWithScriptName("HobbeRoaming", hobbes);
        }
        if (IsActiveThreadTerminating()) return;
        if (spawn_timer.GetTime() == 0 && IsDistanceBetweenThingsOver(Me, Game->GetHero(), 8.0f) && !Game->IsCameraPosOnScreen(Me.GetPos())) {
            if (IsActiveThreadTerminating()) return;
            CCharString creature("CREATURE_HOBBE_GRUNT");
            switch (rand() % 12) {
            case 0:
                creature = "CREATURE_HOBBE_SPELLCASTER";
                break;
            case 1:
                creature = "CREATURE_HOBBE_LIEUTENANT";
                break;
            case 2:
            case 3:
            case 4:
                creature = "CREATURE_HOBBE_GRUNT_SPIKE";
                break;
            case 5:
            case 6:
                creature = "CREATURE_HOBBE_GRUNT_HAMMER";
                break;
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
                creature = "CREATURE_HOBBE_GRUNT";
                break;
            }
            Game->CreateCreatureNearby(creature, Me.GetPos(), 5.0f, "HobbeRoaming", false);
            --NumberToGenerate;
            spawn_timer = 2;
        }
    }
    if (IsActiveThreadTerminating()) return;
    Game->RemoveThing(Me, false, true);
}
