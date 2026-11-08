// 00cbfb7d RunCutsceneMacro
// Grade: DIFFER(74906v74778), ordinary MSVC 7.1 RTM; exact 0x18DC frame. Partial checkpoint, no behavior test or complete symbol audit.
// Next: entry register roles, remaining slot order, prefix conditional cleanup, SlideTeleport x87, CrowdKill loop and shared tails. See WORKLOG_CURRENT.md and PROBES_CURRENT.md.
// Flags: /d2inlS8
// Inline-budget: /d2inlS8 + per-site devices -- VC7.1 caps inlining by the caller's whole IL size, greedy in source order; S8 reproduces retail's 524 GetLength / 41 vector<CScriptThing>::operator[] out-of-line calls; `#pragma inline_depth(0)` regions outline the sites retail calls (iterator == 0x00CC8407, vector ctors 0x00CD0DA4 / 0x00CD0E20 / 0x00CD0F3B, vector<CCharString>::operator[] x7 0x00CD147E..0x00CD17B0); `__forceinline` twins expand the sites retail expands (GetLengthExpanded 0x00CC0040, StartsWithExpanded 0x00CC03EA, IsEnd 0x00CC61B8 / 0x00CC6DA9); tools/calls.py --counts: no inline-site difference left.
#pragma optimize("s", on)
// Leaf: ?RunCutsceneMacro@@YIXABVCCharString@@AAV?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@4@@std@@PAV?$map@VCCharString@@_NU?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@_N@std@@@3@@3@PAV?$map@VCCharString@@V1@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@V1@@std@@@3@@3@_N4@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBFB7D, 74,778 bytes, `__fastcall` global (name in ecx, the actor map in edx,
//   four stack arguments: `ret 10h`), frame-pointer size TU (`lea ebp,[esp-68h]`, 0x18DC-byte frame through __chkstk).
// Name: Ego_r.pdb and FableWin.pdb ?RunCutsceneMacro@@YIX... (the Leaf above; both PDBs, same signature) + FSE FableAPI.cpp:183
//   tRunCutsceneMacro at 0xCBFB7D (ghidra_out/labels_fse_addresses.tsv); engine/fable_script_cutscene.h declares it.
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define FABLE_RETAIL_CRGBCOLOUR_CTOR_FORCEINLINE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_BODY
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQUALSCI
#define FABLE_RETAIL_CCHARSTRING_WHITESPACE
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_APPEND_CHAR
#define FABLE_RETAIL_CCHARSTRING_APPEND_COUNT
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CCHARSTRING_GETLENGTH_EXPANDED
#include "fable_string.h"
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
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_BYNAME_BODY
#include "engine/CDefinitionManager.h"
#include "engine/fable_script_cutscene.h"
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CASuspendableProcess.h"
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

// CCutsceneDef (Ego_r.pdb: CParentDefClassBase + eight vector<CCharString>; retail offsets with 12-byte STLport vectors).
class CCutsceneDef : public CParentDefClassBase {
public:
    std::vector<CCharString> Macro;                 // +0x3c
    std::vector<CCharString> SkipCond;              // +0x48
    std::vector<CCharString> Lights;                // +0x54
    std::vector<CCharString> LightScene;            // +0x60
    std::vector<CCharString> SetupCond;             // +0x6c
    std::vector<CCharString> Sound;                 // +0x78
    std::vector<CCharString> Answer0;               // +0x84
    std::vector<CCharString> Answer1;               // +0x90
};

typedef std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> CCutsceneActorMap;

