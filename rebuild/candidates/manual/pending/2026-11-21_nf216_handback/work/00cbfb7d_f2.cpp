// _global::RunCutsceneMacro(const CCharString& name, map<CCharString, CScriptGameResourceObjectScriptedThingBase>& actors, map<CCharString, bool>* flags, map<CCharString, CCharString>* substitutions, bool setup, bool skippable) -- run a cutscene def's macro: one ASCII `[Actor].Verb arg,arg,...` line per frame step, dispatched by a strncmp verb chain
// Inline-budget: FABLE_FORCEINLINE header twins, each measured here (without it): FABLE_RETAIL_C3DVECTOR_CTOR_FORCEINLINE (DIFFER 75282v74778), FABLE_RETAIL_CRGBCOLOUR_CTOR_FORCEINLINE (74672v74778), FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQUALSCI (73965v74778), FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ (74452v74778), StartsWith forced inside the STARTSWITH_ASSERT opt-in (EVIDENCED, fable_string.h); the caller-size budget those sites share is set by the dead asserts below. Devices (all marked where they stand): 36 dead release asserts (FABLE_RETAIL_ASSERT( occurs 36 times = grep -c 'FABLE_RETAIL_ASSERT('; `// Inferred:` block above RunCutsceneMacro; conditions INFERRED as facts the verb code relies on, count and places FITTED), FABLE_RETAIL_CCHARSTRING_STARTSWITH_ASSERT (fable_string.h: one dead FABLE_RETAIL_ASSERT_ARM arm in StartsWith, the non-empty-prefix check `prefix.PStringData != 0`; FITTED carrier of the caller-size budget, FableWin's later debug StartsWith has no assert; forceinline EVIDENCED), row-local inline CSPositionY (descriptive name; FITTED carrier of an EVIDENCED x87 order), FITTED named locals `fraction = 0.0f` (.SlideTeleport / SlideTeleport; value is a dead store), `offset` (CameraFOVLookBetweenPos) and hoisted `frames` (CrowdAnimate), descriptive globals g_CutsceneSoundFrame (0x013B83C8) / g_CutsceneMusicOverridden (0x0143E8FD), header opt-ins FABLE_RETAIL_C3DVECTOR_CTOR_FORCEINLINE / FABLE_RETAIL_CRGBCOLOUR_CTOR_FORCEINLINE. CCutsceneDef from the shared engine/CCutsceneDef.h. No flag, no inline pragma.
// Matched by wfC1 (rounds 0-2): RELOCATION_MATCH, 74,778 bytes, 20,435 instructions, 4,068 calls, frame 0x18DC.
#pragma optimize("s", on)
// Leaf: ?RunCutsceneMacro@@YIXABVCCharString@@AAV?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@4@@std@@PAV?$map@VCCharString@@_NU?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@_N@std@@@3@@3@PAV?$map@VCCharString@@V1@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@V1@@std@@@3@@3@_N4@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBFB7D, 74,778 bytes, `__fastcall` global (name in ecx, the actor map in edx,
//   four stack arguments: `ret 10h`), frame-pointer size TU (`lea ebp,[esp-68h]`, 0x18DC-byte frame through __chkstk).
// Name: Ego_r.pdb and FableWin.pdb ?RunCutsceneMacro@@YIX... (the Leaf above; both PDBs, same signature) + FSE FableAPI.cpp:183
//   tRunCutsceneMacro at 0xCBFB7D (ghidra_out/labels_fse_addresses.tsv); engine/fable_script_cutscene.h declares it.
// The last parameter is `skippable` in the Anniversary build's PDB (parameters macro_name, entitymap, flagmap, input_args, setupcond,
//   skippable) and goes to SetCutsceneMode(state, skippable) only.
// Locals are descriptive names (TLC's PDBs hold none). The bool / number locals that feed script calls are named after the callee's
//   parameter in the Anniversary build's PDB (weak third source: PlayAnimation(anim, stay_on_last_frame, overwrite_existing_actions,
//   add_as_queued_action, wait_for_anim_to_finish, use_physics, .., allow_looking), MoveToThing(target, proximity, move_type, wait,
//   avoid_dynamic_obstacles, ignore_path_preferability, face_movement), GiveHeroObject(item_name, quick_access_slot, silent), ...).
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define FABLE_RETAIL_CRGBCOLOUR_CTOR_FORCEINLINE
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH_ASSERT
#define FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQUALSCI
#define FABLE_RETAIL_CCHARSTRING_WHITESPACE
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_APPEND_CHAR
#define FABLE_RETAIL_CCHARSTRING_APPEND_COUNT
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#include "fable_string.h"
#include "fable_assert.h"
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_SET_FIND
#include "stlport/fable_map.h"
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CTOR_FORCEINLINE
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#define FABLE_RETAIL_GSIBASE_ENDLETTERBOX_PUBLIC
#include "engine/CGameScriptInterface.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "engine/CDefPointer.h"
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY
#include "engine/CDefinitionManager.h"
#include "engine/fable_script_cutscene.h"
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CASuspendableProcess.h"
#include "engine/CSuspendableProcessMicrothread.h"
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER_CUTSCENE
#include "engine/CPlayerGui.h"
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_text.h"
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CScreenFilterSThingByPass.h"
#define FABLE_RETAIL_CSYMBOLMAP
#include "engine/CSymbolMap.h"

#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/CCutsceneDef.h"                                                // CCutsceneDef (shared header: def->Macro / SkipCond / SetupCond / Sound)

typedef std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> CCutsceneActorMap;

CDefinitionManager* GFGetPDefinitionManager();                                   // 0x00CD3EF0
extern CGameScriptInterfaceBase* pDSTGame;                                        // 0x0143E8F8
extern long g_CutsceneSoundFrame;                                                 // 0x013B83C8 (descriptive)
extern bool g_bSkipCutsceneWithActionButton;                                      // 0x0143E8F4 (landed spelling, 0x00CBEB7E)
extern bool g_CutsceneMusicOverridden;                                            // 0x0143E8FD (descriptive)
extern CSuspendableProcessMicrothread* g_pScriptRunner;                           // 0x013D2838 (landed spelling, reloc_bindings.tsv; the running script microthread)
// Same-TU helpers: retail compiles these before RunCutsceneMacro in the same file, so VC7.1 knows they only read through
// the references they get (a parameter string, the parameter vector) and keeps locals in registers across the calls.
bool __fastcall IsTrue(const CCharString& text)                                   // 0x00CBEDBA
{
    return text.EqualsCaseInsensitive("true");
}
bool __fastcall IsFalse(const CCharString& text)                                  // 0x00CBEE0C
{
    return text.EqualsCaseInsensitive("false");
}
bool __fastcall IsNull(const CCharString& text)                                   // 0x00CBEE5E
{
    return text.EqualsCaseInsensitive("null");
}
// The skip button: the action-mode button when skipping with it is enabled, else the cutscene-skip message (landed 0x00CBEB7E).
bool __fastcall IsCutsceneSkipped()                                               // 0x00CBEB7E
{
    if (g_bSkipCutsceneWithActionButton) {
        if (pDSTGame->MsgIsActionModeButtonPressed())
            return true;
    } else {
        if (pDSTGame->MsgIsCutSceneSkipped())
            return true;
    }
    return false;
}
static inline bool IsScriptingActor(const CScriptGameResourceObjectScriptedThingBase* actor)
{
    return actor && actor->IsValid();
}
// A cutscene actor name as a script thing: "HERO" is the hero, a valid actor in the cast map gives its thing, anything else
// is looked up by script name (landed 0x00CBF9DE).
CScriptThing __fastcall CSFindThing(const CCharString& name, CCutsceneActorMap& actors)   // 0x00CBF9DE
{
    CScriptThing thing;
    if (name == "HERO") {
        thing = pDSTGame->GetHero();
    } else {
        CCutsceneActorMap::iterator it = actors.find(name);
        if (it != actors.end() && (*it).second.IsValid() && IsScriptingActor(&(*it).second))
            thing = (*it).second.GetScriptThing();
        else
            thing = pDSTGame->GetThingWithScriptName(name);
    }
    return thing;
}
CScriptThing __fastcall GetBestThingFromMarkerList(const std::vector<CScriptThing>&, const CScriptThing&, const CScriptThing&, bool);   // 0x00CBF13C (Ego_r.pdb + FableWin.pdb ?GetBestThingFromMarkerList@@YI?AVCScriptThing@@...)
void __fastcall InitializeMusicTypes();                                           // 0x00CBEB1A
void __fastcall PlayCutsceneSound(std::map<int, CCharString>&, int);               // 0x00CBF7FE
bool __fastcall CutsceneCameraPreloader(std::vector<CCharString>&, bool, long*);   // 0x00CBF29F
float __fastcall GFCharStringToFloat(const CCharString&);                         // 0x0099E690
long __fastcall GFCharStringToInt(const CCharString&);                            // 0x0099E7F0
long __fastcall GFStringToInt(const char*);                                       // 0x009E1960
float __fastcall GFStringToFloat(const char*);                                    // 0x009E18B0
double __fastcall GFGetTime();                                                    // 0x009E1BC0
extern CSymbolMap* g_pMusicTypeSymbolMap;                                         // 0x0143E900 (landed spelling, 0x00CBEB1A)
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
// Split a comma-separated parameter text into the vector (a trailing non-empty token too); returns the vector's size.
int __fastcall CSExtractParameterList(CCharString& text, std::vector<CCharString>& params)   // 0x00CBFACA (landed)
{
    CCharString current("");
    for (long i = 0; i < text.GetLength(); ++i) {
        if (((const char*)text)[i] == ',') {
            params.push_back(current);
            current = "";
        } else {
            current = current + ((const char*)text)[i];
        }
    }
    if (current.GetLength() > 0)
        params.push_back(current);
    return params.size();
}
// Split a "name:r,g,b" light line: the text before the colon is the name, each comma stores the parsed token through the
// current colour pointer (landed 0x00CBF050).
void __fastcall CutsceneMacro_ReadLightScene(CCharString& line, CCharString& name, unsigned char* red, unsigned char* green, unsigned char* blue)
{
    if (line.GetLength() == 0)
        return;
    long colon = line.FindCharPos(':');
    if (colon == -1)
        return;
    name = line.GetSubString(0, colon);
    CCharString token;
    unsigned char* dest = red;
    unsigned char state = 0;
    for (long pos = colon; pos < line.GetLength(); ++pos) {
        if (((const char*)line)[pos] == ',') {
            *dest = (unsigned char)GFCharStringToInt(token);
            switch (state) {
            case 0: dest = red;   state = 1; break;
            case 1: dest = green; state = 2; break;
            case 2: dest = blue;  state = 0; break;
            default: state = 0; break;
            }
        } else if (((const char*)line)[pos] != ' ') {
            token.Append(((const char*)line)[pos], token.GetLength());
        }
    }
}
//   // 0x00CBF050 (Ego_r.pdb + FableWin.pdb ?CutsceneMacro_ReadLightScene@@YIXAAVCCharString@@0PAE11@Z)
extern CWideString g_ScriptFormattedText;                                         // 0x0143E908 (landed descriptive spelling, reloc_bindings.tsv; 0x00E81B40 / 0x00E64FB0)
// 0040dce0 reads the pointer, rejects null, then tests CThing::Dead at +0x91 bit 0.
// FableWin's exact instantiation name is ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z.
// Keep the observed call out of line while exposing its real read-only body.
template <class T>
bool __fastcall IS_THING_ALIVE(const T& thing)
{
    if (!thing)
        return false;
    if (!thing->IsAlive())
        return false;
    return true;
}

// One cutscene frame: run a script frame when enabled, play this frame's sound cue and move the cue counter on.
#define CUTSCENE_FRAME()                                        \
    if (doScriptFrames)                                         \
        pDSTGame->NewScriptFrame();                             \
    PlayCutsceneSound(sounds, g_CutsceneSoundFrame);            \
    if (g_CutsceneSoundFrame != -1)                             \
        ++g_CutsceneSoundFrame

