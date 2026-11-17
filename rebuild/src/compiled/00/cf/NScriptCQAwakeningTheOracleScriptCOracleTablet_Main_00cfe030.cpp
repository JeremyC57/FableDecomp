// NScript::CQ_AwakeningTheOracleScript::COracleTablet::Main() -- an Oracle digging spot in Awakening the Oracle: wait for the digging-spot rewards, mark Y/M/C/cursed tablet spots on the minimap, keep GRAVEYARD_SPARKLES_GRAVE alive (4 s timer) while the spot is enabled and retire it once three tablets are collected; when dug, give the Y/M/C expression for the tablet the hero carries (crumbling sound, info text, ++TabletsCollected) or, for the cursed BAD_ONE tablet, show the cursed text once and spawn creatures at the ATO_Enemy spawn points up to ATO_MaxSpawnedCreatures, then remove the cursed tablets; finally drop the spot from DiggingPoints and fade the sparkles out
// Grade: RELOCATION_MATCH (3575v3575) -- wfD2 successor 3: the last residue (SIB base/index order of the two CreateCreature-arm spawn_points[i] addresses, 0x00CFEC29/2C and 0x00CFECD7/DA) closes with ONE dead release assert (WF_RULES device d) before `AlreadyDugHere = true;`.
// Inferred: 1 dead release assert (FABLE_RETAIL_ASSERT through the overlay fable_assert.h, FABLE_RETAIL_ASSERT_DEAD_ARM; FITTED condition, place and count). It emits no bytes and no data
//   (its literals are the row's own strings; the stringized text goes with the deleted arm) and cannot change behaviour. Its dead IL (8 inline CCharString text ==
//   expansions) adds the c2 records that lift the loop-strength-reduction temp's sort key above the spawn_points._M_start field record (NOTES.md 0x00CFE030 "THIRD pass":
//   T key id = (64*C + 22) mod 2048 v S = 653), which gives retail's offset-first `[edi+ecx]` / `[edi+eax]`. Measured windows at this place: one assert with 8 == terms
//   (7 no), 2 x 4 terms, 4 copies of the row's own BAD_ONE / GOOD_ONE test, 18..40 one-term asserts; the same asserts at the top of Main change live code (inline budget).
// FITTED (inherited from the earlier chunks, chosen by grading): `const CScriptThing* sparkles_ptr = &sparkles;` (keeps IsAlive virtual; the reference-bound
//   `CScriptThing& sparkles = Game->CreateEffect(...)` grades the same but binds a non-const reference to a temporary, an MSVC extension), the final erase loop as
//   for(;;)+break, FABLE_FORCEINLINE CTimer::operator= / GetTime. EVIDENCED: identity (Ego_r), call targets, opt-ins; CTimer ctor text = landed 0x00CD4450 shape.
// Leaf: ?Main@COracleTablet@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CFE030, 3575 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): Ego_r.pdb ?Main@COracleTablet@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ; the body reads ParentClass +0x14
//   (GivenDiggingSpotsRewards +0x58, TabletsCollected +0x4c, GotY/M/C +0x6d..+0x6f, DisplayedBadTabletMessage +0x64,
//   TotalSpawnedCreaturesAlive +0x68, DiggingPoints +0x74) and AlreadyDugHere +0x1c, and calls the private RemoveCursedTablets 0x00CFEE30.
// Callees: IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / ~ 0x0099EAE0 / operator!=(const char*) 0x0099E960,
//   CBasicString<char>::Compare 0x00411570 (inline text ==), CCountedPointer<CScriptThing>::operator= 0x00704580, ~CScriptThing 0x004AA840,
//   CScriptThing::GetDataString 0x004AA900 (devirtualised on the local), vector<CScriptThing>::erase 0x00CD3152 / ~vector 0x008AC970,
//   FadeOutAndKillEntity 0x00CBE716 (__fastcall), ~CTimer 0x00CD4470 (cold exits), ~CBaseClass 0x0099A2E0 (in the one expanded dtor).
// Shape: `Me.GetDataString() == "BAD_ONE" || ... == "GOOD_ONE"` needs FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE + _CCHARSTRING_FORCEINLINE_TEXT_EQ;
//   sparkles.IsAlive() stays virtual (`call [edx+12Ch]`) only through a pointer to the local; CScriptThing::operator== spelled
//   `PImp->IsEqualTo(*other.PImp)` (as 0x008AAE20) reads Me's Data as [ebp+0Ch] and frees ebx (the `PImp.Data->` spelling CSEs &Me into
//   ebx, spills the spawn-loop offset and grows the frame by 4).
// Chunk 821: a row-local plain `inline CScriptThing::~CScriptThing() { }` plus the final DiggingPoints erase loop written as a
//   `for (;;) { if (it == end()) break; ... ++it; }` loop: a guarded pointer `for` leaks its body frequency to the code after it
//   (c2 static estimate, INLINE_DEPTH_REVISIT.md), which made VC7.1 expand ~CScriptThing at the final exit too; the for(;;)-break
//   form does not leak, so only the in-loop-1 / post-loop exits expand (shared block 0x00CFE3E3) and the final tail calls 0x004AA840.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CQ_AWAKENINGTHEORACLESCRIPT_CORACLETABLET
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#include "fable_string.h"
#include "fable_assert.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_AwakeningTheOracleScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CTimer.h"
#include "engine/CScriptDef.h"
extern CGameScriptInterfaceBase* pDSTGame;               // 0x0143E8F8
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>; agentNF60: was a local CScriptDef model and `CScriptDef* pGlobalScriptDef`)
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
template<> CCountedPointer<CScriptThing>& CCountedPointer<CScriptThing>::operator=(const CCountedPointer<CScriptThing>&);
template<> std::vector<CScriptThing, std::allocator<CScriptThing> >::~vector();
void FABLE_FASTCALL FadeOutAndKillEntity(const CScriptThing&, float, bool, bool);                     // 0x00CBE716
inline CScriptThing::~CScriptThing() { }