CDefinitionManager* GFGetPDefinitionManager();                                   // 0x00CD3EF0
extern CGameScriptInterfaceBase* pDSTGame;                                        // 0x0143E8F8
extern long g_CutsceneSoundFrame;                                                 // 0x013B83C8 (descriptive)
extern bool g_bSkipCutsceneWithActionButton;                                      // 0x0143E8F4 (landed spelling, 0x00CBEB7E)
extern bool g_CutsceneMusicOverridden;                                            // 0x0143E8FD (descriptive)
extern CASuspendableProcess* g_pActiveSuspendableProcess;                         // 0x013D2838 (descriptive)
// Same-TU helpers: retail compiles these before RunCutsceneMacro in the same file, so VC7.1 knows they only read through
// the references they get (a parameter string, the parameter vector) and keeps locals in registers across the calls.
#pragma auto_inline(off)
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
#pragma auto_inline(on)
#pragma auto_inline(off)
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
#pragma auto_inline(on)
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
#pragma auto_inline(off)
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
#pragma auto_inline(on)
#pragma auto_inline(off)
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
#pragma auto_inline(on)
//   // 0x00CBF050 (Ego_r.pdb + FableWin.pdb ?CutsceneMacro_ReadLightScene@@YIXAAVCCharString@@0PAE11@Z)
extern CWideString g_UnprovenWideString_0143E908;                                 // 0x0143E908 (landed spelling, 0x00E81B40)
// 0040dce0 reads the pointer, rejects null, then tests CThing::Dead at +0x91 bit 0.
// FableWin's exact instantiation name is ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z.
// Keep the observed call out of line while exposing its real read-only body.
template <class T>
#ifdef _MSC_VER
__declspec(noinline)
#endif
bool __fastcall IS_THING_ALIVE(const T& thing)
{
    if (!thing)
        return false;
    if (!thing->IsAlive())
        return false;
    return true;
}

// Inline-budget twin (line 5): `it == c.end()` with the body of STLport's _Rb_tree_base_iterator::operator== forced in place,
// the find call evaluated before end() as retail does (0x00CC61B8 `mov eax,[eax] ; cmp eax,[map header]`). A __forceinline
// wrapper that only calls the plain operator== is capped like the operator itself, so the twin carries the body.
#ifdef _MSC_VER
template <class Iterator, class Container> static __forceinline bool IsEnd(const Iterator& it, const Container& c) { return it._M_node == c.end()._M_node; }
#else
template <class Iterator, class Container> static inline bool IsEnd(const Iterator& it, const Container& c) { return it._M_node == c.end()._M_node; }
#endif

// One cutscene frame: run a script frame when enabled, play this frame's sound cue and move the cue counter on.
#define CUTSCENE_FRAME()                                        \
    if (doScriptFrames)                                         \
        pDSTGame->NewScriptFrame();                             \
    PlayCutsceneSound(sounds, g_CutsceneSoundFrame);            \
    if (g_CutsceneSoundFrame != -1)                             \
        ++g_CutsceneSoundFrame

