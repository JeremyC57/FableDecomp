// NScript::CQ_GuildTrainingPreMeleeScript::CPreMeleeWhisper::Main() -- the pre-melee guide Whisper: waits for the whisper cut-scene, follows the hero, and when he talks to her or stands at a numbered PreMeleeChatMarker within 7 units of both (level, timer run out) removes that marker and says its guide line
// Leaf: ?Main@CPreMeleeWhisper@CQ_GuildTrainingPreMeleeScript@NScript@@EAEXXZ
// Identity: Ego_r.pdb / FableWin.pdb ?Main@CPreMeleeWhisper@CQ_GuildTrainingPreMeleeScript@NScript@@EAEXXZ (private virtual); retail
//   vtable 0x012CF874 slot 1 = 0x00D524A0, stored by the landed Alloc 0x00D52230 (slot 0 the landed scalar deleting dtor 0x00D522F0,
//   slot 2 the landed Init 0x00D522E0, slot 3 GetParentScript 0x00D522D0).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D524A0, 2540 bytes incl. the 19-entry switch jump table at 0x00D52E40 (cases 2..20); no EH frame.
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30; CBaseObject 0x0099A380 (inline seh_me ctor), seh_me IsValid 0x00CD23B9 /
//   Release 0x00CD2770 / FollowThing 0x007E7320 / ClearCommands 0x007E7360 / GetScriptThing 0x007E7490 / Speak 0x007E7390 /
//   IsPerformingScriptTask 0x007E7450 / dtor 0x007E74D0; movie ctor 0x006E7B60 / dtor 0x006E7B80; CCharString 0x0099EBF0 / ~ 0x0099EAE0;
//   ~CScriptThing 0x004AA840, CScriptThing::GetDataString 0x004AA900 (the local marker, devirtualised; slot 3 on the vector elements);
//   GFCharStringToInt 0x0099E7F0 (__fastcall); IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall); ~vector<CScriptThing> 0x008AC970;
//   ~CTimer 0x00CD4470 (called at two exits, DeregisterTimer expanded at two); pDSTGame 0x0143E8F8 timer slots 87..90; NullCoord 0x0143E8E0;
//   CGameScriptInterfaceBase slots 7, 8, 70, 75, 77, 108, 264, 364, 365, 366, 370, 379, 517; CScriptThing slots 3, 6, 27, 67.
// Layout: engine/CQ_GuildTrainingPreMeleeScript.h (FABLE_RETAIL_CQ_GUILDTRAININGPREMELEESCRIPT): parent WhisperCutsceneFinished +0x4a,
//   WhisperStopFollowing +0x55 (Ego_r.pdb - 0x14).
// Shape: the BarrelThug entity model; the marker number of each vector element is a named local compared after its CCharString dies
//   (a2: `if (GFCharStringToInt(...) == n)` set the flag before the dtor, 245 lines -> RELOCATION_MATCH). Case order = source order =
//   the descending literal addresses (LIBRARY 0x012CFB70 .. DINING_ROOM 0x012CF8AC).
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CQ_GUILDTRAININGPREMELEESCRIPT
#include <math.h>
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CQ_GuildTrainingPreMeleeScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/fable_script_distance.h"
#include "stlport/fable_containers.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
long FABLE_FASTCALL GFCharStringToInt(const CCharString&);   // 0x0099E7F0
template<> std::vector<CScriptThing>::~vector();              // 0x008AC970
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
__forceinline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }

void NScript::CQ_GuildTrainingPreMeleeScript::CPreMeleeWhisper::Main()
{
    Game->EntitySetAsKillable(Me, false, true);
    Me.SetFriendsWithEverythingFlag(true);
    while (!ParentClass.WhisperCutsceneFinished) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CTimer ChatTimer;
    ChatTimer = 0;
    seh_me.FollowThing(Game->GetHero(), 1.0f, true);
    while (!ParentClass.WhisperStopFollowing) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            seh_me.ClearCommands();
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_CHAT", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
            seh_me.FollowThing(Game->GetHero(), 1.0f, true);
        }
        CScriptThing marker = Game->GetNearestWithScriptName(Me, "PreMeleeChatMarker");
        float height_difference = marker.GetPos().Z - Game->GetHero().GetPos().Z;
        if (IsDistanceBetweenThingsUnder(Me, marker, 7.0f) && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 7.0f) &&
            ChatTimer.GetTime() <= 5 && fabs(height_difference) <= 1.0f) {
            if (IsActiveThreadTerminating()) return;
            long marker_number = GFCharStringToInt(marker.GetDataString());
            std::vector<CScriptThing> markers;
            Game->GetAllThingsWithScriptName("PreMeleeChatMarker", markers);
            for (unsigned long i = 0; i < markers.size(); ++i) {
                if (IsActiveThreadTerminating()) return;
                long number = GFCharStringToInt(markers[i].GetDataString());
                if (number == marker_number) {
                    if (IsActiveThreadTerminating()) return;
                    Game->RemoveThing(markers[i], false, true);
                }
            }
            if (IsActiveThreadTerminating()) return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            ChatTimer = 10;
            switch (marker_number) {
            case 2: Game->AddLineToConversation(conversation, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_LIBRARY", false, Me, Game->GetHero()); break;
            case 4: Game->AddLineToConversation(conversation, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SHOP", false, Me, Game->GetHero()); break;
            case 5: Game->AddLineToConversation(conversation, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_CLOISTERS", false, Me, Game->GetHero()); break;
            case 6: Game->AddLineToConversation(conversation, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE1", false, Me, Game->GetHero()); break;
            case 7: Game->AddLineToConversation(conversation, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE2", false, Me, Game->GetHero()); break;
            case 8: Game->AddLineToConversation(conversation, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE3", false, Me, Game->GetHero()); break;
            case 9: Game->AddLineToConversation(conversation, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE4", false, Me, Game->GetHero()); break;
            case 10: Game->AddLineToConversation(conversation, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_MAZE", false, Me, Game->GetHero()); break;
            case 11: Game->AddLineToConversation(conversation, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_WILL", false, Me, Game->GetHero()); break;
            case 13: Game->AddLineToConversation(conversation, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_WOODS", false, Me, Game->GetHero()); break;
            case 14: Game->AddLineToConversation(conversation, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SKILL", false, Me, Game->GetHero()); break;
            case 15: Game->AddLineToConversation(conversation, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SERVANTS", false, Me, Game->GetHero()); break;
            case 16: Game->AddLineToConversation(conversation, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_MAIN_DORM", false, Me, Game->GetHero()); break;
            case 19: Game->AddLineToConversation(conversation, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_DOOR", false, Me, Game->GetHero()); break;
            case 20: Game->AddLineToConversation(conversation, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_DINING_ROOM", false, Me, Game->GetHero()); break;
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
}