void NScript::CQ_AwakeningTheOracleScript::COracleTablet::Main()
{
    while (!ParentClass.GivenDiggingSpotsRewards) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (Me.GetDataString() == "BAD_ONE" || Me.GetDataString() == "GOOD_ONE") {
        if (IsActiveThreadTerminating()) return;
        Game->MiniMapAddMarker(Me, "HUD_ORB_GREEN_SMALL");
    }
    CScriptThing sparkles = Game->CreateEffect("GRAVEYARD_SPARKLES_GRAVE", Me.GetPos(), "", 0.0f, false, true);
    const CScriptThing* sparkles_ptr = &sparkles;
    CTimer sparkle_timer;
    bool removed = false;
    while (Game->IsDiggingSpotEnabled(Me)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (sparkle_timer.GetTime() == 0 && !sparkles_ptr->IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            sparkles = Game->CreateEffect("GRAVEYARD_SPARKLES_GRAVE", Me.GetPos(), "", 0.0f, false, true);
            sparkle_timer = 4;
        }
        if (!removed && ParentClass.TabletsCollected > 2) {
            if (IsActiveThreadTerminating()) return;
            Game->MiniMapRemoveMarker(Me);
            removed = true;
            for (std::vector<CScriptThing>::iterator it = ParentClass.DiggingPoints.begin(); it != ParentClass.DiggingPoints.end(); ++it) {
                if (Me == *it) {
                    ParentClass.DiggingPoints.erase(it);
                    break;
                }
            }
            FadeOutAndKillEntity(sparkles, 1.0f, true, true);
            Game->RemoveThing(Me, false, true);
        }
    }
    if (IsActiveThreadTerminating()) return;
    Game->MiniMapRemoveMarker(Me);
    if (!AlreadyDugHere && Me.GetDataString() != "BAD_ONE") {
        if (IsActiveThreadTerminating()) return;
        if (!ParentClass.GotY && Game->IsObjectInThingsPossession("OBJECT_SS_EXPRESSION_TABLET_Y", Game->GetHero())) {
            if (IsActiveThreadTerminating()) return;
            Game->GiveHeroExpression("EXPRESSION_Y", -1, false);
            ParentClass.GotY = true;
            Game->TakeObjectFromHero("OBJECT_SS_EXPRESSION_TABLET_Y");
            Game->PlaySoundOnThing(Me, "SND_MM_GLYPH_PICK_UP_CRUMBLING");
            Game->DisplayGameInfo("TEXT_QST_B03_TABLET_INFO_Y");
            while (!Game->MsgIsGameInfoClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            ++ParentClass.TabletsCollected;
        } else if (!ParentClass.GotM && Game->IsObjectInThingsPossession("OBJECT_SS_EXPRESSION_TABLET_M", Game->GetHero())) {
            if (IsActiveThreadTerminating()) return;
            Game->GiveHeroExpression("EXPRESSION_M", -1, false);
            ParentClass.GotM = true;
            Game->TakeObjectFromHero("OBJECT_SS_EXPRESSION_TABLET_M");
            Game->PlaySoundOnThing(Me, "SND_MM_GLYPH_PICK_UP_CRUMBLING");
            Game->DisplayGameInfo("TEXT_QST_B03_TABLET_INFO_M");
            while (!Game->MsgIsGameInfoClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            ++ParentClass.TabletsCollected;
        } else if (!ParentClass.GotC && Game->IsObjectInThingsPossession("OBJECT_SS_EXPRESSION_TABLET_C", Game->GetHero())) {
            if (IsActiveThreadTerminating()) return;
            Game->GiveHeroExpression("EXPRESSION_C", -1, false);
            ParentClass.GotC = true;
            Game->TakeObjectFromHero("OBJECT_SS_EXPRESSION_TABLET_C");
            Game->PlaySoundOnThing(Me, "SND_MM_GLYPH_PICK_UP_CRUMBLING");
            Game->DisplayGameInfo("TEXT_QST_B03_TABLET_INFO_C");
            while (!Game->MsgIsGameInfoClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            ++ParentClass.TabletsCollected;
        }
    } else if (!AlreadyDugHere && Me.GetDataString() == "BAD_ONE") {
        if (IsActiveThreadTerminating()) return;
        Game->PlaySoundOnThing(Me, "SND_MM_GLYPH_PICK_UP_CRUMBLING");
        if (!ParentClass.DisplayedBadTabletMessage) {
            if (IsActiveThreadTerminating()) return;
            Game->DisplayGameInfo("TEXT_QST_B03_TABLET_INFO_CURSED");
            while (!Game->MsgIsGameInfoClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            ParentClass.DisplayedBadTabletMessage = true;
        }
        CScriptThing nearest = Game->GetNearestWithScriptName(Me, "ATO_Enemy");
        std::vector<CScriptThing> spawn_points;
        Game->GetAllThingsWithScriptName(nearest.GetDataString(), spawn_points);
        if (spawn_points.size() != 0) {
            if (IsActiveThreadTerminating()) return;
            for (unsigned long i = 0; i < spawn_points.size(); ++i) {
                if (IsActiveThreadTerminating()) return;
                if (ParentClass.TotalSpawnedCreaturesAlive > pGlobalScriptDef->ATO_MaxSpawnedCreatures) {
                    if (IsActiveThreadTerminating()) return;
                    break;
                }
                if (spawn_points[i].GetDataString() != "NULL" && spawn_points[i].GetDataString() != "") {
                    if (IsActiveThreadTerminating()) return;
                    Game->CreateCreature(spawn_points[i].GetDataString(), spawn_points[i].GetPos(), "NecropolisEnemy", true);
                } else {
                    if (IsActiveThreadTerminating()) return;
                    Game->CreateCreature("CREATURE_MINION_DREADWINGS", spawn_points[i].GetPos(), "NecropolisEnemy", true);
                }
            }
            if (IsActiveThreadTerminating()) return;
        }
        RemoveCursedTablets();
    }
    // Inferred: dead release assert (FITTED; see the header): a digging spot's data string is none of the script's other names.
    FABLE_RETAIL_ASSERT(!(Me.GetDataString() == "EXPRESSION_Y" || Me.GetDataString() == "EXPRESSION_M" || Me.GetDataString() == "EXPRESSION_C" || Me.GetDataString() == "ATO_Enemy" || Me.GetDataString() == "NecropolisEnemy" || Me.GetDataString() == "CREATURE_MINION_DREADWINGS" || Me.GetDataString() == "HUD_ORB_GREEN_SMALL" || Me.GetDataString() == "GRAVEYARD_SPARKLES_GRAVE"));
    AlreadyDugHere = true;
    std::vector<CScriptThing>::iterator it = ParentClass.DiggingPoints.begin();
    for (;;) {
        if (it == ParentClass.DiggingPoints.end())
            break;
        if (Me == *it) {
            ParentClass.DiggingPoints.erase(it);
            break;
        }
        ++it;
    }
    FadeOutAndKillEntity(sparkles, 1.0f, true, true);
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}