void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneActorMap& actors, std::map<CCharString, bool>* flags,
                                 std::map<CCharString, CCharString>* substitutions, bool setup, bool keepPlayerControl)
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
    bool b234 = false;
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
        pDSTGame->SetCutsceneMode(true, keepPlayerControl);
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
                        if (cue.GetLengthExpanded() != 0)   // 0x00CC0040: expanded by retail
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
                    long param = 0;
                    bool quoted = false;
                    for (long c = space + 1; c < commands[line].GetLength(); ++c) {
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
                        if (params[k].GetLength() != 0 && params[k].StartsWithExpanded("$")) {   // 0x00CC03EA: prefix length expanded by retail
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
                            if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
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
                            if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
                                break;
                        }
                    }
                } else if (command.StartsWith(".WalkTo")) {
                    if (actor && params[0].GetLength()) {
                        float speed = 0.3f;
                        bool wait = false;
                        bool avoidObstacles = true;
                        bool b630 = false;
                        bool b6a4 = true;
                        if (params[1].GetLength())
                            speed = GFCharStringToFloat(params[1]);
                        if (IsTrue(params[3]))
                            wait = true;
                        if (IsFalse(params[4]))
                            avoidObstacles = false;
                        if (IsTrue(params[5]))
                            b630 = true;
                        if (IsFalse(params[6]))
                            b6a4 = false;
                        pDSTGame->EntitySetCutsceneBehaviour(actor->GetScriptThing(), (ECutsceneBehaviour)2);
                        actor->MoveToThing((CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]),
                                           speed, (EScriptEntityMoveType)0, false, avoidObstacles, b630, b6a4);
                        if (IsTrue(params[2]) || wait) {
                            while (actor->IsPerformingScriptTask()) {
                                if (IsCutsceneSkipped())
                                    break;
                                CUTSCENE_FRAME();
                                if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
                                    break;
                            }
                        } else {
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith(".RunTo")) {
                    if (actor && params[0].GetLength()) {
                        float speed = 0.3f;
                        bool wait = false;
                        bool avoidObstacles = true;
                        bool b650 = false;
                        bool b648 = true;
                        if (params[1].GetLength())
                            speed = GFCharStringToFloat(params[1]);
                        if (IsTrue(params[3]))
                            wait = true;
                        if (IsFalse(params[4]))
                            avoidObstacles = false;
                        if (IsTrue(params[5]))
                            b650 = true;
                        if (IsFalse(params[6]))
                            b648 = false;
                        pDSTGame->EntitySetCutsceneBehaviour(actor->GetScriptThing(), (ECutsceneBehaviour)2);
                        actor->MoveToThing((CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]),
                                           speed, (EScriptEntityMoveType)1, false, avoidObstacles, b650, b648);
                        if (IsTrue(params[2]) || wait) {
                            while (actor->IsPerformingScriptTask()) {
                                if (IsCutsceneSkipped())
                                    break;
                                CUTSCENE_FRAME();
                                if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
                                    break;
                            }
                        } else {
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith(".SneakTo")) {
                    if (actor && params[0].GetLength()) {
                        float speed = 0.3f;
                        bool wait = false;
                        bool avoidObstacles = true;
                        bool b670 = false;
                        bool b668 = true;
                        if (params[1].GetLength())
                            speed = GFCharStringToFloat(params[1]);
                        if (IsTrue(params[3]))
                            wait = true;
                        if (IsFalse(params[4]))
                            avoidObstacles = false;
                        if (IsTrue(params[5]))
                            b670 = true;
                        if (IsFalse(params[6]))
                            b668 = false;
                        pDSTGame->EntitySetCutsceneBehaviour(actor->GetScriptThing(), (ECutsceneBehaviour)2);
                        actor->MoveToThing((CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]),
                                           speed, (EScriptEntityMoveType)2, false, avoidObstacles, b670, b668);
                        if (IsTrue(params[2]) || wait) {
                            while (actor->IsPerformingScriptTask()) {
                                if (IsCutsceneSkipped())
                                    break;
                                CUTSCENE_FRAME();
                                if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
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
                        CScriptThing thing = actor->GetScriptThing();
                        bool drunk = true;
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
                        bool b6d0 = false;
                        bool b6c8 = false;
                        bool b6c0 = false;
                        bool b6b8 = true;
                        bool b6b0 = false;
                        if (IsTrue(params[1]))
                            b6d0 = true;
                        if (IsTrue(params[2]))
                            b6c8 = true;
                        if (IsTrue(params[3]))
                            b6c0 = true;
                        if (IsFalse(params[4]))
                            b6b8 = false;
                        if (IsTrue(params[5]))
                            b6b0 = true;
                        actor->PlayAnimation(params[0], b6d0, b6c8, b6c0, b6b8, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, b6b0);
                        if (frameAfterAnim) {
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith(".PlayCombatAnim")) {
                    if (actor && params[0].GetLength()) {
                        long repeat = 1;
                        bool b6f0 = false;
                        bool b6e8 = false;
                        bool b6e0 = true;
                        bool b6f8 = true;
                        bool b6d8 = false;
                        bool unusedFlag = false;
                        if (IsTrue(params[1]))
                            unusedFlag = true;
                        if (IsTrue(params[2]))
                            b6f0 = true;
                        if (IsTrue(params[3]))
                            b6e8 = true;
                        if (IsFalse(params[4]))
                            b6e0 = false;
                        if (IsFalse(params[5]))
                            b6f8 = false;
                        if (params[6].GetLength())
                            repeat = GFCharStringToInt(params[6]);
                        if (IsTrue(params[7]))
                            b6d8 = true;
                        for (long i = 0; i < repeat; ++i)
                            actor->PlayCombatAnimation(params[0], b6f8, b6f0, b6e8, b6e0, false, b6d8);
                        if (frameAfterAnim) {
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith(".PlayLoopingAnim")) {
                    if (actor && params[0].GetLength() && params[1].GetLength()) {
                        bool b57c = false;
                        bool b7c8 = false;
                        bool b710 = false;
                        bool b708 = true;
                        bool b700 = false;
                        if (IsTrue(params[2]))
                            b57c = true;
                        if (IsTrue(params[3]))
                            b7c8 = true;
                        if (IsTrue(params[4]))
                            b710 = true;
                        if (IsFalse(params[5]))
                            b708 = false;
                        if (IsTrue(params[6]))
                            b700 = true;
                        long loops = GFCharStringToInt(params[1]);
                        actor->PlayLoopingAnimation(params[0], loops, b57c, b7c8, b710, b708,
                                                    NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, b700);
                        if (frameAfterAnim) {
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith(".WaitPlayAnimation")) {
                    if (actor && params[0].GetLength()) {
                        bool b3f0 = false;
                        bool b168 = false;
                        bool b3e8 = true;
                        if (IsTrue(params[1]))
                            b3f0 = true;
                        if (IsTrue(params[2]))
                            b168 = true;
                        if (IsFalse(params[3]))
                            b3e8 = false;
                        if (IsTrue(params[4]))
                            actor->PlayCombatAnimation(params[0], true, b3f0, b168, b3e8, false, false);
                        else
                            actor->PlayAnimation(params[0], false, b3f0, b168, b3e8, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
                        while (actor->IsPerformingScriptTask() == true) {
                            if (IsCutsceneSkipped())
                                break;
                            CUTSCENE_FRAME();
                            if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
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
                        bool b9c0 = IsTrue(params[1]);
                        CScriptThing thing = actor->GetScriptThing();
                        pDSTGame->GiveThingItemInHand(thing, params[0], b9c0);
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
                            if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
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
                            actor->Speak(target->GetScriptThing(), params[1], method, false, true, b234);
                        else
                            actor->Speak((CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]), params[1], method, false, true, b234);
                        while (actor->IsPerformingScriptTask()) {
                            if (IsCutsceneSkipped())
                                break;
                            CUTSCENE_FRAME();
                            if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
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
                            if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
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
                                if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
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
                        bool b794 = true;
                        bool frame = true;
                        if (IsFalse(params[1]))
                            b794 = false;
                        if (IsFalse(params[2]))
                            frame = false;
                        pDSTGame->EntitySetFacingAngleTowardsThing(actor->GetScriptThing(), target, b794);
                        if (frame) {
                            CUTSCENE_FRAME();
                        }
                    }
                } else if (command.StartsWith(".LookInDirection")) {
                    if (actor && params[0].GetLength()) {
                        float angle = GFCharStringToFloat(params[0]) * (1.0f / 360.0f);
                        bool b5cc = true;
                        if (IsFalse(params[1]))
                            b5cc = false;
                        pDSTGame->EntitySetFacingAngle(actor->GetScriptThing(), angle, b5cc);
                    }
                } else if (command.StartsWith(".WaitForUnderRadius")) {
                    if (actor && params[0].GetLength() && params[1].GetLength()) {
                        float radius = GFCharStringToFloat(params[1]);
                        while (!IsDistanceBetweenThingsUnder(actor->GetScriptThing(), (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]), radius) && !IsCutsceneSkipped()) {
                            CUTSCENE_FRAME();
                            if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
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
                                if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
                                    break;
                            }
                        }
                    }
                } else if (command.StartsWith(".FollowNavRoute")) {
                    if (actor && params[0].GetLength()) {
                        long moveType = 0;
                        bool b7d4 = false;
                        if (params[1].EqualsCaseInsensitive("run"))
                            moveType = 1;
                        if (params[1].EqualsCaseInsensitive("sneak"))
                            moveType = 2;
                        if (IsTrue(params[2]))
                            b7d4 = true;
                        actor->FollowPreCalculatedRoute((CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]), (EScriptEntityMoveType)moveType, b7d4, false);
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
                        bool b5e4 = true;
                        if (IsFalse(params[1]))
                            b5e4 = false;
                        if (thing.IsAlive() && target.IsAlive())
                            pDSTGame->EntityTeleportToPosition(thing, target.GetPos(), target.GetAngleXY(), b5e4, false);
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
                        if (params[2].GetLength())
                            steps = GFCharStringToInt(params[2]);
                        C3DVector start = from.GetPos();
                        C3DVector end = to.GetPos();
                        float dx = end.X - start.X;
                        C3DVector step(dx / steps, (end.Y - start.Y) / steps, (end.Z - start.Z) / steps);
                        float angle = atan((end.Y - start.Y) / dx) * 0.15915494f;
                        float angleStep = 0.5 / steps;
                        if (IsFalse(params[4]))
                            angleStep = 0.0f;
                        for (; i <= steps; ++i) {
                            if (IsCutsceneSkipped())
                                break;
                            float fraction = (float)i;
                            C3DVector position(start);
                            position += step * fraction;
                            pDSTGame->EntityTeleportToPosition(thing, position, angle + angleStep * fraction, false, false);
                            if (IsTrue(params[3])) {
                                CUTSCENE_FRAME();
                            }
                            if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
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
                        if (params[3].GetLength())
                            steps = GFCharStringToInt(params[3]);
                        C3DVector start = from.GetPos();
                        C3DVector end = to.GetPos();
                        float dx = end.X - start.X;
                        C3DVector step(dx / steps, (end.Y - start.Y) / steps, (end.Z - start.Z) / steps);
                        float angle = atan((end.Y - start.Y) / dx) * 0.15915494f;
                        float angleStep = 0.5 / steps;
                        if (IsFalse(params[4])) {
                            angle = from.GetAngleXY();
                            angleStep = 0.0f;
                        }
                        for (; i <= steps; ++i) {
                            if (IsCutsceneSkipped())
                                break;
                            float fraction = (float)i;
                            C3DVector position(start);
                            position += step * fraction;
                            pDSTGame->EntityTeleportToPosition(thing, position, angle + angleStep * fraction, false, false);
                            CUTSCENE_FRAME();
                            if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
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
                    b234 = true;
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
                            if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
                                break;
                        }
                        answer = (result != 0);
                    }
                } else if (command.StartsWith("GiveHeroExpression")) {
                    if (params[0].GetLength()) {
                        CCharString expression(params[0]);
                        if (IsEnd(localFlags.find(expression), localFlags)) {   // 0x00CC61B8: == expanded by retail
                            long level = -1;
                            bool b5f4 = false;
                            if (IsTrue(params[1]))
                                b5f4 = true;
                            if (params[2].GetLength())
                                level = GFCharStringToInt(params[2]);
                            pDSTGame->GiveHeroExpression(expression, level, b5f4);
                        }
                    }
                } else if (command.StartsWith("GiveHeroMorality")) {
                    if (params[0].GetLength()) {
                        float morality = GFCharStringToFloat(params[0]);
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
                        long b_c = -1;
                        bool b7f = false;
                        bool b25c = false;
                        bool b13 = true;
                        if (params[1].GetLength())
                            count = GFCharStringToInt(params[1]);
                        if (params[2].GetLength())
                            b_c = GFCharStringToInt(params[2]);
                        if (IsTrue(params[3]))
                            b25c = true;
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
                                pDSTGame->GiveHeroObject(object, b_c, b25c);
                            if (it == givenObjects.end())
                                givenObjects[object] = count;
                            else
                                givenObjects[object] += count;
                            if (b7f) {
                                if (!b25c) {
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
                            if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
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
                                if (IsEnd(registeredActors.find(extraName), registeredActors) && extra.IsAlive()) {   // 0x00CC6DA9: == expanded by retail
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
                        bool b6f4 = false;
                        if (IsTrue(params[2]))
                            b6f4 = true;
                        CScriptThing thing = pDSTGame->GetThingWithScriptName(params[0]);
                        pDSTGame->EntitySetCutsceneBehaviour(thing, (ECutsceneBehaviour)2);
                        pDSTGame->EntityPlayObjectAnimation(thing, params[1], b6f4);
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
                        bool b7b0 = true;
                        if (IsFalse(params[2]))
                            b7b0 = false;
                        if (thing.IsAlive() && target.IsAlive())
                            pDSTGame->EntityTeleportToPosition(thing, target.GetPos(), target.GetAngleXY(), b7b0, false);
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
                        std::map<CCharString, long>::iterator it;
                        it = givenObjects.find("Gold");
                        if (it != givenObjects.end())
                            gold -= givenObjects["Gold"];
                        if (gold > 0) {
                            pDSTGame->GiveHeroGold(gold);
                            std::map<CCharString, long>::iterator last = givenObjects.end();
#pragma inline_depth(0)   // 0x00CC8407: retail calls the iterator == (0x0041C820) out of line here
                            if (it == last)
#pragma inline_depth()
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
                        float maxFrames = GFCharStringToFloat(params[2]) * 15.0f;
                        float frames = 0.0f;
                        while (!IsCutsceneSkipped()) {
                            if (pDSTGame->EntityGetBossPhase(boss) != phase || !(frames <= maxFrames))
                                break;
                            CUTSCENE_FRAME();
                            frames += 1.0f;
                            if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
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
                        if (params[1].EqualsCaseInsensitive("clock")) {
                            double end = GFGetTime() + seconds;
                            while (GFGetTime() < end) {
                                if (IsCutsceneSkipped())
                                    break;
                                CUTSCENE_FRAME();
                                if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
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
                        if (musicType != -1)
                            pDSTGame->CacheMusicSet((EMusicSetType)musicType);
                    }
                } else if (command.StartsWith("PlayMusic")) {
                    if (params[0].GetLength()) {
                        long musicType = g_pMusicTypeSymbolMap->GetSymbolSLONG(params[0]);
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
                        bool b63c = true;
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
                            b63c = false;
                        float distance = GFCharStringToFloat(params[6]);
                        if (marker0.IsAlive() && marker1.IsAlive() && marker2.IsAlive() && marker3.IsAlive() && from.IsAlive() && to.IsAlive()) {
                            CScriptThing best = GetBestThingFromMarkerList(markers, from, to, b63c);
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
                    if (params[0].GetLength())
                        pDSTGame->EntityTeleportToThing(pDSTGame->GetHero(), (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]), false);
                } else if (command.StartsWith("WaitForCamera")) {
                    while (pDSTGame->IsCameraInScriptedMode()) {
                        if (pDSTGame->IsActiveThreadTerminating())
                            break;
                        if (IsCutsceneSkipped())
                            break;
                        CUTSCENE_FRAME();
                        if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
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
                        if (target && target->IsValid())
                            pDSTGame->CameraMoveBetweenLookingAt(C3DVector(fromX, fromY, fromZ) + from.GetPos(), C3DVector(toX, toY, toZ) + to.GetPos(),
                                                                 target->GetScriptThing(), time, -1.0f);
                        else
                            pDSTGame->CameraMoveBetweenLookingAt(C3DVector(fromX, fromY, fromZ) + from.GetPos(), C3DVector(toX, toY, toZ) + to.GetPos(),
                                                                 (CCharString("HERO") == params[2]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[2]), time, -1.0f);
                        CUTSCENE_FRAME();
                    }
                } else if (command.StartsWith("CameraPath")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength() && params[3].GetLength() && params[4].GetLength()) {
                        CScriptThing from = CSFindThing(params[0], actors);
                        CScriptThing fromLookAt = CSFindThing(params[1], actors);
                        CScriptThing to = CSFindThing(params[2], actors);
                        CScriptThing toLookAt = CSFindThing(params[3], actors);
                        float time = GFCharStringToFloat(params[4]);
                        pDSTGame->CameraMoveBetweenLookFromAndLookTo(from.GetPos(), to.GetPos(), fromLookAt.GetPos(), toLookAt.GetPos(), time);
                    }
                } else if (command.StartsWith("CameraFOVLookBetweenPos")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength() && params[3].GetLength()) {
                        CScriptThing from = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        CScriptThing to = (CCharString("HERO") == params[1]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[1]);
                        float time = GFCharStringToFloat(params[3]);
                        float offsetX = 0.0f;
                        float offsetY = 0.0f;
                        float offsetZ = 0.0f;
                        if (params[4].GetLength())
                            offsetX = GFCharStringToFloat(params[4]);
                        if (params[5].GetLength())
                            offsetY = GFCharStringToFloat(params[5]);
                        if (params[6].GetLength())
                            offsetZ = GFCharStringToFloat(params[6]);
                        CScriptGameResourceObjectScriptedThingBase* target = 0;
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
                        lookAt.X += offsetX;
                        lookAt.Y += offsetY;
                        lookAt.Z += offsetZ;
                        pDSTGame->CameraMoveBetweenLookingAt(from.GetPos(), to.GetPos(), lookAt, time, fov);
                        CUTSCENE_FRAME();
                    }
                } else if (command.StartsWith("CameraFOVLookBetween")) {
                    if (params[0].GetLength() && params[1].GetLength() && params[2].GetLength() && params[3].GetLength()) {
                        CScriptThing from = (CCharString("HERO") == params[0]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[0]);
                        CScriptThing to = (CCharString("HERO") == params[1]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[1]);
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
                        CScriptGameResourceObjectScriptedThingBase* target = 0;
                        CCutsceneActorMap::iterator it;
                        it = actors.find(params[2]);
                        if (it != actors.end() && (*it).second.IsValid())
                            target = &(*it).second;
                        float fov = -1.0f;
                        if (params[4].GetLength())
                            fov = GFCharStringToFloat(params[4]) * (1.0f / 360.0f);
                        if (target && target->IsValid())
                            pDSTGame->CameraMoveBetweenLookingAt(from.GetPos(), to.GetPos(), target->GetScriptThing(), time, fov);
                        else
                            pDSTGame->CameraMoveBetweenLookingAt(from.GetPos(), to.GetPos(),
                                                                 (CCharString("HERO") == params[2]) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(params[2]), time, fov);
                        CUTSCENE_FRAME();
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
                            if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
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
                        crowdName = params[1].GetLength() ? params[1] : searchName;
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
                        CScriptThing thing;
                        bool found = false;
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
                            float interval = GFCharStringToFloat(params[3]) * 15.0f;
                            float waited = 0.0f;
                            float timer = 0.0f;
                            if (IsFalse(params[4])) {
                                for (long i = 0; i < count; ++i) {
                                    markerName = params[1] + GFIntToCharString(i);
                                    marker = pDSTGame->GetThingWithScriptName(markerName);
                                    memberName = params[0] + GFIntToCharString(i);
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
                            float speed = 1.0f;
                            if (params[2].GetLength())
                                speed = GFCharStringToFloat(params[2]);
                            EScriptEntityMoveType moveType;
                            if (params[3] == "SNEAK")
                                moveType = (EScriptEntityMoveType)2;
                            else if (params[3] == "RUN")
                                moveType = (EScriptEntityMoveType)1;
                            else
                                moveType = (EScriptEntityMoveType)0;
                            for (long i = 0; i < count; ++i) {
                                CCharString memberName = params[0] + GFIntToCharString(i);
                                member = actors[memberName];
                                if (member.IsValid())
                                    member.MoveToThing(target, speed, moveType, false, true, false, true);
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
                            long loops = -1;
                            bool stayOnLastFrame = false;
                            bool looping = false;
                            if (IsTrue(params[6]))
                                looping = true;
                            if (params[7].GetLength())
                                loops = GFCharStringToInt(params[7]);
                            if (IsTrue(params[8]))
                                stayOnLastFrame = true;
                            if (duration == 0.0f) {
                                bool lookAt = false;
                                if (crowdLooking)
                                    lookAt = true;
                                for (long i = 0; i < count; ++i) {
                                    CCharString memberName = params[0] + GFIntToCharString(i);
                                    member = actors[memberName];
                                    if (member.IsValid()) {
                                        if (looping)
                                            member.PlayLoopingAnimation(anim, loops, false, stayOnLastFrame, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, lookAt);
                                        else
                                            member.PlayAnimation(anim, false, stayOnLastFrame, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, lookAt);
                                    }
                                }
                            } else {
                                float totalTime = GFCharStringToFloat(params[2]);
                                long randomRange = 0;
                                std::vector<CScriptGameResourceObjectScriptedThingBase> members;
                                for (long i = 0; i < count; ++i) {
                                    CCharString memberName = params[0] + GFIntToCharString(i);
                                    member = actors[memberName];
                                    if (member.IsValid())
                                        members.push_back(member);
                                }
                                long memberCount = members.size();
                                float random = 0.0f;
                                long played = 1;
                                float time = 0.0f;
                                long index = 0;
                                bool sequential = false;
                                long frame = 0;
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
                                                member.PlayLoopingAnimation(anim, loops, false, stayOnLastFrame, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
                                            else
                                                member.PlayAnimation(anim, false, stayOnLastFrame, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
                                        }
                                        ++played;
                                        if (randomRange > 0)
                                            random = (rand() % randomRange) * 0.01f;
                                        long frames = (long)((duration + random + time) * 15.0f);
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
                            bool b580 = false;
                            bool b578 = false;
                            bool b7bc = true;
                            if (IsTrue(params[5]))
                                b580 = true;
                            if (IsTrue(params[6]))
                                b578 = true;
                            if (IsFalse(params[7]))
                                b7bc = false;
                            if (duration == 0.0f) {
                                CScriptGameResourceObjectScriptedThingBase member;
                                long count = crowdSizes[params[0]];
                                for (long i = 0; i < count; ++i) {
                                    CCharString memberName = params[0] + GFIntToCharString(i);
                                    member = actors[memberName];
                                    if (member.IsValid())
                                        member.PlayCombatAnimation(anim, true, b580, b578, b7bc, false, false);
                                }
                            } else {
                                float totalTime = GFCharStringToFloat(params[2]);
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
                        if (delay > 0.0f) {
                            while (timer < delay && !IsCutsceneSkipped()) {
                                timer += 1.0f;
                                CUTSCENE_FRAME();
                            }
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
                            if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
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
                            CScriptThing thing = CSFindThing(params[0], actors);
                            bool withFade = false;
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
                        if (g_UnprovenWideString_0143E908 != L"")
                            pDSTGame->DisplayGameInfoText(g_UnprovenWideString_0143E908);
                        else
                            pDSTGame->DisplayGameInfo(params[0]);
                        while (!pDSTGame->MsgIsGameInfoClickedPast()) {
                            if (IsCutsceneSkipped())
                                break;
                            CUTSCENE_FRAME();
                            if (g_pActiveSuspendableProcess && g_pActiveSuspendableProcess->StopRunning)
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
                        std::allocator<CCharString> stringAllocator;
#pragma inline_depth(0)   // 0x00CD0DA4: retail calls this vector ctor (0x00414DE0) out of line
                        std::vector<CCharString> values(stringAllocator);
#pragma inline_depth()
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
                        std::allocator<CScreenFilterSThingByPass> bypassAllocator;
#pragma inline_depth(0)   // 0x00CD0E20: retail calls this vector ctor (0x00CD2D28) out of line
                        std::vector<CScreenFilterSThingByPass> bypass(bypassAllocator);
#pragma inline_depth()
                        if (CSExtractParameterList(params[6], values) > 0) {
                            unsigned long count = values.size();
                            for (unsigned long i = 0; i < count; ++i) {
                                CCharString& value = values[i];
                                if (value.StartsWith("ALL:") || value.StartsWith("ALLDEF:")) {
                                    CCharString name = value.GetSubString(value.FindCharPos(':') + 1, value.GetLength());
                                    std::allocator<CScriptThing> thingAllocator;
#pragma inline_depth(0)   // 0x00CD0F3B: retail calls this vector ctor (0x008AC5A0) out of line
                                    std::vector<CScriptThing> things(thingAllocator);
#pragma inline_depth()
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
                        pDSTGame->ScreenFilterFadeOut(GFCharStringToFloat(params[0]), screenFilter);
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
#pragma inline_depth(0)   // 0x00CD147E: retail calls vector<CCharString>::operator[] (0x0040F110) out of line
                                CCharString lightLine(lights[i]);
#pragma inline_depth()
                                unsigned char red;
                                unsigned char green;
                                unsigned char blue;
                                CutsceneMacro_ReadLightScene(lightLine, lightName, &red, &green, &blue);
                                CScriptThing light = (CCharString("HERO") == lightName) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(lightName);
                                if (light.IsAlive())
                                    pDSTGame->SetLightColour(light, CRGBColour(0, 0, 0, 255));
                            }
                            CCharString value;
#pragma inline_depth(0)   // 0x00CD15D5..0x00CD17B0: retail calls vector<CCharString>::operator[] (0x0040F110) out of line in the scene loop
                            for (long j = 0; j < scenes[scene].GetLength(); ++j) {
                                if (((const char*)scenes[scene])[j] == ',') {
#pragma inline_depth()
                                    long index = GFCharStringToInt(value);
                                    CCharString lightName;
#pragma inline_depth(0)   // 0x00CD1621: operator[] out of line
                                    CCharString lightLine(lights[index]);
#pragma inline_depth()
                                    unsigned char red;
                                    unsigned char green;
                                    unsigned char blue;
                                    CutsceneMacro_ReadLightScene(lightLine, lightName, &red, &green, &blue);
                                    CScriptThing light = (CCharString("HERO") == lightName) ? pDSTGame->GetHero() : pDSTGame->GetThingWithScriptName(lightName);
                                    if (light.IsAlive())
                                        pDSTGame->SetLightColour(light, CRGBColour(red, green, blue, 255));
                                    value = "";
#pragma inline_depth(0)
                                } else if (((const char*)scenes[scene])[j] != ' ') {
                                    value.Append(((const char*)scenes[scene])[j], value.GetLength());
                                }
                            }
#pragma inline_depth()
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