// Height of a position, read through a reference parameter (descriptive name; FITTED carrier of an EVIDENCED fact).
// Evidence: retail .SlideTeleport 0x00CC5962..0x00CC59A2 keeps the Y product of `step * fraction` in memory ([ebp-0x1424]) and adds
// it tmp-first (`fld [T.Y] ; fadd [start.Y]`, 0x00CC597E) while X / Z stay `fld [start.K] ; fadd st(n)`; VC7.1 emits that only when
// the caller never names start.Y as a float before the loop (its field record is then created late, at the inline fold). The twin
// SlideTeleport (0x00CC5C25) names all three. Only `start` goes through the helper: `end.Y` stays named in the caller, which keeps
// two more c2 auto records than a two-argument helper (measured; CrowdAnimate's `random` then opens its record batch, 0x00CCE8EA).
static inline float CSPositionY(const C3DVector& position) { return position.Y; }
// Inferred: 36 dead release asserts (FABLE_RETAIL_ASSERT, fable_assert.h; they emit no bytes and cannot change behaviour). Their
// texts are not known: FableWin.exe is not on this machine, and ghidra_out holds the retail decomp (runcutscenemacro_full.c) and the
// verb names (runcutscene_string_literals.txt) but no debug-build text of this function. Each condition states a fact the verb code
// relies on, in the shape the budget needs (one local against a constant or another local, 18 dead tuples each):
//   - a parsed magnitude is not negative (16: time, radius, seconds, frames, distance, speed, phase, delay, interval, duration) or
//     not zero where a zero line would do nothing (GiveHeroMorality, GiveGold), and a parsed CrowdMove proximity is positive;
//   - CacheMusic / PlayMusic: the music-set symbol is a set or -1 (GetSymbolSLONG's not-found value, which the next line tests);
//   - CrowdAnimate / CrowdCombatAnimate: the member list is not empty (`rand() % memberCount` divides by it; the animate-for-time
//     branches also check totalTime > 0);
//   - in the other Crowd* member loops, the member index is below the crowd size CrowdCreate registered (11, incl. the round-robin
//     index of CrowdTeleportRipple), which is what makes `actors[name + index]` find a member.
// Count and places are FITTED to three measured c2 facts of retail:
//   - caller size (inline budget): CCharString::GetLength expanded at 0x00CC03EA and called from 0x00CC0510 on needs about 595..695
//     more caller tuples than the plain row; 36 asserts = 648 (c2 inline trace: margins +47 / -53 tuples, i.e. 34..38 asserts);
//   - record ids (an x87 commutative operand order is the descending order of record id mod 2048): 0x00CCB3C6 `fld offsetX ; fadd
//     lookAt.X` needs id(lookAt.X) - id(offsetX) in 1494..2047 (mod 2048), which falls by about 12 for each row assert standing before
//     the verb instead of after it (16 before -> measured in the window; none before -> 150);
//   - 0x00CCE8EA `fld duration ; fadd random` needs the id wrap between the record batch of CrowdAnimate's `duration` and the batch
//     `random` opens, and 0x00CCED7D `fld random ; fadd duration` no wrap between CrowdCombatAnimate's pair.
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneActorMap& actors, std::map<CCharString, bool>* flags,
                                 std::map<CCharString, CCharString>* substitutions, bool setup, bool skippable)
{
    std::vector<CScriptThing> removeAtEnd;               // things the macro created
    std::vector<CScriptThing> followers;                 // the hero's followers
    std::vector<CScriptThing> fadedFollowers;            // followers faded out for the cutscene (faded back in at the end)
    std::vector<CScriptThing> lookingThings;
    std::vector<CScriptThing> collideThings;
    CDefPointer<const CCutsceneDef> def;
    bool restoreFollowers = true;
    std::map<CCharString, long> crowdSizes;
    std::set<CCharString> registeredActors;
    std::vector<CScriptThing> creatures;
    std::vector<CScriptThing> removedExtras;
    std::vector<C3DVector> extraPositions;
    std::vector<float> extraAngles;
    bool crowdLooking = false;                           // a crowd was told to look at a thing (CrowdLookTo)
    bool displayOverScreenFades = false;
    bool keepActors = false;
    bool extrasRemoved = false;
    bool doScriptFrames = true;
    long conversation = -1;
    CCharString lastCamera;
    bool relightHandLamp = false;
    bool fadeInAtEnd = true;
    std::map<CCharString, long> givenObjects;
    std::map<CCharString, bool> localFlags;
    CCharString weaponState("NONE");
    std::vector<CScriptThing> engageThings;
    std::vector<CScriptThing> damageThings;
    bool flagWasSet = false;
    if (GFGetPDefinitionManager()->GetDef(name, &def)) {
        pDSTGame->SetCutsceneMode(true, skippable);
        pDSTGame->SetAllowScreenFadingIfAlreadyFaded(true);
        pDSTGame->EnableDangerMusic(false);
        g_CutsceneSoundFrame = -1;
        CCharString params[12];
        CCharString questName;
        float timeOfDay = -1.0f;
        bool stopMusicAtEnd = false;
        bool debugCamera = false;
        bool frameAfterCamera = true;
        bool frameAfterAnim = true;
        bool skipping = false;
        bool resetThemeAtEnd = false;
        g_CutsceneMusicOverridden = false;
        void* screenFilter = 0;
        questName = "NULL";
        std::vector<CCharString> soundLines;
        std::vector<CCharString> commands;
        std::map<int, CCharString> sounds;
        std::vector<long> unused;
        long answer = -1;
        bool answered = false;
        if (setup == true) {
            commands = def->SetupCond;
        } else {
            commands = def->Macro;
            if (commands[0].EqualsCaseInsensitive("FadeOut 0.5,0"))
                pDSTGame->FadeScreenOutUntilNextCallToFadeScreenIn(0.5f, 0.0f);
            CutsceneCameraPreloader(commands, true, &g_CutsceneSoundFrame);
        }
        pDSTGame->GetFollowingEntityList(pDSTGame->GetHero(), followers);
        pDSTGame->SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(true);
        pDSTGame->CancelHeroTeleportEffects();
        if (actors.find("HeroFollower0") == actors.end()) {
            for (unsigned int i = 0; i < followers.size(); ++i) {
                if (followers[i].IsAlive() && !pDSTGame->IsEntityUnderScriptedControl(followers[i])) {
                    fadedFollowers.push_back(followers[i]);
                    pDSTGame->EntityFadeOut(followers[i], 0.5f);
                }
            }
        }
        if (def->Sound.size() > 0) {
            soundLines = def->Sound;
            for (unsigned int j = 0; j < soundLines.size(); ++j) {
                long colon = soundLines[j].FindCharPos(':');
                if (colon != -1) {
                    CCharString frameText;
                    frameText = soundLines[j].GetSubString(0, colon);
                    frameText.RemoveWhiteSpaceBeginEnd();
                    if (colon + 1 < soundLines[j].GetLength()) {
                        CCharString soundName = soundLines[j].GetSubString(colon + 1, soundLines[j].GetLength());
                        soundName.RemoveWhiteSpaceBeginEnd();
                        int frame = GFStringToInt(frameText);
                        CCharString cue = sounds[frame];
                        if (cue.GetLength() != 0)   // 0x00CC0040: expanded by retail
                            cue = cue + "\n";
                        cue = cue + soundName;
                        sounds[frame] = cue;
                    }
                }
            }
        }
        if (pDSTGame->IsHeroHandLampLit()) {
            pDSTGame->SetHeroHandLampAsLit(false);
            relightHandLamp = true;
        }
        InitializeMusicTypes();

        for (long line = 0; line < commands.size(); ++line) {
            if (IsCutsceneSkipped() && !skipping) {
                if (pDSTGame->IsConversationActive(conversation))
                    pDSTGame->RemoveConversation(conversation, true);
                pDSTGame->NewScriptFrame();
                line = 0;
                commands.resize(0);
                if (def->SkipCond.size() > 0)
                    commands = def->SkipCond;
                skipping = true;
            }
            if (answer == 0) {
                pDSTGame->NewScriptFrame();
                line = 0;
                commands.resize(0);
                if (def->Answer0.size() > 0)
                    commands = def->Answer0;
                answer = -1;
                skipping = false;
                answered = true;
            } else if (answer == 1) {
                pDSTGame->NewScriptFrame();
                line = 0;
                commands.resize(0);
                if (def->Answer1.size() > 0)
                    commands = def->Answer1;
                answer = -1;
                skipping = false;
                answered = true;
            }
            if (commands.size() > 0) {
                CCharString command;
                CCharString actorName;
                long dot = commands[line].FindCharPos('.');
                long space = commands[line].FindCharPos(' ');
                if (dot != -1 && space > dot) {
                    command = commands[line].GetSubString(dot, commands[line].GetLength());
                    for (long c = 0; c < dot; ++c) {
                        if (commands[line][c] != ' ')
                            actorName += commands[line][c];
                    }
                } else {
                    if (space == -1)
                        space = commands[line].GetLength();
                    command = commands[line].GetSubString(0, space);
                }
                for (int k = 0; k < 10; ++k)
                    params[k] = "";
                if (space != -1) {
                    long c = space + 1;
                    long param = 0;
                    bool quoted = false;
                    for (; c < commands[line].GetLength(); ++c) {
                        if (commands[line][c] == '\'') {
                            quoted = !quoted;
                        } else if (commands[line][c] == ',' && !quoted) {
                            if (++param > 11)
                                param = 0;
                        } else if (commands[line][c] != ' ' || quoted == true) {
                            params[param] += commands[line][c];
                        }
                    }
                }
                if (substitutions) {
                    for (int k = 0; k < 11; ++k) {
                        if (params[k].GetLength() != 0) {
                            // 0x00CC03D2..0x00CC0435: a separate inner `if` (the empty-parameter edge `je 0x00CC04A0` leaves directly) whose
                            // condition is a logical `&&`: the CCharString("$") temporary is a conditional one (flag bit 1 at 0x00CC03F0 /
                            // 0x00CC0420) and the strncmp result is kept as a value in bl (0x00CC0418 `neg ebx ; sbb bl,bl ; inc bl`). The left
                            // operand costs no code: the pointer the block goes on to use, already tested at 0x00CC03B5 (c2 drops the re-test).
                            if (substitutions && params[k].StartsWith("$")) {   // 0x00CC03EA: prefix length expanded by retail
                                std::map<CCharString, CCharString>::iterator it;
                                it = substitutions->find(params[k]);
                                if (it != substitutions->end()) {
                                    if ((*it).second.GetLength() != 0)
                                        params[k] = CCharString((*it).second);
                                } else {
                                    params[k] = "NULL";
                                }
                            }
                        }
                    }
                }
                CScriptGameResourceObjectScriptedThingBase* actor = 0;
                CCutsceneActorMap::iterator found;
                found = actors.find(actorName);
                if (found != actors.end() && (*found).second.IsValid())
                    actor = &(*found).second;

                if (command.StartsWith(".SummonerAttack")) {
                    if (actor && params[0].GetLength()) {
                        CScriptThing target = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        actor->SummonerLightningOrbAttackTarget(target);
                    }
                }
                if (command.StartsWith(".DoBossFight")) {
                  if (actor && params[0].GetLength()) {
                    long phase = GFCharStringToInt(params[0]);
                    CScriptThing boss = actor->GetScriptThing();
                    actor->Release();
                    pDSTGame->EntitySetBossPhase(boss, phase);
                    long frames = 0;
                    if (!params[1].GetLength()) {
                        while (!IsCutsceneSkipped()) {
                            if (pDSTGame->EntityGetBossPhase(boss) != phase)
                                break;
                            CUTSCENE_FRAME();
                            ++frames;
                            if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                break;
                        }
                        if (frames > 1) {
                            pDSTGame->StartScriptingEntity(boss, *actor, (EScriptAIPriority)4);
                            actors[boss.GetName()] = *actor;
                        }
                    }
                  }
                }
                if (command.StartsWith(".WaitTask")) {
                    if (actor) {
                        while (actor->IsPerformingScriptTask()) {
                            if (IsCutsceneSkipped())
                                break;
                            CUTSCENE_FRAME();
                            if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                break;
                        }
                    }
                } else if (command.StartsWith(".WalkTo")) {
                    if (actor && params[0].GetLength()) {
                        float proximity = 0.3f;
                        bool wait = false;
                        bool avoidObstacles = true;
                        bool ignorePathPreferability = false;
                        bool faceMovement = true;
                        if (params[1].GetLength())
                            proximity = GFCharStringToFloat(params[1]);
                        if (IsTrue(params[3]))
                            wait = true;
                        if (IsFalse(params[4]))
                            avoidObstacles = false;
                        if (IsTrue(params[5]))
                            ignorePathPreferability = true;
                        if (IsFalse(params[6]))
                            faceMovement = false;
                        pDSTGame->EntitySetCutsceneBehaviour(actor->GetScriptThing(), (ECutsceneBehaviour)2);
                        actor->MoveToThing((CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]),
                                           proximity, (EScriptEntityMoveType)0, false, avoidObstacles, ignorePathPreferability, faceMovement);
                        if (IsTrue(params[2]) || wait) {
                            while (actor->IsPerformingScriptTask()) {
                                if (IsCutsceneSkipped())
                                    break;
                                CUTSCENE_FRAME();
                                if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                    break;
                            }
                        } else {
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith(".RunTo")) {
                    if (actor && params[0].GetLength()) {
                        float proximity = 0.3f;
                        bool wait = false;
                        bool avoidObstacles = true;
                        bool ignorePathPreferability = false;
                        bool faceMovement = true;
                        if (params[1].GetLength())
                            proximity = GFCharStringToFloat(params[1]);
                        if (IsTrue(params[3]))
                            wait = true;
                        if (IsFalse(params[4]))
                            avoidObstacles = false;
                        if (IsTrue(params[5]))
                            ignorePathPreferability = true;
                        if (IsFalse(params[6]))
                            faceMovement = false;
                        pDSTGame->EntitySetCutsceneBehaviour(actor->GetScriptThing(), (ECutsceneBehaviour)2);
                        actor->MoveToThing((CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]),
                                           proximity, (EScriptEntityMoveType)1, false, avoidObstacles, ignorePathPreferability, faceMovement);
                        if (IsTrue(params[2]) || wait) {
                            while (actor->IsPerformingScriptTask()) {
                                if (IsCutsceneSkipped())
                                    break;
                                CUTSCENE_FRAME();
                                if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                    break;
                            }
                        } else {
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith(".SneakTo")) {
                    if (actor && params[0].GetLength()) {
                        float proximity = 0.3f;
                        bool wait = false;
                        bool avoidObstacles = true;
                        bool ignorePathPreferability = false;
                        bool faceMovement = true;
                        if (params[1].GetLength())
                            proximity = GFCharStringToFloat(params[1]);
                        if (IsTrue(params[3]))
                            wait = true;
                        if (IsFalse(params[4]))
                            avoidObstacles = false;
                        if (IsTrue(params[5]))
                            ignorePathPreferability = true;
                        if (IsFalse(params[6]))
                            faceMovement = false;
                        pDSTGame->EntitySetCutsceneBehaviour(actor->GetScriptThing(), (ECutsceneBehaviour)2);
                        actor->MoveToThing((CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]),
                                           proximity, (EScriptEntityMoveType)2, false, avoidObstacles, ignorePathPreferability, faceMovement);
                        if (IsTrue(params[2]) || wait) {
                            while (actor->IsPerformingScriptTask()) {
                                if (IsCutsceneSkipped())
                                    break;
                                CUTSCENE_FRAME();
                                if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                    break;
                            }
                        } else {
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith(".SetFree")) {
                    if (actor) {
                        CScriptThing thing = actor->GetScriptThing();
                        pDSTGame->EntitySetAsRemoveAllMovementBlockingModes(thing);
                    }
                } else if (command.StartsWith(".SetAttackable")) {
                    if (actor) {
                        CScriptThing thing = actor->GetScriptThing();
                        pDSTGame->EntitySetAbleToBeEngagedInCombat(thing, false);
                        engageThings.push_back(thing);
                    }
                } else if (command.StartsWith(".SetDamageable")) {
                    if (actor) {
                        CScriptThing thing = actor->GetScriptThing();
                        pDSTGame->EntitySetAsDamageable(thing, false);
                        damageThings.push_back(thing);
                    }
                } else if (command.StartsWith(".SetPushable")) {
                    if (actor) {
                        bool pushable = false;
                        if (IsTrue(params[0]))
                            pushable = true;
                        CScriptThing thing = actor->GetScriptThing();
                        pDSTGame->SetIsPushableByHero(thing, pushable);
                    }
                } else if (command.StartsWith(".SetBound")) {
                    if (actor && params[0].GetLength()) {
                        bool bound = true;
                        if (IsFalse(params[0]))
                            bound = false;
                        CScriptThing thing = actor->GetScriptThing();
                        pDSTGame->EntitySetAsHavingBoundHands(thing, bound);
                    }
                } else if (command.StartsWith(".SetScared")) {
                    if (actor) {
                        bool scared = true;
                        if (IsFalse(params[0]))
                            scared = false;
                        pDSTGame->EntitySetAsScared(actor->GetScriptThing(), scared);
                    }
                } else if (command.StartsWith(".SetDrunk")) {
                    if (actor) {
                        bool drunk = true;
                        CScriptThing thing = actor->GetScriptThing();
                        if (IsFalse(params[0]))
                            drunk = false;
                        pDSTGame->EntitySetAsDrunk(thing, drunk);
                    }
                } else if (command.StartsWith(".PreloadAnim")) {
                    if (actor) {
                        CScriptThing thing = actor->GetScriptThing();
                        if (!params[0].GetLength() || params[0].EqualsCaseInsensitive("basic"))
                            pDSTGame->EntityBeginLoadingBasicAnimations(thing);
                        else
                            pDSTGame->EntityBeginLoadingAnimation(thing, params[0]);
                    }
                } else if (command.StartsWith(".PlayAnimation")) {
                    if (actor && params[0].GetLength()) {
                        bool stayOnLastFrame = false;
                        bool overwriteExistingActions = false;
                        bool addAsQueuedAction = false;
                        bool waitForAnimToFinish = true;
                        bool allowLooking = false;
                        if (IsTrue(params[1]))
                            stayOnLastFrame = true;
                        if (IsTrue(params[2]))
                            overwriteExistingActions = true;
                        if (IsTrue(params[3]))
                            addAsQueuedAction = true;
                        if (IsFalse(params[4]))
                            waitForAnimToFinish = false;
                        if (IsTrue(params[5]))
                            allowLooking = true;
                        actor->PlayAnimation(params[0], stayOnLastFrame, overwriteExistingActions, addAsQueuedAction, waitForAnimToFinish, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, allowLooking);
                        if (frameAfterAnim) {
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith(".PlayCombatAnim")) {
                    if (actor && params[0].GetLength()) {
                        bool overwriteExistingActions = false;
                        bool addAsQueuedAction = false;
                        bool waitForAnimToFinish = true;
                        bool usePhysics = true;
                        long repeat = 1;
                        bool allowLooking = false;
                        // Parsed like .PlayAnimation's stay-on-last-frame flag but never passed (PlayCombatAnimation has no such
                        // parameter): retail calls IsTrue(params[1]) at 0x00CC167B and drops the result (no test of al: the next instruction is the lea ecx,[ebp+30h] of params[2]).
                        bool stayOnLastFrame = false;
                        if (IsTrue(params[1]))
                            stayOnLastFrame = true;
                        if (IsTrue(params[2]))
                            overwriteExistingActions = true;
                        if (IsTrue(params[3]))
                            addAsQueuedAction = true;
                        if (IsFalse(params[4]))
                            waitForAnimToFinish = false;
                        if (IsFalse(params[5]))
                            usePhysics = false;
                        if (params[6].GetLength())
                            repeat = GFCharStringToInt(params[6]);
                        if (IsTrue(params[7]))
                            allowLooking = true;
                        for (long i = 0; i < repeat; ++i)
                            actor->PlayCombatAnimation(params[0], usePhysics, overwriteExistingActions, addAsQueuedAction, waitForAnimToFinish, false, allowLooking);
                        if (frameAfterAnim) {
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith(".PlayLoopingAnim")) {
                    if (actor && params[0].GetLength() && params[1].GetLength()) {
                        bool useMovement = false;
                        bool overwriteExistingActions = false;
                        bool addAsQueuedAction = false;
                        bool waitForAnimToFinish = true;
                        bool allowLooking = false;
                        if (IsTrue(params[2]))
                            useMovement = true;
                        if (IsTrue(params[3]))
                            overwriteExistingActions = true;
                        if (IsTrue(params[4]))
                            addAsQueuedAction = true;
                        if (IsFalse(params[5]))
                            waitForAnimToFinish = false;
                        if (IsTrue(params[6]))
                            allowLooking = true;
                        long loops = GFCharStringToInt(params[1]);
                        actor->PlayLoopingAnimation(params[0], loops, useMovement, overwriteExistingActions, addAsQueuedAction, waitForAnimToFinish,
                                                    NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, allowLooking);
                        if (frameAfterAnim) {
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith(".WaitPlayAnimation")) {
                    if (actor && params[0].GetLength()) {
                        bool overwriteExistingActions = false;
                        bool addAsQueuedAction = false;
                        bool waitForAnimToFinish = true;
                        if (IsTrue(params[1]))
                            overwriteExistingActions = true;
                        if (IsTrue(params[2]))
                            addAsQueuedAction = true;
                        if (IsFalse(params[3]))
                            waitForAnimToFinish = false;
                        if (IsTrue(params[4]))
                            actor->PlayCombatAnimation(params[0], true, overwriteExistingActions, addAsQueuedAction, waitForAnimToFinish, false, false);
                        else
                            actor->PlayAnimation(params[0], false, overwriteExistingActions, addAsQueuedAction, waitForAnimToFinish, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
                        while (actor->IsPerformingScriptTask() == true) {
                            if (IsCutsceneSkipped())
                                break;
                            CUTSCENE_FRAME();
                            if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                break;
                        }
                    }
                } else if (command.StartsWith(".FollowThing")) {
                    if (actor && params[0].GetLength()) {
                        CScriptGameResourceObjectScriptedThingBase* target = 0;
                        CCutsceneActorMap::iterator it;
                        it = actors.find(params[0]);
                        if (it != actors.end() && (*it).second.IsValid())
                            target = &(*it).second;
                        float distance = 1.0f;
                        if (params[1].GetLength())
                            distance = GFCharStringToFloat(params[1]);
                        if (target && target->IsValid())
                            actor->FollowThing(target->GetScriptThing(), distance, true);
                        else
                            actor->FollowThing(pDSTGame->GetThingWithScriptName(params[0]), distance, true);
                        CUTSCENE_FRAME();
                    }
                } else if (command.StartsWith(".StopFollowingThing")) {
                    if (actor) {
                        CScriptGameResourceObjectScriptedThingBase* target = 0;
                        CCutsceneActorMap::iterator it;
                        it = actors.find(params[0]);
                        if (it != actors.end() && (*it).second.IsValid())
                            target = &(*it).second;
                        if (target && target->IsValid())
                            actor->StopFollowingThing(target->GetScriptThing());
                        else
                            actor->StopFollowingThing(pDSTGame->GetThingWithScriptName(params[0]));
                        CUTSCENE_FRAME();
                    }
                } else if (command.StartsWith(".Killable")) {
                    if (actor && params[0].GetLength()) {
                        bool killable = true;
                        if (IsFalse(params[0]))
                            killable = false;
                        pDSTGame->EntitySetAsKillable(actor->GetScriptThing(), killable, true);
                    }
                } else if (command.StartsWith(".FightWith")) {
                    if (actor && params[0].GetLength()) {
                        CScriptGameResourceObjectScriptedThingBase* target = 0;
                        CCutsceneActorMap::iterator it;
                        it = actors.find(params[0]);
                        if (it != actors.end() && (*it).second.IsValid())
                            target = &(*it).second;
                        CScriptThing enemy = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        if (target && target->IsValid())
                            enemy = target->GetScriptThing();
                        CScriptThing thing = actor->GetScriptThing();
                        if (thing.IsAlive()) {
                            actor->Release();
                            pDSTGame->StartScriptingEntity(thing, *actor, (EScriptAIPriority)0);
                            if (actor->IsValid()) {
                                actors[thing.GetName()] = *actor;
                                pDSTGame->EntitySetThingAsEnemyOfThing(actor->GetScriptThing(), enemy);
                                CUTSCENE_FRAME();
                            }
                        }
                    }
                } else if (command.StartsWith(".FightStop")) {
                    if (actor) {
                        CScriptGameResourceObjectScriptedThingBase* target = 0;
                        CCutsceneActorMap::iterator it;
                        it = actors.find(params[0]);
                        if (it != actors.end() && (*it).second.IsValid())
                            target = &(*it).second;
                        CScriptThing ally = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        if (target && target->IsValid())
                            ally = target->GetScriptThing();
                        CScriptThing thing = actor->GetScriptThing();
                        if (thing.IsAlive()) {
                            actor->Release();
                            pDSTGame->StartScriptingEntity(thing, *actor, (EScriptAIPriority)4);
                            if (actor->IsValid()) {
                                actors[thing.GetName()] = *actor;
                                pDSTGame->EntitySetThingAsAllyOfThing(actor->GetScriptThing(), ally);
                                CUTSCENE_FRAME();
                            }
                        }
                    }
                } else if (command.StartsWith(".HoldInHand")) {
                    if (actor && params[0].GetLength()) {
                        bool rightHand = IsTrue(params[1]);
                        CScriptThing thing = actor->GetScriptThing();
                        pDSTGame->GiveThingItemInHand(thing, params[0], rightHand);
                        CUTSCENE_FRAME();
                    }
                } else if (command.StartsWith(".ModifyHealth")) {
                    if (actor && params[0].GetLength()) {
                        CScriptThing thing = actor->GetScriptThing();
                        float amount = GFCharStringToFloat(params[0]);
                        pDSTGame->ModifyThingHealth(thing, amount, false);
                        CUTSCENE_FRAME();
                    }
                } else if (command.StartsWith(".WalkUpToThing")) {
                    if (actor && params[0].GetLength() && params[1].GetLength()) {
                        CScriptThing target = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        long moveType = 0;
                        if (params[3].EqualsCaseInsensitive("run"))
                            moveType = 1;
                        CThing* pThing = target.GetPThing();
                        CTCPhysicsBase* physics;
                        if (pThing->GetTC(TCI_PHYSICS, &physics)) {
                            float distance = GFCharStringToFloat(params[1]);
                            actor->MoveToPosition(physics->GetPosition() + physics->GetRHSet().GetForward() * distance, 1.0f,
                                                  (EScriptEntityMoveType)moveType, false, true);
                        while (actor->IsPerformingScriptTask()) {
                            if (IsCutsceneSkipped())
                                break;
                            CUTSCENE_FRAME();
                            if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                break;
                        }
                            pDSTGame->EntitySetFacingAngleTowardsThing(actor->GetScriptThing(), target, true);
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith(".Speak")) {
                    if (actor && params[0].GetLength() && params[1].GetLength() && !IsNull(params[1])) {
                        ETextGroupSelectionMethod method = (ETextGroupSelectionMethod)0;
                        if (IsTrue(params[2]))
                            pDSTGame->FixMovieSequenceCamera(true);
                        if (params[3].EqualsCaseInsensitive("random"))
                            method = (ETextGroupSelectionMethod)1;
                        else if (params[3].EqualsCaseInsensitive("norepeat"))
                            method = (ETextGroupSelectionMethod)2;
                        else if (params[3].EqualsCaseInsensitive("sequence"))
                            method = (ETextGroupSelectionMethod)3;
                        CScriptGameResourceObjectScriptedThingBase* target = 0;
                        CCutsceneActorMap::iterator it;
                        it = actors.find(params[0]);
                        if (it != actors.end() && (*it).second.IsValid())
                            target = &(*it).second;
                        if (target && target->IsValid())
                            actor->Speak(target->GetScriptThing(), params[1], method, false, true, displayOverScreenFades);
                        else
                            actor->Speak((CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]), params[1], method, false, true, displayOverScreenFades);
                        while (actor->IsPerformingScriptTask()) {
                            if (IsCutsceneSkipped())
                                break;
                            CUTSCENE_FRAME();
                            if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                break;
                        }
                        if (IsTrue(params[2]))
                            pDSTGame->FixMovieSequenceCamera(false);
                    }
                } else if (command.StartsWith(".DataSpeak")) {
                    if (actor && params[0].GetLength() && params[1].GetLength()) {
                        CCharString key;
                        CScriptThing thing = actor->GetScriptThing();
                        if (!params[2].GetLength())
                            key = params[0] + params[1];
                        else if (params[2].EqualsCaseInsensitive("DATA"))
                            key = params[0] + thing.GetDataString() + params[1];
                        else
                            key = params[0] + params[2] + params[1];
                        ETextGroupSelectionMethod method = (ETextGroupSelectionMethod)0;
                        if (params[3].EqualsCaseInsensitive("random"))
                            method = (ETextGroupSelectionMethod)1;
                        else if (params[3].EqualsCaseInsensitive("norepeat"))
                            method = (ETextGroupSelectionMethod)2;
                        else if (params[3].EqualsCaseInsensitive("sequence"))
                            method = (ETextGroupSelectionMethod)3;
                        CCutsceneActorMap::iterator it;
                        it = actors.find(params[0]);
                        actor->Speak(thing, key, method, false, true, false);
                        while (actor->IsPerformingScriptTask()) {
                            if (IsCutsceneSkipped())
                                break;
                            CUTSCENE_FRAME();
                            if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                break;
                        }
                    }
                } else if (command.StartsWith(".InteractiveSpeakGroup")) {
                    if (actor && params[0].GetLength() && params[1].GetLength() && params[2].GetLength()) {
                        CScriptThing thing = actor->GetScriptThing();
                        CScriptThing listener = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        long groupConversation = pDSTGame->AddNewConversation(thing, true, true);
                        long lines = GFStringToInt(params[2]);
                        pDSTGame->AddPersonToConversation(groupConversation, listener);
                        for (long i = 0; i < lines; ++i) {
                            CCharString lineKey(params[1]);
                            lineKey += "_";
                            lineKey += GFIntToCharString((i + 1) * 10);
                            pDSTGame->AddLineToConversation(groupConversation, lineKey, true, thing, listener);
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith(".InteractiveSpeak")) {
                    if (actor && params[0].GetLength() && params[1].GetLength()) {
                        CScriptThing thing = actor->GetScriptThing();
                        CScriptThing listener = CSFindThing(params[0], actors);
                        conversation = pDSTGame->AddNewConversation(thing, true, true);
                        pDSTGame->AddPersonToConversation(conversation, listener);
                        pDSTGame->AddLineToConversation(conversation, params[1], true, thing, listener);
                        if (params[3].GetLength())
                            pDSTGame->AddLineToConversation(conversation, params[3], true, thing, listener);
                        if (params[4].GetLength())
                            pDSTGame->AddLineToConversation(conversation, params[4], true, thing, listener);
                        if (params[5].GetLength())
                            pDSTGame->AddLineToConversation(conversation, params[5], true, thing, listener);
                        if (params[6].GetLength())
                            pDSTGame->AddLineToConversation(conversation, params[6], true, thing, listener);
                        if (params[7].GetLength())
                            pDSTGame->AddLineToConversation(conversation, params[7], true, thing, listener);
                        if (params[8].GetLength())
                            pDSTGame->AddLineToConversation(conversation, params[8], true, thing, listener);
                        if (IsTrue(params[2])) {
                            while (pDSTGame->IsConversationActive(conversation)) {
                                if (IsCutsceneSkipped())
                                    break;
                                CUTSCENE_FRAME();
                                if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                    break;
                            }
                        } else {
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith(".DialogSpeak")) {
                    if (actor && params[0].GetLength() && params[1].GetLength() && !IsNull(params[1])) {
                        CScriptThing thing = actor->GetScriptThing();
                        CScriptThing listener = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        if (IsTrue(params[2]) || (conversation != -1 && !pDSTGame->IsConversationActive(conversation)))
                            conversation = -1;
                        if (conversation == -1)
                            conversation = pDSTGame->AddNewConversation(thing, true, true);
                        pDSTGame->AddPersonToConversation(conversation, listener);
                        pDSTGame->AddLineToConversation(conversation, params[1], true, thing, listener);
                        CUTSCENE_FRAME();
                    }
                } else if (command.StartsWith(".DialogadSpeak")) {
                    if (actor && params[0].GetLength() && params[1].GetLength()) {
                        ETextGroupSelectionMethod method = (ETextGroupSelectionMethod)0;
                        if (params[3].EqualsCaseInsensitive("random"))
                            method = (ETextGroupSelectionMethod)1;
                        else if (params[3].EqualsCaseInsensitive("norepeat"))
                            method = (ETextGroupSelectionMethod)2;
                        else if (params[3].EqualsCaseInsensitive("sequence"))
                            method = (ETextGroupSelectionMethod)3;
                        CScriptGameResourceObjectScriptedThingBase* target = 0;
                        CCutsceneActorMap::iterator it;
                        it = actors.find(params[0]);
                        if (it != actors.end() && (*it).second.IsValid())
                            target = &(*it).second;
                        if (target && target->IsValid())
                            actor->Speak(target->GetScriptThing(), params[1], method, false, true, false);
                        else
                            actor->Speak((CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]), params[1], method, false, true, false);
                    }
                } else if (command.StartsWith(".EntitySetMaxWalkingSpeed")) {
                    if (actor && params[0].GetLength()) {
                        float speed = GFCharStringToFloat(params[0]);
                        if (speed == 0.0f)
                            pDSTGame->EntityResetMaxWalkingSpeed(actor->GetScriptThing());
                        else
                            pDSTGame->EntitySetMaxWalkingSpeed(actor->GetScriptThing(), speed);
                        CUTSCENE_FRAME();
                    }
                } else if (command.StartsWith(".EntitySetMaxRunningSpeed")) {
                    if (actor && params[0].GetLength()) {
                        float speed = GFCharStringToFloat(params[0]);
                        if (speed == 0.0f)
                            pDSTGame->EntityResetMaxRunningSpeed(actor->GetScriptThing());
                        else
                            pDSTGame->EntitySetMaxRunningSpeed(actor->GetScriptThing(), speed);
                        CUTSCENE_FRAME();
                    }
                } else if (command.StartsWith(".Sheathe")) {
                    if (actor) {
                        CCharString mode(params[0]);
                        if (IsTrue(params[1])) {
                            if (weaponState == "MELEE" || weaponState == "RANGED")
                                weaponState = "NONE";
                            else
                                mode = "NONE";
                        }
                        if (mode.EqualsCaseInsensitive("melee"))
                            pDSTGame->EntityUnsheatheMeleeWeapon(actor->GetScriptThing(), false);
                        else if (mode.EqualsCaseInsensitive("ranged"))
                            pDSTGame->EntityUnsheatheRangedWeapon(actor->GetScriptThing(), false);
                        else if (mode.EqualsCaseInsensitive("false"))
                            pDSTGame->EntityUnsheatheWeapons(actor->GetScriptThing(), false);
                        else if (!mode.EqualsCaseInsensitive("none"))
                            pDSTGame->EntitySheatheWeapons(actor->GetScriptThing(), false);
                    }
                } else if (command.StartsWith(".Collide")) {
                    if (actor) {
                        CScriptThing thing = actor->GetScriptThing();
                        if (IsTrue(params[0])) {
                            pDSTGame->EntitySetAsCollidableToThings(thing, true);
                        } else {
                            pDSTGame->EntitySetAsCollidableToThings(thing, false);
                            if (!IsFalse(params[1]))
                                collideThings.push_back(thing);
                        }
                    }
                } else if (command.StartsWith(".LookToThing")) {
                    if (actor && params[0].GetLength()) {
                        CScriptThing target = CSFindThing(params[0], actors);
                        CScriptThing thing = actor->GetScriptThing();
                        if (target.IsAlive())
                            pDSTGame->EntityForceToLookAtThing(thing, target);
                        if (params[1].EqualsCaseInsensitive("forever"))
                            pDSTGame->SetLookForFiniteTime(thing, false);
                        if (IsFalse(params[1]))
                            pDSTGame->SetLookForFiniteTime(thing, true);
                        if (!IsFalse(params[2])) {
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith(".LookToCamera")) {
                    if (actor) {
                        bool look = true;
                        if (IsFalse(params[0]))
                            look = false;
                        CScriptThing thing = CSFindThing(params[0], actors);
                        pDSTGame->EntityForceToLookAtCamera(thing, look);
                    }
                } else if (command.StartsWith(".LookAtNothing")) {
                    if (actor) {
                        CScriptThing thing = actor->GetScriptThing();
                        if (IsFalse(params[0])) {
                            pDSTGame->EntityResetForceToLookAt(thing);
                        } else {
                            pDSTGame->EntityForceToLookAtNothing(thing);
                            if (!IsFalse(params[1]))
                                lookingThings.push_back(thing);
                        }
                    }
                } else if (command.StartsWith(".LookAt")) {
                    if (actor && params[0].GetLength()) {
                        CScriptThing target = CSFindThing(params[0], actors);
                        bool immediate = true;
                        bool frame = true;
                        if (IsFalse(params[1]))
                            immediate = false;
                        if (IsFalse(params[2]))
                            frame = false;
                        pDSTGame->EntitySetFacingAngleTowardsThing(actor->GetScriptThing(), target, immediate);
                        if (frame) {
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith(".LookInDirection")) {
                    if (actor && params[0].GetLength()) {
                        float angle = GFCharStringToFloat(params[0]) * (1.0f / 360.0f);
                        bool immediate = true;
                        if (IsFalse(params[1]))
                            immediate = false;
                        pDSTGame->EntitySetFacingAngle(actor->GetScriptThing(), angle, immediate);
                    }
                } else if (command.StartsWith(".WaitForUnderRadius")) {
                    if (actor && params[0].GetLength() && params[1].GetLength()) {
                        float radius = GFCharStringToFloat(params[1]);
                        while (!IsDistanceBetweenThingsUnder(actor->GetScriptThing(), (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]), radius) && !IsCutsceneSkipped()) {
                            CUTSCENE_FRAME();
                            if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                break;
                        }
                    }
                } else if (command.StartsWith(".WaitForAnimationEvent")) {
                    if (actor && params[0].GetLength() && !IsCutsceneSkipped()) {
                        CScriptThing thing = actor->GetScriptThing();
                        if (thing.IsAlive()) {
                            while (!thing.MsgOnCutSceneAnimEvent(params[0])) {
                                if (IsCutsceneSkipped())
                                    break;
                                CUTSCENE_FRAME();
                                if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                    break;
                            }
                        }
                    }
                } else if (command.StartsWith(".FollowNavRoute")) {
                    if (actor && params[0].GetLength()) {
                        long moveType = 0;
                        bool waitForRoute = false;
                        if (params[1].EqualsCaseInsensitive("run"))
                            moveType = 1;
                        if (params[1].EqualsCaseInsensitive("sneak"))
                            moveType = 2;
                        if (IsTrue(params[2]))
                            waitForRoute = true;
                        actor->FollowPreCalculatedRoute((CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]), (EScriptEntityMoveType)moveType, waitForRoute, false);
                        CUTSCENE_FRAME();
                    }
                } else if (command.StartsWith(".AILevel")) {
                    if (actor && params[0].GetLength()) {
                        CScriptThing thing = actor->GetScriptThing();
                        long priority = 4;
                        if (params[0].EqualsCaseInsensitive("HIGH"))
                            priority = 3;
                        else if (params[0].EqualsCaseInsensitive("MEDIUM"))
                            priority = 2;
                        if (thing.IsAlive()) {
                            actor->Release();
                            pDSTGame->StartScriptingEntity(thing, *actor, (EScriptAIPriority)priority);
                            actors[thing.GetName()] = *actor;
                        }
                    }
                } else if (command.StartsWith(".Release")) {
                    if (actor)
                        actor->Release();
                } else if (command.StartsWith(".Teleport")) {
                    if (actor && params[0].GetLength()) {
                        CScriptThing thing = actor->GetScriptThing();
                        CScriptThing target = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        bool forceNavigable = true;
                        if (IsFalse(params[1]))
                            forceNavigable = false;
                        if (thing.IsAlive() && target.IsAlive())
                            pDSTGame->EntityTeleportToPosition(thing, target.GetPos(), target.GetAngleXY(), forceNavigable, false);
                    }
                } else if (command.StartsWith(".TeleportInFrontOf")) {
                    if (actor && params[0].GetLength() && params[1].GetLength()) {
                        CScriptThing thing = actor->GetScriptThing();
                        if (thing.IsAlive()) {
                            CScriptThing target = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                            CThing* pThing = target.GetPThing();
                            CTCPhysicsBase* physics;
                            if (pThing->GetTC(TCI_PHYSICS, &physics)) {
                                float distance = GFCharStringToFloat(params[1]);
                                pDSTGame->EntityTeleportToPosition(thing, physics->GetPosition() + physics->GetRHSet().GetForward() * distance, 0.0f, false, false);
                                pDSTGame->EntitySetFacingAngleTowardsThing(actor->GetScriptThing(), target, true);
                                CUTSCENE_FRAME();
                            }
                        }
                    }
                } else if (command.StartsWith(".ResetPos")) {
                    if (actor) {
                        CScriptThing thing = actor->GetScriptThing();
                        if (thing.IsAlive())
                            pDSTGame->EntityTeleportToPosition(thing, thing.GetHomePos(), 0.0f, false, false);
                    }
                } else if (command.StartsWith(".SetAppearanceSeed")) {
                    if (actor) {
                        CScriptThing thing = actor->GetScriptThing();
                        long seed = GFCharStringToInt(params[0]);
                        if (thing.IsAlive())
                            pDSTGame->EntitySetAppearanceMorphSeed(thing, seed);
                    }
                } else if (command.StartsWith(".FadeIn")) {
                    if (actor && params[0].GetLength()) {
                        float time = GFCharStringToFloat(params[0]);
                        float to = 1.0f;
                        float from = 0.0f;
                        if (params[1].GetLength())
                            to = GFCharStringToFloat(params[1]);
                        if (params[2].GetLength())
                            from = GFCharStringToFloat(params[2]);
                        if (time > 0.0f) {
                            float step = (to - from) / (time * 15.0f);
                            for (float alpha = from; alpha < to; alpha += step) {
                                if (IsCutsceneSkipped())
                                    break;
                                pDSTGame->EntitySetAlpha(actor->GetScriptThing(), alpha, true);
                                CUTSCENE_FRAME();
                            }
                        }
                        pDSTGame->EntitySetAlpha(actor->GetScriptThing(), to, true);
                    }
                } else if (command.StartsWith(".AddScriptedMode")) {
                    if (actor && params[0].GetLength())
                        pDSTGame->AddCreatureScriptedMode(actor->GetScriptThing(), params[0]);
                } else if (command.StartsWith(".RemoveScriptedMode")) {
                    if (actor)
                        pDSTGame->RemoveCreatureScriptedMode(actor->GetScriptThing());
                } else if (command.StartsWith(".Drawable")) {
                    if (actor && params[0].GetLength()) {
                        bool drawable = true;
                        if (IsFalse(params[0]))
                            drawable = false;
                        pDSTGame->EntitySetAsDrawable(actor->GetScriptThing(), drawable);
                    }
                } else if (command.StartsWith(".SetAlpha")) {
                    if (actor && params[0].GetLength()) {
                        float alpha = GFCharStringToFloat(params[0]);
                        CScriptThing thing = actor->GetScriptThing();
                        pDSTGame->EntitySetAlpha(thing, alpha, true);
                    }
                } else if (command.StartsWith(".FadeOut")) {
                    if (actor && params[0].GetLength()) {
                        float time = GFCharStringToFloat(params[0]);
                        CScriptThing thing = actor->GetScriptThing();
                        CScriptThing other;
                        float from = 1.0f;
                        float to = 0.0f;
                        if (params[1].GetLength() && !IsNull(params[1]))
                            other = CSFindThing(params[1], actors);
                        if (params[2].GetLength())
                            from = GFCharStringToFloat(params[2]);
                        if (params[3].GetLength())
                            to = GFCharStringToFloat(params[3]);
                        if (time > 0.0f) {
                            float step = (from - to) / (time * 15.0f);
                            for (float alpha = from; alpha > to; alpha -= step) {
                                if (IsCutsceneSkipped())
                                    break;
                                if (thing.IsAlive())
                                    pDSTGame->EntitySetAlpha(thing, alpha, true);
                                if (other.IsAlive())
                                    pDSTGame->EntitySetAlpha(other, alpha, true);
                                CUTSCENE_FRAME();
                            }
                        }
                        pDSTGame->EntitySetAlpha(actor->GetScriptThing(), to, true);
                    }
                } else if (command.StartsWith(".FadeCross")) {
                    if (actor && params[0].GetLength() && params[1].GetLength()) {
                        CScriptThing thing = actor->GetScriptThing();
                        CScriptThing other;
                        CCutsceneActorMap::iterator it;
                        it = actors.find(params[0]);
                        if (it != actors.end() && (*it).second.IsValid())
                            other = (*it).second.GetScriptThing();
                        if (other.IsNull())
                            other = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        float step = 1.0f / (GFCharStringToFloat(params[1]) * 15.0f);
                        for (float alpha = 1.0f; alpha > 0.0f; alpha -= step) {
                            if (IsCutsceneSkipped())
                                break;
                            pDSTGame->EntitySetAlpha(thing, alpha, true);
                            pDSTGame->EntitySetAlpha(other, 1.0f - alpha, true);
                            CUTSCENE_FRAME();
                        }
                        pDSTGame->EntitySetAlpha(thing, 0.0f, true);
                        pDSTGame->EntitySetAlpha(other, 1.0f, true);
                    }
                } else if (command.StartsWith(".ClearCommands")) {
                    if (actor) {
                        actor->ClearAllActionsIncludingLoopingAnimations();
                        actor->ClearCommands();
                        if (!IsTrue(params[0])) {
                            CUTSCENE_FRAME();
                        }
                        if (IsTrue(params[1]))
                            pDSTGame->EntityLeaveCombatStance(actor->GetScriptThing());
                    }
                } else if (command.StartsWith(".Decapitate")) {
                    if (actor) {
                        pDSTGame->EntitySetAsKillable(actor->GetScriptThing(), true, true);
                        CUTSCENE_FRAME();
                        pDSTGame->EntityDecapitate(actor->GetScriptThing());
                        CUTSCENE_FRAME();
                    }
                } else if (command.StartsWith(".TurnInto")) {
                    if (actor && params[0].GetLength()) {
                        CScriptThing thing = actor->GetScriptThing();
                        actor->Release();
                        pDSTGame->TurnCreatureInto(thing, params[0]);
                        pDSTGame->StartScriptingEntity(thing, actors[actorName], (EScriptAIPriority)4);
                    }
                } else if (command.StartsWith(".SlideTeleport")) {
                    if (actor && params[0].GetLength() && params[1].GetLength()) {
                        CScriptThing thing = actor->GetScriptThing();
                        CScriptThing from = CSFindThing(params[0], actors);
                        CScriptThing to = CSFindThing(params[1], actors);
                        long steps = 100;
                        long i = 1;
                        float fraction = 0.0f;                // defined (not only declared) before angleStep, beside steps and i: retail loads
                                                              // angleStep first and keeps fraction on the x87 stack past its last use (0x00CC59A4 /
                                                              // 0x00CC5C5F `fld [angleStep] ; fmul st(1)`, popped at 0x00CC59B7); value FITTED (dead store)
                        if (params[2].GetLength())
                            steps = GFCharStringToInt(params[2]);
                        C3DVector start = from.GetPos();
                        C3DVector end = to.GetPos();
                        float dx = end.X - start.X;
                        C3DVector step;
                        step.X = dx / steps;
                        step.Y = (end.Y - CSPositionY(start)) / steps;
                        step.Z = (end.Z - start.Z) / steps;
                        float angle = atan((end.Y - CSPositionY(start)) / dx) * 0.15915494f;
                        float angleStep = 0.5 / steps;
                        if (IsFalse(params[4]))
                            angleStep = 0.0f;
                        for (; i <= steps; ++i) {
                            if (IsCutsceneSkipped())
                                break;
                            fraction = (float)i;
                            pDSTGame->EntityTeleportToPosition(thing, start + step * fraction, angle + angleStep * fraction, false, false);
                            if (IsTrue(params[3])) {
                                CUTSCENE_FRAME();
                            }
                            if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                break;
                        }
                    }
                } else if (command.StartsWith("SlideTeleport")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength()) {
                        CScriptThing thing = CSFindThing(params[0], actors);
                        CScriptThing from = CSFindThing(params[1], actors);
                        CScriptThing to = CSFindThing(params[2], actors);
                        long steps = 100;
                        long i = 1;
                        float fraction = 0.0f;                // defined (not only declared) before angleStep, beside steps and i: retail loads
                                                              // angleStep first and keeps fraction on the x87 stack past its last use (0x00CC59A4 /
                                                              // 0x00CC5C5F `fld [angleStep] ; fmul st(1)`, popped at 0x00CC59B7); value FITTED (dead store)
                        if (params[3].GetLength())
                            steps = GFCharStringToInt(params[3]);
                        C3DVector start = from.GetPos();
                        C3DVector end = to.GetPos();
                        float dx = end.X - start.X;
                        C3DVector step;
                        step.X = dx / steps;
                        step.Y = (end.Y - start.Y) / steps;
                        step.Z = (end.Z - start.Z) / steps;
                        float angle = atan((end.Y - start.Y) / dx) * 0.15915494f;
                        float angleStep = 0.5 / steps;
                        if (IsFalse(params[4])) {
                            angle = from.GetAngleXY();
                            angleStep = 0.0f;
                        }
                        for (; i <= steps; ++i) {
                            if (IsCutsceneSkipped())
                                break;
                            fraction = (float)i;
                            pDSTGame->EntityTeleportToPosition(thing, start + step * fraction, angle + angleStep * fraction, false, false);
                            CUTSCENE_FRAME();
                            if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                break;
                        }
                        pDSTGame->EntityTeleportToPosition(thing, end, angle + angleStep * (float)i, false, false);
                    }
                } else if (command.StartsWith("Collide")) {
                    if (params[0].GetLength()) {
                        CScriptThing thing = CSFindThing(params[0], actors);
                        if (IsTrue(params[1])) {
                            pDSTGame->EntitySetAsCollidableToThings(thing, true);
                        } else {
                            pDSTGame->EntitySetAsCollidableToThings(thing, false);
                            if (!IsFalse(params[2]))
                                collideThings.push_back(thing);
                        }
                    }
                } else if (command.StartsWith("AToSkip")) {
                    g_bSkipCutsceneWithActionButton = !IsFalse(params[0]);
                } else if (command.StartsWith("KeepEntityMap")) {
                    keepActors = true;
                } else if (command.StartsWith("EnableBlackScreenSubtitles")) {
                    displayOverScreenFades = true;
                } else if (command.StartsWith("HideBodies")) {
                    if (params[0].GetLength() && !IsTrue(params[0]))
                        pDSTGame->SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(false);
                    else
                        pDSTGame->SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(true);
                } else if (command.StartsWith("AskQuestion")) {
                    if (params[0].GetLength() && !answered) {
                        CCharString question(params[0]);
                        CCharString yes("TEXT_OBJECT_HERO_ANSWER_YES");
                        CCharString no("TEXT_OBJECT_HERO_ANSWER_NO");
                        if (params[1].GetLength())
                            yes = params[1];
                        if (params[2].GetLength())
                            no = params[2];
                        pDSTGame->RemoveConversation(conversation, true);
                        pDSTGame->GiveHeroYesNoQuestion(question, yes, no, "", true);
                        long result;
                        while ((result = pDSTGame->MsgIsQuestionAnsweredYesOrNo()) < 0) {
                            CUTSCENE_FRAME();
                            if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                break;
                        }
                        answer = (result != 0);
                    }
                } else if (command.StartsWith("GiveHeroExpression")) {
                    if (params[0].GetLength()) {
                        CCharString expression(params[0]);
                        std::map<CCharString, bool>::iterator it;
                        it = localFlags.find(expression);       // 0x00CC61B8: the node is read through find's returned pointer and
                        if (it == localFlags.end()) {           // compared with the header after the call (0x00CC61BD `mov eax,[eax]`,
                                                                // 0x00CC61BF `cmp eax,[localFlags]`), operator== expanded
                            long quickAccessSlot = -1;
                            bool silent = false;
                            if (IsTrue(params[1]))
                                silent = true;
                            if (params[2].GetLength())
                                quickAccessSlot = GFCharStringToInt(params[2]);
                            pDSTGame->GiveHeroExpression(expression, quickAccessSlot, silent);
                        }
                    }
                } else if (command.StartsWith("GiveHeroMorality")) {
                    if (params[0].GetLength()) {
                        float morality = GFCharStringToFloat(params[0]);
                        FABLE_RETAIL_ASSERT(morality != 0.0f);
                        pDSTGame->GiveHeroMorality(morality);
                    }
                } else if (command.StartsWith("GiveHeroHealth")) {
                    if (params[0].GetLength()) {
                        float amount;
                        if (params[0].EqualsCaseInsensitive("MAX"))
                            amount = pDSTGame->GetHeroHealthMax() - pDSTGame->GetHeroHealth();
                        else
                            amount = GFCharStringToFloat(params[0]);
                        pDSTGame->ChangeHeroHealthBy(amount, true, false);
                    }
                } else if (command.StartsWith("GiveHero")) {
                    if (params[0].GetLength()) {
                        CCharString object(params[0]);
                        long count = 1;
                        long quickAccessSlot = -1;
                        bool b7f = false;
                        bool silent = false;
                        bool b13 = true;
                        if (params[1].GetLength())
                            count = GFCharStringToInt(params[1]);
                        if (params[2].GetLength())
                            quickAccessSlot = GFCharStringToInt(params[2]);
                        if (IsTrue(params[3]))
                            silent = true;
                        if (IsTrue(params[4]))
                            b7f = true;
                        if (IsFalse(params[5]))
                            b13 = false;
                        std::map<CCharString, long>::iterator it;
                        it = givenObjects.find(object);
                        if (it != givenObjects.end())
                            count -= givenObjects[object];
                        if (count > 0) {
                            for (long i = 0; i < count; ++i)
                                pDSTGame->GiveHeroObject(object, quickAccessSlot, silent);
                            if (it == givenObjects.end())
                                givenObjects[object] = count;
                            else
                                givenObjects[object] += count;
                            if (b7f) {
                                if (!silent) {
                                    CUTSCENE_FRAME();
                                }
                                if (b13)
                                    pDSTGame->SetWeaponAsHerosActiveWeapon(object);
                                else
                                    pDSTGame->MakeHeroCarryItemInHand(object);
                            }
                        }
                    }
                } else if (command.StartsWith("WaitActiveDialog")) {
                    if (conversation != -1) {
                        while (pDSTGame->IsConversationActive(conversation)) {
                            if (IsCutsceneSkipped())
                                break;
                            CUTSCENE_FRAME();
                            if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                break;
                        }
                    }
                } else if (command.StartsWith("RegisterActor")) {
                    if (params[0].GetLength())
                        registeredActors.insert(params[0]);
                } else if (command.StartsWith("RemoveAllThings")) {
                    if (params[0].GetLength()) {
                        std::vector<CScriptThing> things;
                        pDSTGame->GetAllThingsWithScriptName("LadyGreyIntro", things);
                        for (unsigned int i = 0; i < things.size(); ++i)
                            pDSTGame->RemoveThing(things[i], false, true);
                    }
                } else if (command.StartsWith("RemoveAll")) {
                    pDSTGame->GetAllCreaturesExcludingHero(creatures);
                    for (unsigned int i = 0; i < creatures.size(); ++i)
                        if (creatures[i].IsAlive()) {
                            if (IsFalse(params[0]))
                                pDSTGame->EntitySetAsDrawable(creatures[i], true);
                            else
                                pDSTGame->EntitySetAsDrawable(creatures[i], false);
                        }
                } else if (command.StartsWith("ReturnFollowers")) {
                    if (params[0].GetLength()) {
                        if (IsTrue(params[0])) {
                            restoreFollowers = true;
                            pDSTGame->GetFollowingEntityList(pDSTGame->GetHero(), followers);
                            if (actors.find("HeroFollower0") == actors.end()) {
                                for (unsigned int i = 0; i < followers.size(); ++i)
                                    if (followers[i].IsAlive())
                                        fadedFollowers.push_back(followers[i]);
                            }
                        } else {
                            restoreFollowers = false;
                        }
                    }
                } else if (command.StartsWith("TeleportFollowers")) {
                    if (followers.size() > 0) {
                        if (IsTrue(params[0])) {
                            pDSTGame->FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 255));
                            pDSTGame->Pause(0.5f);
                        }
                        pDSTGame->TeleportAllFollowersToHeroPosition();
                        if (IsTrue(params[0]))
                            pDSTGame->FadeScreenIn();
                    }
                } else if (command.StartsWith("RemoveExtras")) {
                    CScriptThing destination;
                    bool limbo = false;
                    bool returnExtras = false;
                    if (params[1].EqualsCaseInsensitive("limbo"))
                        limbo = true;
                    else if (params[1].EqualsCaseInsensitive("return"))
                        returnExtras = true;
                    if (params[1].GetLength() && !IsNull(params[1]) && !returnExtras && !limbo)
                        destination = CSFindThing(params[1], actors);
                    if (!extrasRemoved) {
                        if (params[2].GetLength() && params[3].GetLength()) {
                            float radius = GFCharStringToFloat(params[3]);
                            FABLE_RETAIL_ASSERT(radius >= 0.0f);
                            CScriptThing centre = CSFindThing(params[2], actors);
                            pDSTGame->GetAllCreaturesInAreaWithScriptName("", centre.GetPos(), radius, creatures);
                        } else
                            pDSTGame->GetAllCreaturesExcludingHero(creatures);
                        CScriptThing extra;
                        CCharString defName;
                        CCharString extraName;
                        for (unsigned int i = 0; i < creatures.size(); ++i) {
                            extra = creatures[i];
                            defName = extra.GetDefName();
                            if (defName != "CREATURE_DEMON_DOOR_FACE_01" && defName != "CREATURE_NW_DEMON_DOOR_FACE_01") {
                                extraName = extra.GetName();
                                std::set<CCharString>::iterator registered = registeredActors.find(extraName);
                                if (registered == registeredActors.end() && extra.IsAlive()) {
                                    removedExtras.push_back(extra);
                                    extraPositions.push_back(extra.GetPos());
                                    extraAngles.push_back(extra.GetAngleXY());
                                    if (IsFalse(params[0])) {
                                        if (limbo)
                                            pDSTGame->EntitySetInLimbo(extra, false, true);
                                        else
                                            pDSTGame->EntitySetAsDrawable(extra, true);
                                    } else {
                                        if (limbo)
                                            pDSTGame->EntitySetInLimbo(extra, true, true);
                                        else
                                            pDSTGame->EntitySetAsDrawable(extra, false);
                                        if (destination.IsAlive())
                                            pDSTGame->EntityTeleportToPosition(extra, destination.GetPos(), destination.GetAngleXY(), false, false);
                                    }
                                }
                            }
                        }
                        extrasRemoved = true;
                    } else {
                        for (unsigned int i = 0; i < removedExtras.size(); ++i) {
                            CScriptThing extra = removedExtras[i];
                            if (extra.IsAlive()) {
                                if (IsFalse(params[0])) {
                                    if (limbo)
                                        pDSTGame->EntitySetInLimbo(extra, false, true);
                                    else
                                        pDSTGame->EntitySetAsDrawable(extra, true);
                                    if (returnExtras)
                                        pDSTGame->EntityTeleportToPosition(extra, extraPositions[i], extraAngles[i], false, false);
                                } else {
                                    if (limbo)
                                        pDSTGame->EntitySetInLimbo(extra, true, true);
                                    else
                                        pDSTGame->EntitySetAsDrawable(extra, false);
                                    if (destination.IsAlive()) {
                                        extraPositions.push_back(extra.GetPos());
                                        extraAngles.push_back(extra.GetAngleXY());
                                        pDSTGame->EntityTeleportToPosition(extra, destination.GetPos(), destination.GetAngleXY(), false, false);
                                    }
                                }
                            }
                        }
                    }
                }
                if (command.StartsWith("DoScriptFrame")) {
                    long frames = 1;
                    if (params[0].GetLength())
                        frames = GFCharStringToInt(params[0]);
                    for (long i = 0; i < frames; ++i) {
                        CUTSCENE_FRAME();
                    }
                }
                if (command.StartsWith("ScriptFrame")) {
                    doScriptFrames = !IsFalse(params[0]);
                } else if (command.StartsWith("AnimationPause")) {
                    frameAfterAnim = !IsFalse(params[0]);
                } else if (command.StartsWith("CameraPause")) {
                    frameAfterCamera = !IsFalse(params[0]);
                } else if (command.StartsWith("MuteSounds")) {
                    if (IsFalse(params[0]))
                        pDSTGame->SetAllSoundsAsMuted(false);
                    else
                        pDSTGame->SetAllSoundsAsMuted(true);
                } else if (command.StartsWith("DebugCamera")) {
                    debugCamera = !IsFalse(params[0]);
                } else if (command.StartsWith("NoDialogCam")) {
                    if (IsTrue(params[0]))
                        pDSTGame->FixMovieSequenceCamera(true);
                    else
                        pDSTGame->FixMovieSequenceCamera(false);
                } else if (command.StartsWith("LookAtNothing")) {
                    if (params[0].GetLength()) {
                        CScriptThing thing = pDSTGame->GetThingWithScriptName(params[0]);
                        if (thing.IsAlive()) {
                            if (IsFalse(params[1])) {
                                pDSTGame->EntityResetForceToLookAt(thing);
                            } else {
                                pDSTGame->EntityForceToLookAtNothing(thing);
                                if (!IsFalse(params[2]))
                                    lookingThings.push_back(thing);
                            }
                        }
                    }
                } else if (command.StartsWith("PlayObjectAnim")) {
                    if (params[0].GetLength() && params[1].GetLength()) {
                        bool looping = false;
                        if (IsTrue(params[2]))
                            looping = true;
                        CScriptThing thing = pDSTGame->GetThingWithScriptName(params[0]);
                        pDSTGame->EntitySetCutsceneBehaviour(thing, (ECutsceneBehaviour)2);
                        pDSTGame->EntityPlayObjectAnimation(thing, params[1], looping);
                        if (frameAfterAnim) {
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith("DoOneFrame")) {
                    CUTSCENE_FRAME();
                } else if (command.StartsWith("FadeThingOut")) {
                    if (params[0].GetLength() && params[1].GetLength()) {
                        CScriptThing thing = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        float from = 1.0f;
                        float to = 0.0f;
                        if (params[2].GetLength())
                            from = GFCharStringToFloat(params[2]);
                        if (params[3].GetLength())
                            to = GFCharStringToFloat(params[3]);
                        float time = GFCharStringToFloat(params[1]);
                        FABLE_RETAIL_ASSERT(time >= 0.0f);
                        if (time > 0.0f) {
                            float step = (from - to) / (time * 15.0f);
                            for (float alpha = from; alpha > to; alpha -= step) {
                                pDSTGame->EntitySetAlpha(thing, alpha, true);
                                CUTSCENE_FRAME();
                            }
                        }
                        pDSTGame->EntitySetAlpha(thing, to, true);
                    }
                } else if (command.StartsWith("FadeThingIn")) {
                    if (params[0].GetLength() && params[1].GetLength()) {
                        CScriptThing thing = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        float from = 0.0f;
                        float to = 1.0f;
                        if (params[2].GetLength())
                            from = GFCharStringToFloat(params[2]);
                        if (params[3].GetLength())
                            to = GFCharStringToFloat(params[3]);
                        float time = GFCharStringToFloat(params[1]);
                        FABLE_RETAIL_ASSERT(time >= 0.0f);
                        if (time > 0.0f) {
                            float step = (to - from) / (time * 15.0f);
                            for (float alpha = from; alpha > to; alpha += step) {
                                pDSTGame->EntitySetAlpha(thing, alpha, true);
                                CUTSCENE_FRAME();
                            }
                        }
                        pDSTGame->EntitySetAlpha(thing, to, true);
                    }
                } else if (command.StartsWith("CameraPreload")) {
                    if (params[0].GetLength()) {
                        pDSTGame->CameraSetCameraPreloadFlag(true);
                        pDSTGame->CameraUseCameraPoint(params[0], (const C3DVector*)0, (const CRightHandedSet*)0, -1.0f, 0, -1);
                        pDSTGame->CameraSetCameraPreloadFlag(false);
                    }
                } else if (command.StartsWith("PauseThing")) {
                    if (params[0].GetLength() && params[1].GetLength()) {
                        CScriptThing thing = CSFindThing(params[0], actors);
                        if (thing.IsAlive()) {
                            if (IsFalse(params[1]))
                                pDSTGame->EntitySetCutsceneBehaviour(thing, (ECutsceneBehaviour)2);
                            else
                                pDSTGame->EntitySetCutsceneBehaviour(thing, (ECutsceneBehaviour)1);
                        }
                    }
                } else if (command.StartsWith("SetGravityOnThing")) {
                    if (params[0].GetLength() && params[1].GetLength()) {
                        CScriptThing thing = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        bool gravity = true;
                        if (IsFalse(params[1]))
                            gravity = false;
                        if (thing.IsAlive())
                            pDSTGame->EntityEnableGravity(thing, gravity);
                    }
                } else if (command.StartsWith("SetHomePosThing")) {
                    if (params[0].GetLength()) {
                        CScriptThing thing = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        if (thing.IsAlive())
                            pDSTGame->EntityTeleportToPosition(thing, thing.GetHomePos(), 0.0f, false, false);
                    }
                } else if (command.StartsWith("TeleportThing")) {
                    if (params[0].GetLength() && params[1].GetLength()) {
                        CScriptThing thing = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        CScriptThing target = (CCharString("HERO") == params[1]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[1]);
                        bool forceNavigable = true;
                        if (IsFalse(params[2]))
                            forceNavigable = false;
                        if (thing.IsAlive() && target.IsAlive())
                            pDSTGame->EntityTeleportToPosition(thing, target.GetPos(), target.GetAngleXY(), forceNavigable, false);
                    }
                } else if (command.StartsWith("SetThingConscious")) {
                    if (params[0].GetLength()) {
                        bool conscious = false;
                        CScriptThing thing;
                        CCharString animation("");
                        CScriptGameResourceObjectScriptedThingBase* target = 0;
                        CCutsceneActorMap::iterator it;
                        it = actors.find(params[0]);
                        if (it != actors.end() && (*it).second.IsValid())
                            target = &(*it).second;
                        if (target && target->IsValid())
                            thing = target->GetScriptThing();
                        else
                            thing = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        if (IsTrue(params[1]))
                            conscious = true;
                        if (params[2].GetLength())
                            animation = params[2];
                        pDSTGame->SetThingAsConscious(thing, conscious, animation);
                    }
                } else if (command.StartsWith("LiftRock")) {
                    if (params[0].GetLength() && params[1].GetLength()) {
                        CScriptThing hero = pDSTGame->GetHero();
                        pDSTGame->GiveThingItemInSlot(hero, params[0], params[1]);
                    }
                } else if (command.StartsWith("GiveGold")) {
                    if (params[0].GetLength()) {
                        long gold = GFCharStringToInt(params[0]);
                        FABLE_RETAIL_ASSERT(gold != 0);
                        std::map<CCharString, long>::iterator it;
                        it = givenObjects.find("Gold");
                        if (it != givenObjects.end())
                            gold -= givenObjects["Gold"];
                        if (gold > 0) {
                            pDSTGame->GiveHeroGold(gold);
                            std::map<CCharString, long>::iterator last = givenObjects.end();
                            if (it == last)
                                givenObjects["Gold"] = gold;
                            else
                                givenObjects["Gold"] += gold;
                        }
                    }
                }
                if (command.StartsWith("WaitBossFight")) {
                    if (params[0].GetLength()) {
                        CScriptThing boss = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        long phase = GFCharStringToInt(params[1]);
                        FABLE_RETAIL_ASSERT(phase >= 0);
                        float maxFrames = GFCharStringToFloat(params[2]) * 15.0f;
                        FABLE_RETAIL_ASSERT(maxFrames >= 0.0f);
                        float frames = 0.0f;
                        while (!IsCutsceneSkipped()) {
                            if (pDSTGame->EntityGetBossPhase(boss) != phase || !(frames <= maxFrames))
                                break;
                            CUTSCENE_FRAME();
                            frames += 1.0f;
                            if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                break;
                        }
                        CScriptGameResourceObjectScriptedThingBase scripted;
                        pDSTGame->StartScriptingEntity(boss, scripted, (EScriptAIPriority)4);
                        actors[boss.GetName()] = scripted;
                    }
                } else if (command.StartsWith("DoCharacterPreload")) {
                    if (params[0].GetLength())
                        pDSTGame->BeginLoadingMesh(params[0]);
                } else if (command.StartsWith("DoCameraPreloading")) {
                    pDSTGame->StartProgressDisplay();
                    if (IsTrue(params[0])) {
                        float time = 2.0f;
                        if (params[1].GetLength())
                            time = GFCharStringToFloat(params[1]);
                        pDSTGame->PreloadNewScene(time);
                    } else {
                        CutsceneCameraPreloader(commands, false, &g_CutsceneSoundFrame);
                    }
                    pDSTGame->StopProgressDisplay();
                } else if (command.StartsWith("StartProgressSpinner")) {
                } else if (command.StartsWith("StopProgressSpinner")) {
                } else if (command.StartsWith("TakeObjectFromHero")) {
                    if (params[0].GetLength() && params[0].GetLength())
                        pDSTGame->TakeObjectFromHero(params[0]);
                } else if (command.StartsWith("GamePause")) {
                    if (params[0].GetLength()) {
                        float seconds = GFCharStringToFloat(params[0]);
                        FABLE_RETAIL_ASSERT(seconds >= 0.0f);
                        if (params[1].EqualsCaseInsensitive("clock")) {
                            double end = GFGetTime() + seconds;
                            while (GFGetTime() < end) {
                                if (IsCutsceneSkipped())
                                    break;
                                CUTSCENE_FRAME();
                                if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                    break;
                            }
                        } else {
                            CUTSCENE_FRAME();
                            float frames = seconds * 15.0f;
                            for (float frame = 0.0f; frame < frames; frame += 1.0f) {
                                if (IsCutsceneSkipped())
                                    break;
                                CUTSCENE_FRAME();
                            }
                        }
                    }
                } else if (command.StartsWith("SetDoorOpen")) {
                    if (params[0].GetLength()) {
                        if (IsFalse(params[1]))
                            pDSTGame->CloseDoor((CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]));
                        else
                            pDSTGame->OpenDoor((CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]));
                    }
                } else if (command.StartsWith("SetChestOpen")) {
                    if (params[0].GetLength()) {
                        if (IsFalse(params[1]))
                            pDSTGame->CloseChest((CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]));
                        else
                            pDSTGame->OpenChest((CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]), false);
                    }
                } else if (command.StartsWith("EnableSounds")) {
                    if (params[0].GetLength() && !IsFalse(params[0]))
                        pDSTGame->EnableSounds(true);
                    else
                        pDSTGame->EnableSounds(false);
                } else if (command.StartsWith("CacheMusic")) {
                    if (params[0].GetLength()) {
                        long musicType = g_pMusicTypeSymbolMap->GetSymbolSLONG(params[0]);
                        FABLE_RETAIL_ASSERT(musicType >= -1);
                        if (musicType != -1)
                            pDSTGame->CacheMusicSet((EMusicSetType)musicType);
                    }
                } else if (command.StartsWith("PlayMusic")) {
                    if (params[0].GetLength()) {
                        long musicType = g_pMusicTypeSymbolMap->GetSymbolSLONG(params[0]);
                        FABLE_RETAIL_ASSERT(musicType >= -1);
                        if (musicType != -1) {
                            pDSTGame->OverrideMusic((EMusicSetType)musicType, true, false);
                            stopMusicAtEnd = !IsFalse(params[1]);
                        }
                    }
                } else if (command.StartsWith("PlaySound")) {
                    if (params[0].GetLength() && params[1].GetLength()) {
                        if (IsNull(params[0]))
                            pDSTGame->Play2DSound(params[1]);
                        else if (params[2].EqualsCaseInsensitive("criteria"))
                            pDSTGame->PlayCriteriaSoundOnThing(CSFindThing(params[0], actors), params[1]);
                        else
                            pDSTGame->PlaySoundOnThing(CSFindThing(params[0], actors), params[1]);
                        CUTSCENE_FRAME();
                    }
                } else if (command.StartsWith("RemoveHeroWeapons")) {
                    if (IsFalse(params[0]))
                        pDSTGame->ReturnAllConfiscatedItemsToHero();
                    else
                        pDSTGame->ConfiscateAllHeroWeapons();
                } else if (command.StartsWith("HeroHair")) {
                    if (params[0].GetLength())
                        pDSTGame->ChangeHeroHairstyle(params[0]);
                } else if (command.StartsWith("HeroTattoo")) {
                    if (params[0].GetLength())
                        pDSTGame->AddTattooToHero(params[0]);
                } else if (command.StartsWith("HeroWear")) {
                    if (params[0].GetLength())
                        pDSTGame->SetHeroAsWearing(params[0]);
                } else if (command.StartsWith("RemoveHeroClothes")) {
                    pDSTGame->RemoveHeroClothing();
                } else if (command.StartsWith("PutUpYourSwords")) {
                    if (IsTrue(params[0])) {
                        CScriptThing hero = pDSTGame->GetHero();
                        if (pDSTGame->IsEntityWieldingMeleeWeapon(hero))
                            weaponState = "MELEE";
                        else if (pDSTGame->IsEntityWieldingRangedWeapon(hero))
                            weaponState = "RANGED";
                    }
                    pDSTGame->SheatheHeroWeapons();
                } else if (command.StartsWith("CameraRig")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength() && params[3].GetLength() &&
                        params[4].GetLength() && params[5].GetLength()) {
                        CScriptThing camera = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        CScriptThing target = (CCharString("HERO") == params[1]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[1]);
                        float offsetX = GFCharStringToFloat(params[2]);
                        float offsetY = GFCharStringToFloat(params[3]);
                        float offsetZ = GFCharStringToFloat(params[4]);
                        float frames = GFCharStringToFloat(params[5]) * 15.0f;
                        FABLE_RETAIL_ASSERT(frames >= 0.0f);
                        for (long i = 0; i < frames; ++i) {
                            if (target.IsAlive()) {
                                pDSTGame->EntityTeleportToPosition(camera, C3DVector(offsetX, offsetY, offsetZ) + target.GetPos(), 0.0f, false, false);
                                pDSTGame->CameraUseCameraPoint(camera, (const C3DVector*)0, (const CRightHandedSet*)0, -1.0f, 0, -1);
                                CUTSCENE_FRAME();
                            }
                        }
                    }
                } else if (command.StartsWith("UseCameraFOVMarkerList")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength() && params[3].GetLength() &&
                        params[4].GetLength() && params[5].GetLength() && params[6].GetLength()) {
                        float fov = -1.0f;
                        bool mostAligned = true;                               // descriptive: GetBestThingFromMarkerList 0x00CBF22D picks the marker most (true) or least in line
                        CScriptThing from = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        CScriptThing to = (CCharString("HERO") == params[1]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[1]);
                        CScriptThing marker0 = (CCharString("HERO") == params[2]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[2]);
                        CScriptThing marker1 = (CCharString("HERO") == params[3]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[3]);
                        CScriptThing marker2 = (CCharString("HERO") == params[4]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[4]);
                        CScriptThing marker3 = (CCharString("HERO") == params[5]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[5]);
                        std::vector<CScriptThing> markers;
                        markers.push_back(marker0);
                        markers.push_back(marker1);
                        markers.push_back(marker2);
                        markers.push_back(marker3);
                        if (params[7].GetLength())
                            fov = GFCharStringToFloat(params[7]) * (1.0f / 360.0f);
                        if (IsFalse(params[8]))
                            mostAligned = false;
                        float distance = GFCharStringToFloat(params[6]);
                        FABLE_RETAIL_ASSERT(distance >= 0.0f);
                        if (marker0.IsAlive() && marker1.IsAlive() && marker2.IsAlive() && marker3.IsAlive() && from.IsAlive() && to.IsAlive()) {
                            CScriptThing best = GetBestThingFromMarkerList(markers, from, to, mostAligned);
                            if (params[9].GetLength())
                                pDSTGame->CameraUseCameraPoint(best.GetName(), (const C3DVector*)0, (const CRightHandedSet*)0, fov, 0, -1);
                            else
                                pDSTGame->CameraMoveBetweenLookingAt(best.GetPos(), best.GetPos(), to, distance, fov);
                        }
                    }
                } else if (command.StartsWith("DrawThing")) {
                    if (params[0].GetLength() && params[1].GetLength()) {
                        bool drawable = true;
                        if (IsFalse(params[1]))
                            drawable = false;
                        CScriptThing thing = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        pDSTGame->EntitySetAsDrawable(thing, drawable);
                    }
                } else if (command.StartsWith("ResetCamera")) {
                    pDSTGame->CameraResetToViewBehindHero(0.0f);
                    pDSTGame->CameraDefault();
                } else if (command.StartsWith("NoLoadUseCamera")) {
                    if (params[0].GetLength()) {
                        long cameraFlags = 0;
                        long cameraParam = -1;
                        if (params[1].GetLength())
                            cameraFlags = GFStringToInt(params[1]);
                        if (params[2].GetLength())
                            cameraParam = GFStringToInt(params[2]);
                        pDSTGame->CameraUseCameraPoint(params[0], (const C3DVector*)0, (const CRightHandedSet*)0, -1.0f, cameraFlags, cameraParam);
                        if (frameAfterCamera) {
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith("UseCamera")) {
                    if (params[0].GetLength() && !IsNull(params[0])) {
                        float fov = -1.0f;
                        CScriptThing target;
                        long cameraFlags = 0;
                        long cameraParam = -1;
                        if (params[1].GetLength())
                            fov = GFStringToFloat(params[1]);
                        if (params[2].GetLength() && !IsNull(params[2])) {
                            CScriptGameResourceObjectScriptedThingBase* actorTarget = 0;
                            CCutsceneActorMap::iterator it;
                            it = actors.find(params[2]);
                            if (it != actors.end() && (*it).second.IsValid())
                                actorTarget = &(*it).second;
                            if (actorTarget && actorTarget->IsValid())
                                target = actorTarget->GetScriptThing();
                            else
                                target = (CCharString("HERO") == params[2]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[2]);
                        }
                        if (params[3].GetLength())
                            cameraFlags = GFStringToInt(params[3]);
                        if (params[4].GetLength())
                            cameraParam = GFStringToInt(params[4]);
                        if (!target.IsNull() && target.IsAlive() == true) {
                            if (fov != -1.0f)
                                pDSTGame->CameraUseCameraPoint(params[0], target, fov, cameraFlags, cameraParam);
                            else
                                pDSTGame->CameraUseCameraPoint(params[0], target, -1.0f, cameraFlags, cameraParam);
                        } else {
                            if (fov != -1.0f)
                                pDSTGame->CameraUseCameraPoint(params[0], (const C3DVector*)0, (const CRightHandedSet*)0, fov, cameraFlags, cameraParam);
                            else
                                pDSTGame->CameraUseCameraPoint(params[0], (const C3DVector*)0, (const CRightHandedSet*)0, -1.0f, cameraFlags, cameraParam);
                        }
                        if (debugCamera)
                            pDSTGame->ShowOnScreenMessage("CAMERA:" + params[0], 5.0f);
                        if (frameAfterCamera) {
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith("PlayAVI")) {
                    if (params[0].GetLength()) {
                        CCharString path("Data\\Video\\");
                        pDSTGame->PlayAVIMovie(path + params[0]);
                    }
                } else if (command.StartsWith("TeleportToHSP")) {
                    if (params[0].GetLength()) {
                        pDSTGame->EntityTeleportToThing(pDSTGame->GetHero(), (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]), false);
                        CUTSCENE_FRAME();           // 0x00CCA417 `jmp 0x00CCA733`: the temporary's destructor is shared with CameraRotateThing's tail, which goes on to a frame (0x00CCA738 `jmp 0x00CC907D`)
                    }
                } else if (command.StartsWith("WaitForCamera")) {
                    while (pDSTGame->IsCameraInScriptedMode()) {
                        if (pDSTGame->IsActiveThreadTerminating())
                            break;
                        if (IsCutsceneSkipped())
                            break;
                        CUTSCENE_FRAME();
                        if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                            break;
                    }
                } else if (command.StartsWith("SetFlag")) {
                    if (params[0].GetLength() && params[1].GetLength() && flags) {
                        if (!IsTrue(params[2]) || !flagWasSet) {
                            if (IsFalse(params[1])) {
                                flagWasSet = true;
                                (*flags)[params[0]] = false;
                            } else {
                                flagWasSet = true;
                                (*flags)[params[0]] = true;
                            }
                        }
                        CUTSCENE_FRAME();
                    }
                } else if (command.StartsWith("CameraRotateThing")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength() && params[3].GetLength() && params[4].GetLength()) {
                        float speed = GFCharStringToFloat(params[1]);
                        FABLE_RETAIL_ASSERT(speed >= 0.0f);
                        float x = GFCharStringToFloat(params[2]);
                        float y = GFCharStringToFloat(params[3]);
                        float z = GFCharStringToFloat(params[4]);
                        pDSTGame->CameraCircleAroundThing((CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]), C3DVector(x, y, z), speed);
                        CUTSCENE_FRAME();
                    }
                } else if (command.StartsWith("CameraLookAt")) {
                    if (params[0].GetLength() && params[1].GetLength()) {
                        CScriptThing camera = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        float offsetX = 0.0f;
                        float offsetY = 0.0f;
                        float offsetZ = 0.0f;
                        float speed = 0.0f;
                        if (params[2].GetLength())
                            speed = GFCharStringToFloat(params[2]);
                        if (params[3].GetLength())
                            offsetX = GFCharStringToFloat(params[3]);
                        if (params[4].GetLength())
                            offsetY = GFCharStringToFloat(params[4]);
                        if (params[5].GetLength())
                            offsetZ = GFCharStringToFloat(params[5]);
                        CScriptGameResourceObjectScriptedThingBase* target = 0;
                        CCutsceneActorMap::iterator it;
                        it = actors.find(params[1]);
                        if (it != actors.end() && (*it).second.IsValid())
                            target = &(*it).second;
                        if (target && target->IsValid())
                            pDSTGame->CameraMoveToPosAndLookAtThing(C3DVector(offsetX, offsetY, offsetZ) + camera.GetPos(), target->GetScriptThing(), speed);
                        else
                            pDSTGame->CameraMoveToPosAndLookAtThing(C3DVector(offsetX, offsetY, offsetZ) + camera.GetPos(),
                                                                    (CCharString("HERO") == params[1]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[1]), speed);
                        CUTSCENE_FRAME();
                    }
                } else if (command.StartsWith("CameraLookBetween")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength() && params[3].GetLength()) {
                        CScriptThing from = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        CScriptThing to = (CCharString("HERO") == params[1]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[1]);
                        float fromX = 0.0f;
                        float fromY = 0.0f;
                        float fromZ = 0.0f;
                        float toX = 0.0f;
                        float toY = 0.0f;
                        float toZ = 0.0f;
                        float time = GFCharStringToFloat(params[3]);
                        FABLE_RETAIL_ASSERT(time >= 0.0f);
                        if (params[4].GetLength())
                            fromX = GFCharStringToFloat(params[4]);
                        if (params[5].GetLength())
                            fromY = GFCharStringToFloat(params[5]);
                        if (params[6].GetLength())
                            fromZ = GFCharStringToFloat(params[6]);
                        if (params[7].GetLength())
                            toX = GFCharStringToFloat(params[7]);
                        if (params[8].GetLength())
                            toY = GFCharStringToFloat(params[8]);
                        if (params[9].GetLength())
                            toZ = GFCharStringToFloat(params[9]);
                        CScriptGameResourceObjectScriptedThingBase* target = 0;
                        CCutsceneActorMap::iterator it;
                        it = actors.find(params[2]);
                        if (it != actors.end() && (*it).second.IsValid())
                            target = &(*it).second;
                        if (target && target->IsValid()) {
                            pDSTGame->CameraMoveBetweenLookingAt(C3DVector(fromX, fromY, fromZ) + from.GetPos(), C3DVector(toX, toY, toZ) + to.GetPos(),
                                                                 target->GetScriptThing(), time, -1.0f);
                            CUTSCENE_FRAME();
                        } else {
                            pDSTGame->CameraMoveBetweenLookingAt(C3DVector(fromX, fromY, fromZ) + from.GetPos(), C3DVector(toX, toY, toZ) + to.GetPos(),
                                                                 (CCharString("HERO") == params[2]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[2]), time, -1.0f);
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith("CameraPath")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength() && params[3].GetLength() && params[4].GetLength()) {
                        CScriptThing from = CSFindThing(params[0], actors);
                        CScriptThing fromLookAt = CSFindThing(params[1], actors);
                        CScriptThing to = CSFindThing(params[2], actors);
                        CScriptThing toLookAt = CSFindThing(params[3], actors);
                        float time = GFCharStringToFloat(params[4]);
                        FABLE_RETAIL_ASSERT(time >= 0.0f);
                        pDSTGame->CameraMoveBetweenLookFromAndLookTo(from.GetPos(), to.GetPos(), fromLookAt.GetPos(), toLookAt.GetPos(), time);
                    }
                } else if (command.StartsWith("CameraFOVLookBetweenPos")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength() && params[3].GetLength()) {
                        CScriptThing from = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        CScriptThing to = (CCharString("HERO") == params[1]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[1]);
                        CScriptGameResourceObjectScriptedThingBase* target = 0;
                        float time = GFCharStringToFloat(params[3]);
                        FABLE_RETAIL_ASSERT(time >= 0.0f);
                        float offsetX = 0.0f;
                        float offsetY = 0.0f;
                        float offsetZ = 0.0f;
                        if (params[4].GetLength())
                            offsetX = GFCharStringToFloat(params[4]);
                        if (params[5].GetLength())
                            offsetY = GFCharStringToFloat(params[5]);
                        if (params[6].GetLength())
                            offsetZ = GFCharStringToFloat(params[6]);
                        CCutsceneActorMap::iterator it;
                        it = actors.find(params[2]);
                        if (it != actors.end() && (*it).second.IsValid())
                            target = &(*it).second;
                        float fov = -1.0f;
                        if (params[4].GetLength())
                            fov = GFCharStringToFloat(params[4]) * (1.0f / 360.0f);
                        C3DVector lookAt;
                        if (target && target->IsValid()) {
                            lookAt = target->GetScriptThing().GetPos();
                        } else {
                            CScriptThing lookAtThing = (CCharString("HERO") == params[2]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[2]);
                            lookAt = lookAtThing.GetPos();
                        }
                        // 0x00CCB3C6..0x00CCB3F5: the member operator (X is reached through `this` at offset 0, so its float record is
                        // created in a later c2 pass than Y / Z: only the X sum loads the offset first, `fld [offsetX] ; fadd [lookAt.X]`).
                        // FITTED: the named operand (a temporary gives the same bytes here; the name is one more c2 record before
                        // CrowdAnimate's `random`, 0x00CCE8EA).
                        C3DVector offset(offsetX, offsetY, offsetZ);
                        lookAt += offset;
                        pDSTGame->CameraMoveBetweenLookingAt(from.GetPos(), to.GetPos(), lookAt, time, fov);
                        CUTSCENE_FRAME();
                    }
                } else if (command.StartsWith("CameraFOVLookBetween")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength() && params[3].GetLength()) {
                        CScriptThing from = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        CScriptThing to = (CCharString("HERO") == params[1]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[1]);
                        CScriptGameResourceObjectScriptedThingBase* target = 0;
                        float time = GFCharStringToFloat(params[3]);
                        float offsetX;
                        float offsetY;
                        float offsetZ;
                        if (params[4].GetLength())
                            offsetX = GFCharStringToFloat(params[4]);
                        if (params[5].GetLength())
                            offsetY = GFCharStringToFloat(params[5]);
                        if (params[6].GetLength())
                            offsetZ = GFCharStringToFloat(params[6]);
                        CCutsceneActorMap::iterator it;
                        it = actors.find(params[2]);
                        if (it != actors.end() && (*it).second.IsValid())
                            target = &(*it).second;
                        float fov = -1.0f;
                        if (params[4].GetLength())
                            fov = GFCharStringToFloat(params[4]) * (1.0f / 360.0f);
                        if (target && target->IsValid()) {
                            pDSTGame->CameraMoveBetweenLookingAt(from.GetPos(), to.GetPos(), target->GetScriptThing(), time, fov);
                            CUTSCENE_FRAME();
                        } else {
                            pDSTGame->CameraMoveBetweenLookingAt(from.GetPos(), to.GetPos(),
                                                                 (CCharString("HERO") == params[2]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[2]), time, fov);
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith("WaitFlag")) {
                    if (params[0].GetLength() && params[1].GetLength() && flags) {
                        bool value = false;
                        if (IsTrue(params[1]))
                            value = true;
                        while ((*flags)[params[0]] != value) {
                            if (IsCutsceneSkipped())
                                break;
                            CUTSCENE_FRAME();
                            if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                break;
                        }
                    }
                } else if (command.StartsWith("CreateLight")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength() && params[3].GetLength() && params[4].GetLength() &&
                        params[5].GetLength() && params[6].GetLength() && params[7].GetLength() && params[8].GetLength()) {
                        float red = GFCharStringToFloat(params[1]);
                        float green = GFCharStringToFloat(params[2]);
                        float blue = GFCharStringToFloat(params[3]);
                        float innerRadius = GFCharStringToFloat(params[4]);
                        float outerRadius = GFCharStringToFloat(params[5]);
                        float flicker = GFCharStringToFloat(params[6]);
                        CScriptThing position = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        CScriptThing light = pDSTGame->CreateLight(position.GetPos(), CRGBColour((unsigned char)red, (unsigned char)green, (unsigned char)blue, 255),
                                                                   params[7], innerRadius, outerRadius, flicker > 0.0f);
                        if (IsTrue(params[8]))
                            removeAtEnd.push_back(light);
                    }
                } else if (command.StartsWith("CreateEffect")) {
                    if (params[0].GetLength() && params[1].GetLength()) {
                        CScriptThing target = CSFindThing(params[1], actors);
                        if (target.IsAlive()) {
                            float height = 0.0f;
                            CCharString data("");
                            if (params[2].GetLength())
                                data = params[2];
                            if (params[3].GetLength())
                                height = GFCharStringToFloat(params[3]);
                            const C3DVector& targetPos = target.GetPos();
                            C3DVector position(targetPos.X, targetPos.Y, height + targetPos.Z);
                            CScriptThing effect = pDSTGame->CreateEffect(params[0], position, data, target.GetAngleXY(), false, false);
                            if (effect.IsAlive()) {
                                pDSTGame->EntitySetCutsceneBehaviour(effect, (ECutsceneBehaviour)2);
                                if (IsTrue(params[4]) || !params[4].GetLength()) {
                                    effect.SetDataString(data);
                                    removeAtEnd.push_back(effect);
                                }
                            }
                        }
                    }
                } else if (command.StartsWith("DummyEffect")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength()) {
                        CScriptThing target = CSFindThing(params[1], actors);
                        if (target.IsAlive()) {
                            CCharString data("");
                            if (params[3].GetLength())
                                data = params[3];
                            CScriptThing effect = pDSTGame->CreateEffect(params[0], target, params[2], data, false, true);
                            if (effect.IsAlive()) {
                                pDSTGame->EntitySetCutsceneBehaviour(effect, (ECutsceneBehaviour)2);
                                if (IsTrue(params[4]) || !params[4].GetLength()) {
                                    effect.SetDataString(data);
                                    removeAtEnd.push_back(effect);
                                }
                            }
                        }
                    }
                } else if (command.StartsWith("CreateNear")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength()) {
                        CScriptThing position = (CCharString("HERO") == params[1]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[1]);
                        float radius = GFCharStringToFloat(params[3]);
                        CScriptThing creature = pDSTGame->CreateCreatureNearby(params[0], position.GetPos(), radius, params[2], false);
                        if (creature.IsAlive()) {
                            if (!IsTrue(params[3]) || !params[3].GetLength())
                                removeAtEnd.push_back(creature);
                            pDSTGame->EntitySetCutsceneBehaviour(creature, (ECutsceneBehaviour)2);
                            CScriptGameResourceObjectScriptedThingBase scripted;
                            pDSTGame->StartScriptingEntity(creature, scripted, (EScriptAIPriority)4);
                            actors[params[2]] = scripted;
                        }
                    }
                } else if (command.StartsWith("RegisterScript")) {
                    if (params[0].GetLength()) {
                        questName = params[0];
                        pDSTGame->ActivateQuest(questName);
                    }
                } else if (command.StartsWith("SmashWindows")) {
                    if (params[0].GetLength() && params[1].GetLength()) {
                        CScriptThing thing = pDSTGame->GetThingWithScriptName(params[0]);
                        float radius = GFCharStringToFloat(params[1]);
                        if (thing.IsAlive())
                            pDSTGame->SmashAllWindowsWithinRadiusOfPoint(thing.GetPos(), radius);
                    }
                } else if (command.StartsWith("Create")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength()) {
                        CCharString scriptName(params[2]);
                        if (params[4].GetLength())
                            scriptName = params[4];
                        CCutsceneActorMap::iterator existing;
                        if (!IsTrue(params[5]) || !((existing = actors.find(params[2])) != actors.end())) {
                            CScriptThing position = (CCharString("HERO") == params[1]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[1]);
                            CScriptThing creature = pDSTGame->CreateCreature(params[0], position.GetPos(), scriptName, false);
                            if (creature.IsAlive()) {
                                if (IsTrue(params[3]) || !params[3].GetLength())
                                    removeAtEnd.push_back(creature);
                                if (!IsFalse(params[6])) {
                                    CScriptGameResourceObjectScriptedThingBase scripted;
                                    pDSTGame->EntitySetCutsceneBehaviour(creature, (ECutsceneBehaviour)2);
                                    pDSTGame->StartScriptingEntity(creature, scripted, (EScriptAIPriority)4);
                                    pDSTGame->EntitySetFacingAngle(creature, position.GetAngleXY(), true);
                                    actors[params[2]] = scripted;
                                }
                                pDSTGame->EntityBeginLoadingBasicAnimations(creature);
                            }
                        }
                    }
                } else if (command.StartsWith("ObjectCreate")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength()) {
                        CScriptThing position = (CCharString("HERO") == params[1]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[1]);
                        pDSTGame->CreateObject(params[0], position.GetPos(), params[2]);
                    }
                } else if (command.StartsWith("CrowdCreateMixed")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength() && params[3].GetLength()) {
                        std::vector<CScriptThing> markers;
                        pDSTGame->GetAllThingsWithScriptName(params[2], markers);
                        long count = markers.size();
                        std::vector<CScriptGameResourceObjectScriptedThingBase> scripted;
                        scripted.resize(count);
                        crowdSizes[params[3]] = count;
                        for (long i = 0; i < count; ++i) {
                            CScriptThing creature;
                            if (rand() % 2 == 1)
                                creature = pDSTGame->CreateCreature(params[0], markers[i].GetPos(), params[3], false);
                            else
                                creature = pDSTGame->CreateCreature(params[1], markers[i].GetPos(), params[3], false);
                            if (creature.IsAlive()) {
                                CCharString crowdName = params[3] + GFIntToCharString(i);
                                pDSTGame->EntitySetCutsceneBehaviour(creature, (ECutsceneBehaviour)2);
                                pDSTGame->StartScriptingEntity(creature, scripted[i], (EScriptAIPriority)4);
                                actors[crowdName] = scripted[i];
                                pDSTGame->EntitySetFacingAngle(creature, markers[i].GetAngleXY(), true);
                                if (!IsTrue(params[4]) || !params[4].GetLength())
                                    removeAtEnd.push_back(creature);
                            }
                        }
                    }
                } else if (command.StartsWith("CrowdCreate")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength()) {
                        std::map<CCharString, long>::iterator crowd;
                        crowd = crowdSizes.find(params[2]);
                        if (crowd == crowdSizes.end() || crowdSizes[params[2]] == 0) {
                            std::vector<CScriptThing> markers;
                            pDSTGame->GetAllThingsWithScriptName(params[1], markers);
                            long count = markers.size();
                            std::vector<CScriptGameResourceObjectScriptedThingBase> scripted;
                            scripted.resize(count);
                            crowdSizes[params[2]] = count;
                            for (long i = 0; i < count; ++i) {
                                CScriptThing creature = pDSTGame->CreateCreature(params[0], markers[i].GetPos(), params[2], false);
                                if (creature.IsAlive()) {
                                    CCharString crowdName = params[2] + GFIntToCharString(i);
                                    pDSTGame->EntitySetCutsceneBehaviour(creature, (ECutsceneBehaviour)2);
                                    pDSTGame->StartScriptingEntity(creature, scripted[i], (EScriptAIPriority)4);
                                    actors[crowdName] = scripted[i];
                                    pDSTGame->EntitySetFacingAngle(creature, markers[i].GetAngleXY(), true);
                                    if (IsTrue(params[3]) || !params[3].GetLength())
                                        removeAtEnd.push_back(creature);
                                }
                            }
                        }
                    }
                } else if (command.StartsWith("CreditScreen")) {
                    if (params[0].GetLength()) {
                        if (params[1].GetLength()) {
                            pDSTGame->ActivateQuest("Hook_Fresco_02_GoodEpilogue");
                            pDSTGame->ActivateQuest("Hook_Fresco_03_JacksObsession");
                            pDSTGame->ActivateQuest("Hook_Fresco_04_Kraken");
                            pDSTGame->ActivateQuest("Hook_Fresco_05_MothersStory");
                            pDSTGame->ActivateQuest("Hook_Fresco_06_Prison");
                            pDSTGame->ActivateQuest("Hook_Fresco_08_SistersStory");
                            pDSTGame->ActivateQuest("Hook_Fresco_07_OakValeRaid");
                            pDSTGame->ActivateQuest("Hook_Fresco_09_TimePassing");
                            pDSTGame->ActivateQuest("Hook_Fresco_10_UneasyAlliance");
                            pDSTGame->ActivateQuest("Hook_Fresco_11_Wedding");
                            pDSTGame->ActivateQuest("Hook_Fresco_12_KilledDragon");
                            pDSTGame->ActivateQuest("Hook_Fresco_13_KilledScorpion");
                        }
                        pDSTGame->KickOffCreditsScreen(params[0]);
                    }
                } else if (command.StartsWith("CrowdAcquire")) {
                    if (params[0].GetLength()) {
                        CCharString crowdName;
                        CCharString searchName(params[0]);
                        if (params[1].GetLength() == 0)
                            crowdName = searchName;
                        else
                            crowdName = params[1];
                        std::map<CCharString, long>::iterator crowd;
                        crowd = crowdSizes.find(crowdName);
                        if (crowd == crowdSizes.end()) {
                            std::vector<CScriptThing> things;
                            if (params[3].EqualsCaseInsensitive("def")) {
                                pDSTGame->GetAllThingsWithDefName(searchName, things);
                            } else if (params[3].EqualsCaseInsensitive("consec")) {
                                long index = 0;
                                CCharString thingName = searchName + GFIntToCharString(index);
                                CScriptThing thing = pDSTGame->GetThingWithScriptName(thingName);
                                while (thing.IsAlive()) {
                                    things.push_back(thing);
                                    ++index;
                                    thingName = searchName + GFIntToCharString(index);
                                    thing = pDSTGame->GetThingWithScriptName(thingName);
                                }
                            } else {
                                pDSTGame->GetAllThingsWithScriptName(searchName, things);
                            }
                            if (IsTrue(params[4])) {
                                for (std::vector<CScriptThing>::iterator it = things.begin(); it != things.end(); ++it)
                                    if ((*it).GetName() != "")
                                        things.erase(it);
                            }
                            long count = things.size();
                            std::vector<CScriptGameResourceObjectScriptedThingBase> scripted;
                            scripted.resize(count);
                            long acquired = 0;
                            for (long i = 0; i < count; ++i) {
                                if (things[i].IsAlive() && pDSTGame->GetHealth(things[i]) > 0.0001f) {
                                    CCharString memberName = crowdName + GFIntToCharString(i);
                                    pDSTGame->EntitySetCutsceneBehaviour(things[i], (ECutsceneBehaviour)2);
                                    pDSTGame->StartScriptingEntity(things[i], scripted[i], (EScriptAIPriority)4);
                                    actors[memberName] = scripted[i];
                                    ++acquired;
                                    if (IsTrue(params[2]))
                                        removeAtEnd.push_back(things[i]);
                                }
                            }
                            crowdSizes[crowdName] = acquired;
                        }
                    }
                } else if (command.StartsWith("FallbackAcquire")) {
                    if (params[0].GetLength()) {
                        bool found = false;
                        CScriptThing thing;
                        std::vector<CScriptThing> things;
                        for (long k = 1; !found && params[k].GetLength(); ++k) {
                            pDSTGame->GetAllThingsWithDefName(params[k], things);
                            if (things.size() > 0) {
                                for (unsigned long i = 0; i < things.size() && !found; ++i) {
                                    thing = things[i];
                                    if (thing.IsAlive() && pDSTGame->GetHealth(thing) > 0.0001f) {
                                        CThing* pThing = thing.GetPThing();
                                        if (IS_THING_ALIVE(pThing) && pThing->GetType() != 13)
                                            found = true;
                                    }
                                }
                                if (found) {
                                    CScriptGameResourceObjectScriptedThingBase scripted;
                                    pDSTGame->EntitySetCutsceneBehaviour(thing, (ECutsceneBehaviour)2);
                                    pDSTGame->StartScriptingEntity(thing, scripted, (EScriptAIPriority)4);
                                    actors[params[0]] = scripted;
                                } else {
                                    things.clear();
                                }
                            }
                        }
                    }
                } else if (command.StartsWith("CrowdRipplePosition")) {
                    if (params[0].GetLength() && params[1].GetLength()) {
                        std::map<CCharString, long>::iterator crowd;
                        crowd = crowdSizes.find(params[0]);
                        if (crowd != crowdSizes.end() && crowdSizes[params[0]] > 0) {
                            long count = crowdSizes[params[0]];
                            CScriptThing member;
                            CCharString memberName;
                            CCharString markerName;
                            CScriptThing marker;
                            for (long i = 0; i < count; ++i) {
                                markerName = params[1] + GFIntToCharString(i);
                                marker = pDSTGame->GetThingWithScriptName(markerName);
                                memberName = params[0] + GFIntToCharString(i);
                                FABLE_RETAIL_ASSERT(i < count);
                                if (actors[memberName].IsValid()) {
                                    member = actors[memberName].GetScriptThing();
                                    if (member.IsAlive() && marker.IsAlive())
                                        pDSTGame->EntityTeleportToPosition(member, marker.GetPos(), marker.GetAngleXY(), false, false);
                                }
                            }
                        }
                    }
                } else if (command.StartsWith("CrowdTeleportRipple")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength() && params[3].GetLength()) {
                        std::map<CCharString, long>::iterator crowd;
                        crowd = crowdSizes.find(params[0]);
                        if (crowd != crowdSizes.end() && crowdSizes[params[0]] > 0) {
                            long count = crowdSizes[params[0]];
                            CScriptThing member;
                            CCharString memberName;
                            CCharString markerName;
                            CScriptThing marker;
                            float delay = GFCharStringToFloat(params[2]) * 15.0f;
                            FABLE_RETAIL_ASSERT(delay >= 0.0f);
                            float interval = GFCharStringToFloat(params[3]) * 15.0f;
                            FABLE_RETAIL_ASSERT(interval >= 0.0f);
                            float waited = 0.0f;
                            float timer = 0.0f;
                            if (IsFalse(params[4])) {
                                for (long i = 0; i < count; ++i) {
                                    markerName = params[1] + GFIntToCharString(i);
                                    marker = pDSTGame->GetThingWithScriptName(markerName);
                                    memberName = params[0] + GFIntToCharString(i);
                                    FABLE_RETAIL_ASSERT(i < count);
                                    if (actors[memberName].IsValid()) {
                                        member = actors[memberName].GetScriptThing();
                                        if (member.IsAlive() && marker.IsAlive())
                                            pDSTGame->EntityTeleportToThing(member, marker, false);
                                    }
                                }
                            }
                            while (waited < delay && !IsCutsceneSkipped()) {
                                timer += 1.0f;
                                waited += 1.0f;
                                CUTSCENE_FRAME();
                            }
                            long markerIndex = count;
                            long memberIndex = 0;
                            while (marker.IsAlive() && timer < interval && !IsCutsceneSkipped()) {
                                if (memberIndex == count)
                                    memberIndex = 0;
                                memberName = params[0] + GFIntToCharString(memberIndex);
                                FABLE_RETAIL_ASSERT(memberIndex < count);
                                if (actors[memberName].IsValid()) {
                                    member = actors[memberName].GetScriptThing();
                                    while (timer < interval &&
                                           (pDSTGame->IsCameraPosOnScreen(member.GetPos()) ||
                                            pDSTGame->IsCameraPosOnScreen(member.GetPos() + C3DVector(0.0f, 0.0f, 1.0f))) &&
                                           !IsCutsceneSkipped()) {
                                        timer += 1.0f;
                                        CUTSCENE_FRAME();
                                    }
                                    markerName = params[1] + GFIntToCharString(markerIndex);
                                    marker = pDSTGame->GetThingWithScriptName(markerName);
                                    CUTSCENE_FRAME();
                                    if (member.IsAlive() && marker.IsAlive())
                                        pDSTGame->EntityTeleportToThing(member, marker, false);
                                }
                                ++markerIndex;
                                ++memberIndex;
                            }
                            while (timer < interval && !IsCutsceneSkipped()) {
                                timer += 1.0f;
                                CUTSCENE_FRAME();
                            }
                        }
                    }
                } else if (command.StartsWith("CrowdTeleport")) {
                    if (params[0].GetLength() && params[1].GetLength()) {
                        if (IsFalse(params[2])) {
                            std::vector<CScriptThing> crowd;
                            pDSTGame->GetAllThingsWithScriptName(params[0], crowd);
                            if (crowd.size() > 0) {
                                std::vector<CScriptThing> markers;
                                pDSTGame->GetAllThingsWithScriptName(params[1], markers);
                                for (unsigned long i = 0; i < crowd.size() && i < markers.size(); ++i)
                                    if (crowd[i].IsAlive() && markers[i].IsAlive())
                                        pDSTGame->EntityTeleportToThing(crowd[i], markers[i], false);
                            }
                        } else {
                            std::map<CCharString, long>::iterator crowd;
                            crowd = crowdSizes.find(params[0]);
                            if (crowd != crowdSizes.end() && crowdSizes[params[0]] > 0) {
                                long count = crowdSizes[params[0]];
                                CCharString memberName;
                                std::vector<CScriptThing> markers;
                                pDSTGame->GetAllThingsWithScriptName(params[1], markers);
                                for (long i = 0; i < count && (unsigned long)i < markers.size(); ++i) {
                                    memberName = params[0] + GFIntToCharString(i);
                                    FABLE_RETAIL_ASSERT(i < count);
                                    if (actors[memberName].IsValid() && markers[i].IsAlive())
                                        pDSTGame->EntityTeleportToThing(actors[memberName].GetScriptThing(), markers[i], false);
                                }
                            }
                        }
                    }
                } else if (command.StartsWith("CrowdMove")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength() && params[3].GetLength()) {
                        std::map<CCharString, long>::iterator crowd;
                        crowd = crowdSizes.find(params[0]);
                        if (crowd != crowdSizes.end() && crowdSizes[params[0]] > 0) {
                            CScriptThing target = CSFindThing(params[1], actors);
                            CScriptGameResourceObjectScriptedThingBase member;
                            long count = crowdSizes[params[0]];
                            float proximity = 1.0f;
                            if (params[2].GetLength())
                                proximity = GFCharStringToFloat(params[2]);
                            FABLE_RETAIL_ASSERT(proximity > 0.0f);
                            EScriptEntityMoveType moveType;
                            if (params[3] == "SNEAK")
                                moveType = (EScriptEntityMoveType)2;
                            else if (params[3] == "RUN")
                                moveType = (EScriptEntityMoveType)1;
                            else
                                moveType = (EScriptEntityMoveType)0;
                            for (long i = 0; i < count; ++i) {
                                CCharString memberName = params[0] + GFIntToCharString(i);
                                FABLE_RETAIL_ASSERT(i < count);
                                member = actors[memberName];
                                if (member.IsValid())
                                    member.MoveToThing(target, proximity, moveType, false, true, false, true);
                            }
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith("CrowdClearActions")) {
                    if (params[0].GetLength()) {
                        std::map<CCharString, long>::iterator crowd;
                        crowd = crowdSizes.find(params[0]);
                        if (crowd != crowdSizes.end() && crowdSizes[params[0]] > 0) {
                            CScriptGameResourceObjectScriptedThingBase member;
                            long count = crowdSizes[params[0]];
                            for (long i = 0; i < count; ++i) {
                                CCharString memberName = params[0] + GFIntToCharString(i);
                                FABLE_RETAIL_ASSERT(i < count);
                                member = actors[memberName];
                                if (member.IsValid()) {
                                    member.ClearAllActionsIncludingLoopingAnimations();
                                    member.ClearCommands();
                                }
                            }
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith("CrowdAnimate")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength() && params[3].GetLength()) {
                        std::map<CCharString, long>::iterator crowd;
                        crowd = crowdSizes.find(params[0]);
                        if (crowd != crowdSizes.end() && crowdSizes[params[0]] > 0) {
                            CScriptGameResourceObjectScriptedThingBase member;
                            long count = crowdSizes[params[0]];
                            CCharString anim(params[1]);
                            float duration = GFCharStringToFloat(params[3]);
                            FABLE_RETAIL_ASSERT(duration >= 0.0f);
                            long loops = -1;
                            bool overwriteExistingActions = false;
                            bool looping = false;
                            if (IsTrue(params[6]))
                                looping = true;
                            if (params[7].GetLength())
                                loops = GFCharStringToInt(params[7]);
                            if (IsTrue(params[8]))
                                overwriteExistingActions = true;
                            if (duration == 0.0f) {
                                bool allowLooking = false;
                                if (crowdLooking)
                                    allowLooking = true;
                                for (long i = 0; i < count; ++i) {
                                    CCharString memberName = params[0] + GFIntToCharString(i);
                                    FABLE_RETAIL_ASSERT(i < count);
                                    member = actors[memberName];
                                    if (member.IsValid()) {
                                        if (looping)
                                            member.PlayLoopingAnimation(anim, loops, false, overwriteExistingActions, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, allowLooking);
                                        else
                                            member.PlayAnimation(anim, false, overwriteExistingActions, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, allowLooking);
                                    }
                                }
                            } else {
                                float totalTime = GFCharStringToFloat(params[2]);
                                FABLE_RETAIL_ASSERT(totalTime > 0.0f);
                                long randomRange = 0;
                                std::vector<CScriptGameResourceObjectScriptedThingBase> members;
                                for (long i = 0; i < count; ++i) {
                                    CCharString memberName = params[0] + GFIntToCharString(i);
                                    member = actors[memberName];
                                    if (member.IsValid())
                                        members.push_back(member);
                                }
                                long memberCount = members.size();
                                FABLE_RETAIL_ASSERT(memberCount > 0);
                                long played = 1;
                                long index = 0;
                                bool sequential = false;
                                long frame = 0;
                                long frames = 0;                // FITTED: declared with the block's other counters (dead store, no bytes); one more
                                                                // c2 record before `random`, which then opens a new record batch (0x00CCE8EA)
                                float random = 0.0f;
                                float time = 0.0f;
                                if (duration < 0.01f)
                                    duration = 0.01f;
                                if (params[4].GetLength())
                                    randomRange = (long)(GFCharStringToFloat(params[4]) * 100.0f);
                                if (IsTrue(params[5]))
                                    sequential = true;
                                while (time < totalTime && !IsCutsceneSkipped()) {
                                    if (sequential)
                                        ++index;
                                    else
                                        index = rand() % memberCount;
                                    if (index >= memberCount) {
                                        if (looping)
                                            index = -1;
                                        else
                                            index = 0;
                                    }
                                    if (index >= 0) {
                                        member = members[index];
                                        if (!member.IsPerformingScriptTask()) {
                                            if (looping)
                                                member.PlayLoopingAnimation(anim, loops, false, overwriteExistingActions, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
                                            else
                                                member.PlayAnimation(anim, false, overwriteExistingActions, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
                                        }
                                        ++played;
                                        if (randomRange > 0)
                                            random = (rand() % randomRange) * 0.01f;
                                        frames = (long)(((duration + random) + time) * 15.0f);
                                        while (played > 4 || (time < totalTime && frame < frames && !IsCutsceneSkipped())) {
                                            time += 1.0f / 15.0f;
                                            played = 0;
                                            ++frame;
                                            CUTSCENE_FRAME();
                                        }
                                    } else {
                                        while (time < totalTime && !IsCutsceneSkipped()) {
                                            time += 1.0f / 15.0f;
                                            CUTSCENE_FRAME();
                                        }
                                    }
                                }
                            }
                        }
                    }
                } else if (command.StartsWith("CrowdCombatAnimate")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength() && params[3].GetLength()) {
                        std::map<CCharString, long>::iterator crowd;
                        crowd = crowdSizes.find(params[0]);
                        if (crowd != crowdSizes.end() && crowdSizes[params[0]] > 0) {
                            CCharString anim(params[1]);
                            float duration = GFCharStringToFloat(params[3]);
                            FABLE_RETAIL_ASSERT(duration >= 0.0f);
                            bool overwriteExistingActions = false;
                            bool addAsQueuedAction = false;
                            bool waitForAnimToFinish = true;
                            if (IsTrue(params[5]))
                                overwriteExistingActions = true;
                            if (IsTrue(params[6]))
                                addAsQueuedAction = true;
                            if (IsFalse(params[7]))
                                waitForAnimToFinish = false;
                            if (duration == 0.0f) {
                                CScriptGameResourceObjectScriptedThingBase member;
                                long count = crowdSizes[params[0]];
                                for (long i = 0; i < count; ++i) {
                                    CCharString memberName = params[0] + GFIntToCharString(i);
                                    FABLE_RETAIL_ASSERT(i < count);
                                    member = actors[memberName];
                                    if (member.IsValid())
                                        member.PlayCombatAnimation(anim, true, overwriteExistingActions, addAsQueuedAction, waitForAnimToFinish, false, false);
                                }
                            } else {
                                float totalTime = GFCharStringToFloat(params[2]);
                                FABLE_RETAIL_ASSERT(totalTime > 0.0f);
                                long randomRange = 0;
                                CScriptGameResourceObjectScriptedThingBase member;
                                std::vector<CScriptGameResourceObjectScriptedThingBase> members;
                                long count = crowdSizes[params[0]];
                                for (long i = 0; i < count; ++i) {
                                    CCharString memberName = params[0] + GFIntToCharString(i);
                                    member = actors[memberName];
                                    if (member.IsValid())
                                        members.push_back(member);
                                }
                                long memberCount = members.size();
                                FABLE_RETAIL_ASSERT(memberCount > 0);
                                float random = 0.0f;
                                float time = 0.0f;
                                if (duration < 0.05f)
                                    duration = 0.05f;
                                if (params[4].GetLength())
                                    randomRange = (long)(GFCharStringToFloat(params[4]) * 100.0f);
                                while (time < totalTime && !IsCutsceneSkipped()) {
                                    member = members[rand() % memberCount];
                                    if (!member.IsPerformingScriptTask())
                                        member.PlayCombatAnimation(anim, true, false, false, true, false, false);
                                    if (randomRange > 0)
                                        random = (rand() % randomRange) * 0.01f;
                                    float frames = (random + duration) * 15.0f;
                                    while (time < totalTime && frames > 0.0f && !IsCutsceneSkipped()) {
                                        frames -= 1.0f;
                                        time += 1.0f / 15.0f;
                                        CUTSCENE_FRAME();
                                    }
                                }
                            }
                        }
                    }
                } else if (command.StartsWith("CrowdCollide")) {
                    std::map<CCharString, long>::iterator crowd;
                    crowd = crowdSizes.find(params[0]);
                    if (crowd != crowdSizes.end() && crowdSizes[params[0]] > 0) {
                        CScriptGameResourceObjectScriptedThingBase member;
                        CScriptThing thing;
                        long count = crowdSizes[params[0]];
                        CCharString memberName;
                        bool collide = false;
                        if (IsTrue(params[1]))
                            collide = true;
                        for (long i = 0; i < count; ++i) {
                            memberName = params[0] + GFIntToCharString(i);
                            FABLE_RETAIL_ASSERT(i < count);
                            member = actors[memberName];
                            if (member.IsValid()) {
                                thing = member.GetScriptThing();
                                pDSTGame->EntitySetAsCollidableToThings(thing, collide);
                                collideThings.push_back(thing);
                            }
                        }
                    }
                } else if (command.StartsWith("CrowdLookTo")) {
                    std::map<CCharString, long>::iterator crowd;
                    crowd = crowdSizes.find(params[0]);
                    if (crowd != crowdSizes.end() && crowdSizes[params[0]] > 0) {
                        CScriptGameResourceObjectScriptedThingBase member;
                        CScriptThing thing;
                        long count = crowdSizes[params[0]];
                        CCharString memberName;
                        bool nothing = false;
                        bool forever = false;
                        CScriptThing target;
                        if (params[1].EqualsCaseInsensitive("nothing")) {
                            nothing = true;
                        } else {
                            crowdLooking = true;
                            target = CSFindThing(params[1], actors);
                        }
                        if (params[2].EqualsCaseInsensitive("forever"))
                            forever = true;
                        for (long i = 0; i < count; ++i) {
                            memberName = params[0] + GFIntToCharString(i);
                            FABLE_RETAIL_ASSERT(i < count);
                            member = actors[memberName];
                            if (member.IsValid()) {
                                thing = member.GetScriptThing();
                                if (nothing) {
                                    pDSTGame->EntityForceToLookAtNothing(thing);
                                    lookingThings.push_back(thing);
                                } else if (target.IsAlive()) {
                                    if (forever)
                                        pDSTGame->SetLookForFiniteTime(thing, false);
                                    else
                                        pDSTGame->SetLookForFiniteTime(thing, true);
                                    pDSTGame->EntityForceToLookAtThing(thing, target);
                                }
                            }
                        }
                    }
                } else if (command.StartsWith("CrowdLookAt")) {
                    if (params[0].GetLength() && params[1].GetLength()) {
                        std::vector<CScriptThing> crowd;
                        pDSTGame->GetAllThingsWithScriptName(params[0], crowd);
                        if (crowd.size() > 0) {
                            CScriptThing target = CSFindThing(params[1], actors);
                            for (unsigned long i = 0; i < crowd.size(); ++i)
                                pDSTGame->EntitySetFacingAngleTowardsThing(crowd[i], target, true);
                        }
                        CUTSCENE_FRAME();
                    }
                } else if (command.StartsWith("CrowdKill")) {
                    if (params[0].GetLength()) {
                        std::map<CCharString, long>::iterator crowd;
                        crowd = crowdSizes.find(params[0]);
                        float timer = 0.0f;
                        float delay = 0.0f;
                        bool b84 = true;
                        if (params[1].GetLength())
                            delay = GFCharStringToFloat(params[1]) * 15.0f;
                        if (IsFalse(params[2]))
                            b84 = false;
                        if (!(crowd == crowdSizes.end()) && !IsFalse(params[3])) {
                            if (crowdSizes[params[0]] > 0) {
                                long count = crowdSizes[params[0]];
                                CCharString memberName;
                                for (long i = 0; i < count; ++i) {
                                    memberName = params[0] + GFIntToCharString(i);
                                    FABLE_RETAIL_ASSERT(i < count);
                                    if (actors[memberName].IsValid()) {
                                        pDSTGame->RemoveThing(actors[memberName].GetScriptThing(), b84, true);
                                        if (delay > 0.0f && timer < delay && !IsCutsceneSkipped()) {
                                            timer += 1.0f;
                                            CUTSCENE_FRAME();
                                        }
                                    }
                                }
                            }
                        } else {
                            std::vector<CScriptThing> things;
                            pDSTGame->GetAllThingsWithScriptName(params[0], things);
                            for (unsigned long i = 0; i < things.size(); ++i) {
                                if (things[i].IsAlive()) {
                                    pDSTGame->RemoveThing(things[i], b84, true);
                                    if (delay > 0.0f && timer < delay && !IsCutsceneSkipped()) {
                                        timer += 1.0f;
                                        CUTSCENE_FRAME();
                                    }
                                }
                            }
                        }
                        while (delay > 0.0f && timer < delay && !IsCutsceneSkipped()) {
                            timer += 1.0f;
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith("ScriptEntity")) {
                    if (params[0].GetLength() && params[1].GetLength()) {
                        CScriptThing thing = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        EScriptAIPriority priority = (EScriptAIPriority)4;
                        if (params[2].EqualsCaseInsensitive("medium"))
                            priority = (EScriptAIPriority)2;
                        else if (params[2].EqualsCaseInsensitive("high"))
                            priority = (EScriptAIPriority)3;
                        else if (params[2].EqualsCaseInsensitive("low"))
                            priority = (EScriptAIPriority)1;
                        if (thing.IsAlive())
                            pDSTGame->StartScriptingEntity(thing, actors[params[1]], priority);
                    }
                } else if (command.StartsWith("UseTheme")) {
                    if (params[0].GetLength() && !IsNull(params[0])) {
                        float time = 0.0f;
                        if (params[1].GetLength())
                            time = GFCharStringToFloat(params[1]);
                        if (!params[2].GetLength() || IsTrue(params[2]))
                            resetThemeAtEnd = true;
                        if (params[0].EqualsCaseInsensitive("RESET"))
                            pDSTGame->ResetToDefaultTheme(time);
                        else
                            pDSTGame->TransitionToTheme(params[0], time);
                    }
                } else if (command.StartsWith("TakeFromHero")) {
                    if (params[0].GetLength())
                        pDSTGame->ConfiscateItemsOfTypeFromHero(params[0]);
                } else if (command.StartsWith("PutInHeroHands")) {
                    if (params[0].GetLength()) {
                        if (IsNull(params[0])) {
                            CCharString nothing("");
                            pDSTGame->MakeHeroCarryItemInHand(nothing);
                        } else if (params[1].EqualsCaseInsensitive("NAME"))
                            pDSTGame->MakeHeroCarryItemInHand(params[0]);
                        else
                            pDSTGame->MakeHeroCarryItemInHand((CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]), true, true);
                    }
                } else if (command.StartsWith("SetHeroWeapon")) {
                    if (params[0].GetLength())
                        pDSTGame->SetWeaponAsHerosActiveWeapon(params[0]);
                } else if (command.StartsWith("Get")) {
                    if (params[0].GetLength() && params[1].GetLength()) {
                        CScriptThing thing = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        CScriptGameResourceObjectScriptedThingBase scripted;
                        pDSTGame->StartScriptingEntity(thing, scripted, (EScriptAIPriority)4);
                        actors[params[1]] = scripted;
                        pDSTGame->EntitySetCutsceneBehaviour(thing, (ECutsceneBehaviour)2);
                    }
                } else if (command.StartsWith("WaitForMessageCamera")) {
                    if (params[0].GetLength()) {
                        CCharString message(params[0]);
                        while (!pDSTGame->CheckForCameraMessage(message)) {
                            if (IsCutsceneSkipped())
                                break;
                            CUTSCENE_FRAME();
                            if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                break;
                        }
                    }
                } else if (command.StartsWith("RemoveEffect")) {
                    if (params[0].GetLength()) {
                        for (unsigned long i = 0; i < removeAtEnd.size(); ++i)
                            if (removeAtEnd[i].GetDataString() == params[0])
                                pDSTGame->RemoveThing(removeAtEnd[i], false, true);
                    }
                } else if (command.StartsWith("Remove")) {
                    if (params[0].GetLength()) {
                        if (params[1].EqualsCaseInsensitive("dead")) {
                            pDSTGame->RemoveDeadCreature(params[0]);
                        } else {
                            bool withFade = false;
                            CScriptThing thing = CSFindThing(params[0], actors);
                            if (IsTrue(params[1]))
                                withFade = true;
                            if (thing.IsAlive())
                                pDSTGame->RemoveThing(thing, withFade, true);
                        }
                    }
                } else if (command.StartsWith("ExitGame")) {
                    pDSTGame->ResetToFrontEnd();
                } else if (command.StartsWith("PutInFrontOf")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength()) {
                        CScriptThing thing = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        if (thing.IsAlive()) {
                            CScriptThing target = (CCharString("HERO") == params[1]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[1]);
                            CThing* pTarget = target.GetPThing();
                            CTCPhysicsBase* physics;
                            if (pTarget->GetTC(TCI_PHYSICS, &physics)) {
                                float distance = GFCharStringToFloat(params[2]);
                                pDSTGame->EntityTeleportToPosition(thing, physics->GetRHSet().GetForward() * distance + physics->GetPosition(), 0.0f, false, false);
                                pDSTGame->EntitySetFacingAngleTowardsThing(actor->GetScriptThing(), target, true);
                            }
                        }
                    }
                } else if (command.StartsWith("Print")) {
                    if (params[0].GetLength()) {
                        float duration = 5.0f;
                        if (params[1].GetLength())
                            duration = GFCharStringToFloat(params[1]);
                        pDSTGame->AddScreenTitleMessage(params[0], duration, false);
                    }
                } else if (command.StartsWith("HUD")) {
                    if (IsTrue(params[0]))
                        pDSTGame->SetHUDEnabled(true);
                    else
                        pDSTGame->SetHUDEnabled(false);
                } else if (command.StartsWith("Fullscreen")) {
                    pDSTGame->EndLetterBox();
                } else if (command.StartsWith("GameInfo")) {
                    if (params[0].GetLength()) {
                        if (g_ScriptFormattedText != L"")
                            pDSTGame->DisplayGameInfoText(g_ScriptFormattedText);
                        else
                            pDSTGame->DisplayGameInfo(params[0]);
                        while (!pDSTGame->MsgIsGameInfoClickedPast()) {
                            if (IsCutsceneSkipped())
                                break;
                            CUTSCENE_FRAME();
                            if (g_pScriptRunner && g_pScriptRunner->StopRunning)
                                break;
                        }
                    }
                } else if (command.StartsWith("SetTime")) {
                    if (params[0].GetLength()) {
                        float time = GFCharStringToFloat(params[0]);
                        if (!IsFalse(params[1]))
                            timeOfDay = (float)pDSTGame->GetTimeOfDay();
                        float duration;
                        if (params[2].GetLength() && (duration = GFCharStringToFloat(params[2])) > 0.0f)
                            pDSTGame->FastForwardTimeTo(time, duration);
                        else
                            pDSTGame->SetTimeOfDay(time);
                    }
                } else if (command.StartsWith("StayFadedOut")) {
                    fadeInAtEnd = false;
                } else if (command.StartsWith("FadeIn")) {
                    pDSTGame->FadeScreenIn();
                } else if (command.StartsWith("FadeOut")) {
                    if (!pDSTGame->IsScreenFadingOut() || params[3].EqualsCaseInsensitive("override")) {
                        float time = 0.5f;
                        float hold = 0.5f;
                        CRGBColour colour(0, 0, 0, 255);
                        if (params[0].GetLength())
                            time = GFCharStringToFloat(params[0]);
                        if (params[1].GetLength())
                            hold = GFCharStringToFloat(params[1]);
                        if (!params[2].GetLength() || params[2].EqualsCaseInsensitive("black")) {
                            pDSTGame->FadeScreenOutUntilNextCallToFadeScreenIn(time, hold);
                        } else {
                            if (params[2].EqualsCaseInsensitive("white"))
                                colour = CRGBColour(255, 255, 255, 255);
                            else if (params[2].EqualsCaseInsensitive("red"))
                                colour = CRGBColour(255, 0, 0, 255);
                            else if (params[2].EqualsCaseInsensitive("fire"))
                                colour = CRGBColour(152, 79, 0, 255);
                            pDSTGame->FadeScreenOut(time, hold, colour);
                        }
                    }
                } else if (command.StartsWith("TintScreenTo")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength() && params[3].GetLength() &&
                        params[4].GetLength() && params[5].GetLength() && params[6].GetLength()) {
                        float fadeTime = GFCharStringToFloat(params[0]);
                        float p1 = GFCharStringToFloat(params[1]);
                        float p2 = GFCharStringToFloat(params[2]);
                        float p3 = GFCharStringToFloat(params[3]);
                        float p4 = GFCharStringToFloat(params[4]);
                        float blue = 0.0f;
                        float green = 0.0f;
                        float red = 0.0f;
                        std::vector<CCharString> values;
                        if (CSExtractParameterList(params[5], values) == 3) {
                            red = GFCharStringToFloat(values[0]);
                            green = GFCharStringToFloat(values[1]);
                            blue = GFCharStringToFloat(values[2]);
                        }
                        CRGBFloatColour colour;
                        colour.R = red / 255.0f;
                        colour.G = green / 255.0f;
                        colour.B = blue / 255.0f;
                        values.clear();
                        std::vector<CScreenFilterSThingByPass> bypass;
                        if (CSExtractParameterList(params[6], values) > 0) {
                            unsigned long count = values.size();
                            for (unsigned long i = 0; i < count; ++i) {
                                CCharString& value = values[i];
                                if (value.StartsWith("ALL:") || value.StartsWith("ALLDEF:")) {
                                    CCharString name = value.GetSubString(value.FindCharPos(':') + 1, value.GetLength());
                                    std::vector<CScriptThing> things;
                                    if (value.StartsWith("ALL:"))
                                        pDSTGame->GetAllThingsWithScriptName(name, things);
                                    else
                                        pDSTGame->GetAllThingsWithDefName(name, things);
                                    for (unsigned long j = 0; j < things.size(); ++j)
                                        if (things[j].IsAlive())
                                            bypass.push_back(CScreenFilterSThingByPass(things[j], true));
                                } else {
                                    CScriptThing thing = (CCharString("HERO") == value) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(value);
                                    if (thing.IsAlive())
                                        bypass.push_back(CScreenFilterSThingByPass(thing, true));
                                }
                            }
                        }
                        screenFilter = pDSTGame->ScreenFilterFadeTo(fadeTime, p1, p2, p3, p4, colour, bypass);
                    }
                } else if (command.StartsWith("TintScreenOut")) {
                    if (params[0].GetLength()) {
                        float fadeOutTime = GFCharStringToFloat(params[0]);   // evaluated before the pDSTGame load (0x00CD11E0)
                        pDSTGame->ScreenFilterFadeOut(fadeOutTime, screenFilter);
                        screenFilter = 0;
                    }
                } else if (command.StartsWith("CameraEffect")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength()) {
                        float a = GFCharStringToFloat(params[0]);
                        float b = GFCharStringToFloat(params[1]);
                        float c = GFCharStringToFloat(params[2]);
                        pDSTGame->CameraUseScreenEffect(a, b, c);
                    }
                } else if (command.StartsWith("CameraShake")) {
                    if (params[0].GetLength() && params[1].GetLength()) {
                        float duration = GFCharStringToFloat(params[1]);
                        float magnitude = GFCharStringToFloat(params[0]);
                        pDSTGame->CameraShake(duration, magnitude);
                    }
                } else if (command.StartsWith("StartTimeCode")) {
                    g_CutsceneSoundFrame = 0;
                } else if (command.StartsWith("SetLightScene")) {
                    if (params[0].GetLength()) {
                        unsigned long scene = GFCharStringToInt(params[0]);
                        const std::vector<CCharString>& scenes = def->LightScene;
                        if (scene < scenes.size()) {
                            const std::vector<CCharString>& lights = def->Lights;
                            for (unsigned long i = 0; i < lights.size(); ++i) {
                                CCharString lightName;
                                CCharString lightLine(lights[i]);
                                unsigned char red;
                                unsigned char green;
                                unsigned char blue;
                                CutsceneMacro_ReadLightScene(lightLine, lightName, &red, &green, &blue);
                                CScriptThing light = (CCharString("HERO") == lightName) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(lightName);
                                if (light.IsAlive())
                                    pDSTGame->SetLightColour(light, CRGBColour(0, 0, 0, 255));
                            }
                            CCharString value;
                            for (long j = 0; j < scenes[scene].GetLength(); ++j) {
                                if (((const char*)scenes[scene])[j] == ',') {
                                    long index = GFCharStringToInt(value);
                                    CCharString lightName;
                                    CCharString lightLine(lights[index]);
                                    unsigned char red;
                                    unsigned char green;
                                    unsigned char blue;
                                    CutsceneMacro_ReadLightScene(lightLine, lightName, &red, &green, &blue);
                                    CScriptThing light = (CCharString("HERO") == lightName) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(lightName);
                                    if (light.IsAlive())
                                        pDSTGame->SetLightColour(light, CRGBColour(red, green, blue, 255));
                                    value = "";
                                } else if (((const char*)scenes[scene])[j] != ' ') {
                                    value.Append(((const char*)scenes[scene])[j], value.GetLength());
                                }
                            }
                            CUTSCENE_FRAME();
                        }
                    }
                }

                if (setup == true && (commands.size() == 0 || line == (long)commands.size() - 1)) {
                    commands.resize(0);
                    commands = def->Macro;
                    CutsceneCameraPreloader(commands, true, &g_CutsceneSoundFrame);
                    line = -1;
                    setup = false;
                }
            }
        }
        CPlayerGui::Get().GetSubtitleRenderer().SetAsInCutscene(false);
        CPlayerGui::Get().GetSubtitleRenderer().SetCutsceneAsSkippable(true);
        if (skipping && fadeInAtEnd)
            pDSTGame->FadeScreenIn();
        if (actors.size() != 0 && !keepActors) {
            CCutsceneActorMap::iterator it = actors.begin();
            CScriptThing thing;
            for (; it != actors.end(); ++it) {
                if ((*it).second.IsValid() == true) {
                    (*it).second.ClearAllActionsIncludingLoopingAnimations();
                    (*it).second.ClearCommands();
                    thing = (*it).second.GetScriptThing();
                    if (thing.IsAlive())
                        pDSTGame->SetLookForFiniteTime(thing, true);
                }
            }
            actors.clear();
        }
        if (restoreFollowers) {
            for (unsigned int i = 0; i < fadedFollowers.size(); ++i)
                if (fadedFollowers[i].IsAlive())
                    pDSTGame->EntityFadeIn(fadedFollowers[i], 0.5f);
        }
        pDSTGame->SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(false);
        if (engageThings.size() != 0)
            for (unsigned int i = 0; i < engageThings.size(); ++i)
                if (!engageThings[i].IsNull() && engageThings[i].IsAlive())
                    pDSTGame->EntitySetAbleToBeEngagedInCombat(engageThings[i], true);
        if (damageThings.size() != 0)
            for (unsigned int i = 0; i < damageThings.size(); ++i)
                if (!damageThings[i].IsNull() && damageThings[i].IsAlive())
                    pDSTGame->EntitySetAsDamageable(damageThings[i], true);
        if (removeAtEnd.size() != 0)
            for (unsigned int i = 0; i < removeAtEnd.size(); ++i)
                if (!removeAtEnd[i].IsNull() && removeAtEnd[i].IsAlive() == true)
                    pDSTGame->RemoveThing(removeAtEnd[i], false, true);
        if (lookingThings.size() != 0)
            for (unsigned int i = 0; i < lookingThings.size(); ++i)
                if (!lookingThings[i].IsNull() && lookingThings[i].IsAlive() == true)
                    pDSTGame->EntityResetForceToLookAt(lookingThings[i]);
        if (collideThings.size() != 0)
            for (unsigned int i = 0; i < collideThings.size(); ++i)
                if (!collideThings[i].IsNull() && collideThings[i].IsAlive() == true)
                    pDSTGame->EntitySetAsCollidableToThings(collideThings[i], true);
        if (timeOfDay != -1.0f)
            pDSTGame->SetTimeOfDay(timeOfDay);
        if (stopMusicAtEnd)
            pDSTGame->StopOverrideMusic(true);
        if (weaponState == "MELEE") {
            CScriptThing hero = pDSTGame->GetHero();
            pDSTGame->EntityUnsheatheMeleeWeapon(hero, false);
        }
        if (weaponState == "RANGED") {
            CScriptThing hero = pDSTGame->GetHero();
            pDSTGame->EntityUnsheatheRangedWeapon(hero, false);
        }
        if (relightHandLamp)
            pDSTGame->SetHeroHandLampAsLit(true);
        if (resetThemeAtEnd == true)
            pDSTGame->ResetToDefaultTheme(1.0f);
        pDSTGame->EnableSounds(true);
        if (g_CutsceneMusicOverridden == true) {
            pDSTGame->StopOverrideMusic(true);
            g_CutsceneMusicOverridden = false;
        }
        if (screenFilter)
            pDSTGame->ScreenFilterFadeOut(0.0f, screenFilter);
        pDSTGame->SetAllowScreenFadingIfAlreadyFaded(false);
        if (questName != "NULL")
            pDSTGame->DeactivateQuest(questName, 0);
    }
}