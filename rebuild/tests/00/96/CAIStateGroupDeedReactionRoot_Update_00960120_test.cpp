// Real-object behaviour test for 0x00960120 CAIStateGroup_DeedReactionRoot::Update (links the REAL row object: vland's fallback link of
// the source object with this test). Every callee of the row is a stand-in bound by `/alternatename` to its PDB symbol; thiscall members
// are __fastcall stand-ins (ecx = this, edx unused, the same stack cleanup; a by-value CIntelligentPointer is its 8 bytes on the stack).
// The TU's own COMDATs stay real: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound (the TC map search, start +0 / finish +4) and the
// CIntelligentPointer<const CThing> copy constructor / destructor (over the CBaseIntelligentPointer stand-ins).
// Fixtures use the retail layouts the row reads: state group PBrain +0x04 (brain creature +0x18, brain vtable slot 2 =
// SuspendSuspendableProcess), ThingReactingTo +0x14 (vptr, PItem), CurrentDeedType +0x1C, DeedIsVirtual +0x24, DeliveredReaction +0x34,
// DeliveredSpeech +0x35; thing TC flag words +0x20 (bit 15 TCI_TALK 0x0F), +0x28 (bit 15 opinion-of-hero 0x4F, bit 16 opinion source
// 0x50, bit 17 hero deed log 0x51), +0x38 (bit 23 wife 0xD7), the TC vector map +0x44 (end +0x48), Dead +0x91 bit 0, current action
// +0x100 (InterruptionGroupID +0x20, vtable slot 27 IsActionReadyForInterruption); world +0x74 = the reaction manager (AttitudeMask
// +0x554, DeedReactionSpeechPlaying +0x5A4); CMainGameComponent +0x1C = the player manager; deed log DeedReactions +0xE8 (92 x
// {CCharString Animations[18]; ulong DialogueTags[18]}); effects def refcount +0x04, ReactionDialogueLayer +0x60, IsFlirting +0x7C bit 6,
// AllowSpeechOnNonPureAISpeaker +0x7D bit 0 (CDefPointer released inline). Each scenario's event trace, with the arguments of every call
// (attitudes, deed types, dialogue tags, the LookAt frames and speed bits, the PlayAISpeech and PlayAnimation arguments), is compared
// with a reference model of the reaction; the delivered flags and the def reference counts are checked after each run.
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#define ALT(a, b) comment(linker, "/alternatename:" a "=" b)
#pragma ALT("@CallUpdate@8", "?Update@CAIStateGroup_DeedReactionRoot@@UAEXXZ")
#pragma ALT("??1CBaseIntelligentPointer@@UAE@XZ", "@IPDtor@8")
#pragma ALT("??0CBaseIntelligentPointer@@QAE@XZ", "@IPCtor@8")
#pragma ALT("??0CCreatureAction_PlayAnimation@@QAE@AAVCThingCreatureBase@@ABVCCharString@@ABVCSoundPair@@_N33JJ@Z", "@PlayAnimCtor@40")
#pragma ALT("??1CCreatureActionBase@@UAE@XZ", "@ActionDtor@8")
#pragma ALT("??8?$CBasicString@D@@QBE_NPBD@Z", "@BsEq@12")
#pragma ALT("??9CCharString@@QBE_NPBD@Z", "@StrNe@12")
#pragma ALT("?AIAction_DoNothing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ", "@DoNothing@8")
#pragma ALT("?AIAction_LookAtThing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@$$CBVCThing@@@@JM@Z", "@LookAt@28")
#pragma ALT("?AIAction_PerformActionTargetFaceAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@V?$CIntelligentPointer@$$CBVCThing@@@@_N@Z", "@Perform@24")
#pragma ALT("?GetAttitudeOrReactionType@COpinionReactionManager@@ABE?AW4EOpinionReactionType@@ABVCThing@@_NPB_NPBVCTCOpinionSource@@@Z", "@AttOrReact@24")
#pragma ALT("?GetBlurredAttitude@COpinionReactionManager@@QBE?AW4EOpinionAttitudeType@@W42@@Z", "@Blur@12")
#pragma ALT("?GetCreatureType@CThingCreatureBase@@QBE?AW4ECreatureType@@XZ", "@GetCType@8")
#pragma ALT("?GetGreetDialogueGroupIndex@CTCCreatureOpinionOfHero@@QAEKW4EOpinionAttitudeType@@@Z", "@Greet@12")
#pragma ALT("?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ", "@MainPlayer@8")
#pragma ALT("?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ", "@Controlled@8")
#pragma ALT("?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ", "@IPGet@8")
#pragma ALT("?GetSex@CThingCreatureBase@@QBE?AW4ESex@@XZ", "@GetSex@8")
#pragma ALT("?GetThingAttitudeToPlayer@COpinionReactionManager@@QBE?AW4EOpinionAttitudeType@@ABVCThing@@@Z", "@AttPlayer@12")
#pragma ALT("?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ", "@GetWorld@8")
#pragma ALT("?IsCourtableAndNotBlockingIt@CTCWife@@QBE_NXZ", "@Courtable@8")
#pragma ALT("?IsMarriedToThePlayer@CTCWife@@QBE_NXZ", "@Married@8")
#pragma ALT("?IsStopping@CAIStateGroupBase@@IBE_NXZ", "@IsStopping@8")
#pragma ALT("?IsTalking@CTCTalk@@QBE_NXZ", "@IsTalking@8")
#pragma ALT("?LogDeedReactionPlayingSpeech@COpinionReactionManager@@QAEXW4EOpinionDeedType@@_N@Z", "@Log@16")
#pragma ALT("?PMainGameComponent@CMainGameComponent@@1PAV1@A", "_g_mgc")
#pragma ALT("?PeekPDeedEffectsDef@CTCHeroOpinionDeedLog@@QBE?AV?$CDefPointer@$$CBVCOpinionDeedEffectsDef@@@@W4EOpinionDeedType@@@Z", "@PeekDef@16")
#pragma ALT("?PlayAISpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@K_NW4EDialogueLayer@NSpeechGainManager@@00@Z", "@Speech@28")
#pragma ALT("?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z", "@IPSet@12")
#pragma ALT("?Terminate@CAIStateGroupBase@@QAEXXZ", "@Terminate@8")

static int g_fail = 0, g_scn = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (scenario %d)\n", line, g_scn); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)

// ---- fixtures (retail layouts) ----
struct IP { void* vtbl; void* item; };
struct Pair { long key; void* tc; };
struct Map { void* first; Pair* end; };
struct Action { void** vtbl; unsigned char p04[0x1C]; long group; };
typedef char ActionCheck[(size_t)&((Action*)0)->group == 0x20 ? 1 : -1];
struct Thing {
    unsigned char p00[0x20]; unsigned long f20; unsigned char p24[4]; unsigned long f28; unsigned char p2c[0x38 - 0x2C]; unsigned long f38;
    unsigned char p3c[0x44 - 0x3C]; Map map; unsigned char p4c[0x91 - 0x4C]; unsigned char dead; unsigned char p92[0x100 - 0x92]; Action* cur;
    Pair pairs[8]; int npairs;
};
typedef char ThingCheck[(size_t)&((Thing*)0)->map == 0x44 && (size_t)&((Thing*)0)->dead == 0x91 && (size_t)&((Thing*)0)->cur == 0x100 &&
                        (size_t)&((Thing*)0)->f38 == 0x38 && (size_t)&((Thing*)0)->f28 == 0x28 && (size_t)&((Thing*)0)->f20 == 0x20 ? 1 : -1];
struct Brain { void** vtbl; unsigned char p04[0x14]; Thing* creature; };
typedef char BrainCheck[(size_t)&((Brain*)0)->creature == 0x18 ? 1 : -1];
struct Root { void* vtbl; Brain* brain; unsigned char p08[0xC]; IP ip; long deed; long handle; bool virt; unsigned char p25[0xF];
              bool delivered; bool speech; unsigned char p36[2]; };
typedef char RootCheck[(size_t)&((Root*)0)->ip == 0x14 && (size_t)&((Root*)0)->deed == 0x1C && (size_t)&((Root*)0)->virt == 0x24 &&
                       (size_t)&((Root*)0)->delivered == 0x34 && (size_t)&((Root*)0)->speech == 0x35 ? 1 : -1];
struct World { unsigned char p00[0x74]; void* mgr; };
struct Mgc { unsigned char p00[0x1C]; void* pm; };
struct Def { void** vtbl; long ref; unsigned char p08[0x58]; long layer; unsigned char p64[0x18]; unsigned char f7c, f7d; unsigned char p7e[2]; };
typedef char DefCheck[(size_t)&((Def*)0)->layer == 0x60 && (size_t)&((Def*)0)->f7c == 0x7C ? 1 : -1];
struct Str { const char* text; long len; };           // CCharStringData: the CBasicString<char> (StringLength +4) at +0

extern "C" { Mgc* g_mgc; }
static void* g_ipvtbl[4]; static void* g_tuVtbl;      // the TU's own CIntelligentPointer<const CThing> vtable (a COMDAT of the row object)
static Root g_root; static Brain g_brain; static void* g_brainVtbl[4]; static Thing g_thing, g_creature, g_player; static World g_world;
static unsigned long g_mgrBuf[0x728 / 4]; static Mgc g_mgcObj; static int g_pm, g_playerObj; static unsigned long g_logBuf[0x34A8 / 4];
static Def g_def; static void* g_defVtbl[4]; static Action g_action; static void* g_actionVtbl[32];
static int g_srcTC, g_opinionTC, g_wifeTC, g_talkTC; static unsigned char g_anim[0xB4];
static Str g_strEmpty = { "", 0 }, g_strWave = { "Wave", 4 }, g_strShrug = { "Shrug", 5 }, g_strNod = { "Nod", 3 };

enum { DEED_TOO_FREQUENT = 54, DEED_INCOMPREHENSION = 73, DEED_INDOORS_GREETING = 77 };
static unsigned long* Tag(int deed, int att) { return (unsigned long*)((char*)g_logBuf + 0x130 + (deed * 36 + att) * 4); }
static Str** Anim(int deed, int att) { return (Str**)((char*)g_logBuf + 0xE8 + (deed * 36 + att) * 4); }
static long* Playing(int deed) { return (long*)((char*)g_mgrBuf + 0x5A4 + deed * 4); }

// scenario controls
static struct {
    bool thingNull, thingDead, hasSource, virt, playerNull, logOnThing, logOnPlayer, flirting, hasWife, married, courtable, allow;
    int attSource, attPlayer, blurred, deed, sex, ctype, greetBase, layer;
    int actionGroup, readyAfter, dieInLoop, look, retries, finalResult, doNothingFailAt, deadAtAnim, perform, talking, stopAt;
} P;
static char g_trace[8192]; static int g_tn;
static void T(const char* f, ...) { va_list a; va_start(a, f); g_tn += vsprintf(g_trace + g_tn, f, a); va_end(a); }
static int g_suspends, g_retryLeft, g_doNothings, g_talkLeft, g_stops, g_animLive;

static void AddTC(Thing* t, long key, void* tc)
{
    int i = t->npairs;
    while (i > 0 && t->pairs[i - 1].key > key) { t->pairs[i] = t->pairs[i - 1]; --i; }
    t->pairs[i].key = key; t->pairs[i].tc = tc; ++t->npairs;
    t->pairs[t->npairs].key = 0x7FFFFFFF; t->pairs[t->npairs].tc = 0; t->map.first = &t->pairs[0]; t->map.end = &t->pairs[t->npairs];
}
static void* FindTC(const Thing* t, long key) { for (int i = 0; i < t->npairs; ++i) if (t->pairs[i].key == key) return t->pairs[i].tc; return 0; }
static unsigned FloatBits(float f) { unsigned u; memcpy(&u, &f, 4); return u; }

extern "C" {
void __fastcall CallUpdate(Root* r, int);
void __fastcall IPDtor(IP* p, int) { }
IP* __fastcall IPCtor(IP* p, int) { p->item = 0; return p; }
void* __fastcall IPGet(const IP* p, int) { return p->item; }
void __fastcall IPSet(IP* p, int, void* item) { p->item = item; }
void* __fastcall PlayAnimCtor(void* a, int, Thing* c, const Str** name, const long* sound, int b1, int b2, int b3, long l1, long l2)
{
    CHECK(a != 0 && sound[0] == 0 && sound[1] == 0);
    T("K(%s,%d,%d,%d,%d,%ld,%ld)", *name ? (*name)->text : "<null>", c == &g_creature, b1 & 0xFF, b2 & 0xFF, b3 & 0xFF, l1, l2);
    memcpy(g_anim, &a, sizeof a); ++g_animLive; return a;
}
void __fastcall ActionDtor(void* a, int) { void* b; memcpy(&b, g_anim, sizeof b); CHECK(a == b); T("k"); --g_animLive; }
bool __fastcall BsEq(const Str* s, int, const char* text) { T("e"); return strcmp(s->text, text) == 0; }
bool __fastcall StrNe(Str* const* s, int, const char* text) { T("n"); return !(*s == 0 ? *text == 0 : strcmp((*s)->text, text) == 0); }
int __fastcall DoNothing(Root* r, int) { CHECK(r == &g_root); T("N"); ++g_doNothings; return g_doNothings == P.doNothingFailAt ? 2 : 4; }
int __fastcall LookAt(Root* r, int, int repeat, IP ip, long frames, float speed)
{
    CHECK(r == &g_root && ip.vtbl != 0); g_tuVtbl = ip.vtbl;
    T("L(%d,%d,%ld,%08x)", repeat, ip.item == &g_thing, frames, FloatBits(speed)); return P.look;
}
int __fastcall Perform(Root* r, int, void* action, IP ip, int wait)
{
    void* b; memcpy(&b, g_anim, sizeof b);
    CHECK(r == &g_root && ip.vtbl == g_tuVtbl && g_animLive == 1);
    T("F(%d,%d,%d)", action == b, ip.item == &g_thing, wait & 0xFF); return P.perform;
}
int __fastcall AttOrReact(void* m, int, const Thing* t, int attitude, const bool* mask, const void* src)
{
    CHECK(m == g_mgrBuf && mask == (const bool*)((char*)g_mgrBuf + 0x554) && src == &g_srcTC);
    T("A(%d,%d)", t == &g_creature, attitude & 0xFF); return P.attSource;
}
int __fastcall AttPlayer(void* m, int, const Thing* t) { CHECK(m == g_mgrBuf); T("P(%d)", t == &g_creature); return P.attPlayer; }
int __fastcall Blur(void* m, int, int att) { CHECK(m == g_mgrBuf); T("B(%d)", att); return P.blurred; }
int __fastcall GetCType(Thing* c, int) { CHECK(c == &g_creature); T("Y"); return P.ctype; }
unsigned long __fastcall Greet(void* op, int, int att) { CHECK(op == &g_opinionTC); T("G(%d)", att); return P.greetBase + att; }
void* __fastcall MainPlayer(void* pm, int) { CHECK(pm == &g_pm); T("M"); return &g_playerObj; }
Thing* __fastcall Controlled(void* p, int) { CHECK(p == &g_playerObj); T("C"); return P.playerNull ? 0 : &g_player; }
int __fastcall GetSex(Thing* c, int) { CHECK(c == &g_creature); T("S"); return P.sex; }
World* __fastcall GetWorld(Root* r, int) { CHECK(r == &g_root); T("W"); return &g_world; }
bool __fastcall Courtable(void* w, int) { CHECK(w == &g_wifeTC); T("c"); return P.courtable; }
bool __fastcall Married(void* w, int) { CHECK(w == &g_wifeTC); T("m"); return P.married; }
bool __fastcall IsStopping(Root* r, int) { CHECK(r == &g_root); T("s"); return ++g_stops == P.stopAt; }
bool __fastcall IsTalking(void* t, int) { CHECK(t == &g_talkTC); T("t"); return g_talkLeft-- > 0; }
void __fastcall Log(void* m, int, int deed, int playing) { CHECK(m == g_mgrBuf); T("O(%d,%d)", deed, playing & 0xFF); }
Def** __fastcall PeekDef(void* log, int, Def** out, int deed)
{
    CHECK(log == g_logBuf); T("E(%d)", deed); ++g_def.ref; *out = &g_def; return out;
}
int __fastcall Speech(void* talk, int, unsigned long tag, int allow, int layer, int b1, int b2)
{
    CHECK(talk == &g_talkTC);
    T("V(%lu,%d,%d,%d,%d)", tag, allow & 0xFF, layer, b1 & 0xFF, b2 & 0xFF);
    if (g_retryLeft > 0) { --g_retryLeft; return 2; }
    return P.finalResult;
}
void __fastcall Terminate(Root* r, int) { CHECK(r == &g_root); T("T"); }
void __fastcall Suspend(Brain* b, int)
{
    CHECK(b == &g_brain); T("U"); ++g_suspends;
    if (g_suspends == P.dieInLoop) g_thing.dead = 1;
}
bool __fastcall IsReady(Action* a, int) { CHECK(a == &g_action); T("R"); return g_suspends >= P.readyAfter; }
void __fastcall DefRelease(Def* d, int, unsigned) { CHECK(0); }
}

// IS_THING_DEAD<CIntelligentPointer<const CThing>> (out of line in the row's TU): defined here as the explicit specialisation, so it
// carries the row's mangled name ??$IS_THING_DEAD@V?$CIntelligentPointer@$$CBVCThing@@@@@@YI_NABV?$CIntelligentPointer@$$CBVCThing@@@@@Z.
class CThing; template<class T> class CIntelligentPointer;
template<class T> bool __fastcall IS_THING_DEAD(const T&);
template<> bool __fastcall IS_THING_DEAD<CIntelligentPointer<const CThing> >(const CIntelligentPointer<const CThing>& ip)
{
    const IP* p = (const IP*)&ip; T("X"); const Thing* t = (const Thing*)p->item;
    return t == 0 || (t->dead & 1) != 0 || P.deadAtAnim;
}

// ---- reference model: the expected trace and delivered flags of one run ----
static char g_exp[8192]; static int g_en; static bool g_expDelivered, g_expSpeech;
static void E(const char* f, ...) { va_list a; va_start(a, f); g_en += vsprintf(g_exp + g_en, f, a); va_end(a); }
static void Model()
{
    int suspends = 0, retryLeft = P.retries, doNothings = 0, talkLeft = P.talking, stops = 0; bool thingDead = P.thingDead;
    g_expDelivered = g_expSpeech = false;
#define STOPPING() (E("s"), ++stops == P.stopAt)
    if (P.thingNull || thingDead) { E("T"); return; }
    E("W");
    int att;
    if (P.hasSource) { E("A(1,1)"); att = P.attSource; }
    else { E("P(1)"); E("B(%d)", P.attPlayer); att = P.blurred; }
    bool log;
    if (P.virt) { E("M"); E("C"); log = !P.playerNull && P.logOnPlayer; }
    else log = P.logOnThing;
    if (!log) { g_expDelivered = true; E("T"); return; }
    int deed = P.deed; Str* const* anim = Anim(deed, att); unsigned long tag;
    if (deed == DEED_TOO_FREQUENT || deed == DEED_INDOORS_GREETING) { E("G(%d)", att); tag = P.greetBase + att; }
    else {
        E("E(%d)", deed);
        bool flirt = P.flirting && (E("S"), P.sex == 2) && (E("Y"), P.ctype == 2) && P.hasWife && (E("m"), !P.married) && (E("c"), !P.courtable);
        if (flirt) tag = *Tag(DEED_INCOMPREHENSION, att);
        else {
            tag = *Tag(deed, att);
            if ((*anim == 0 || (*anim)->len == 0) && tag == 0 && att >= 13) {
                static const int map[5] = { 12, 9, 0, 8, 3 };              // wife love/like/neutral/dislike/hate -> love/agreeable/none/offensive/disdain
                int dummy = map[att - 13];                                  // attitudes stop at WIFE_HATE (17)
                tag = *Tag(deed, dummy); anim = Anim(deed, dummy);
            }
        }
    }
    while (g_creature.cur && g_action.group >= 50 && (E("R"), !(suspends >= P.readyAfter)) && !STOPPING()) {
        E("U"); ++suspends; if (suspends == P.dieInLoop) thingDead = true;
    }
    if (thingDead) { E("T"); return; }
    E("L(1,1,33,3b360a13)");
    if (P.look != 2) { E("T"); return; }
    if (tag != 0 && *Playing(deed) <= 0) {
        E("E(%d)", deed); int ret;
        for (;;) {
            E("E(%d)", deed); E("V(%lu,%d,%d,0,1)", tag, P.allow ? 1 : 0, P.layer);
            ret = retryLeft > 0 ? (--retryLeft, 2) : P.finalResult;
            if (ret != 2) break;
            E("N"); if (++doNothings == P.doNothingFailAt) { E("T"); return; }
        }
        if (ret == 0) { g_expSpeech = true; E("O(%d,1)", deed); }
        g_expDelivered = true;
    }
    const char* text = *anim ? (*anim)->text : "";
    E("n");
    if (*text) {
        E("X");
        if (thingDead || P.deadAtAnim) { E("T"); return; }
        E("K(%s,1,0,1,0,0,0)", text); E("F(1,1,0)"); E("k");
        if (P.perform != 2) { E("T"); return; }
        g_expDelivered = true;
    }
    if (tag == 0) { if (*anim) E("e"); if (!*text) g_expDelivered = true; }
    while ((E("t"), talkLeft-- > 0) && !STOPPING()) { E("U"); ++suspends; }
    E("T");
#undef STOPPING
}

static void Reset()
{
    memset(&P, 0, sizeof P);
    P.logOnThing = P.logOnPlayer = true; P.look = 2; P.finalResult = 0; P.perform = 2; P.greetBase = 1000; P.layer = 3; P.sex = 2; P.ctype = 2;
    P.deed = 5; P.attPlayer = 4; P.blurred = 6; P.attSource = 7; P.readyAfter = 0; P.actionGroup = 10;
}
static void Run()
{
    ++g_scn; if (getenv("TDBG")) printf("scenario %d\n", g_scn); g_tn = 0; g_trace[0] = 0; g_en = 0; g_exp[0] = 0;
    g_suspends = 0; g_retryLeft = P.retries; g_doNothings = 0; g_talkLeft = P.talking; g_stops = 0; g_animLive = 0;
    memset(&g_root, 0, sizeof g_root); g_root.brain = &g_brain; g_root.ip.vtbl = g_ipvtbl; g_root.ip.item = P.thingNull ? 0 : &g_thing;
    g_root.deed = P.deed; g_root.virt = P.virt;
    memset(&g_thing, 0, sizeof g_thing); memset(&g_creature, 0, sizeof g_creature); memset(&g_player, 0, sizeof g_player);
    g_thing.dead = P.thingDead ? 1 : 0;
    g_thing.map.first = g_thing.map.end = &g_thing.pairs[0]; g_creature.map.first = g_creature.map.end = &g_creature.pairs[0];
    g_player.map.first = g_player.map.end = &g_player.pairs[0];
    if (P.hasSource) { g_thing.f28 |= 1u << 16; AddTC(&g_thing, 0x50, &g_srcTC); }
    if (P.logOnThing) { g_thing.f28 |= 1u << 17; AddTC(&g_thing, 0x51, g_logBuf); }
    if (P.logOnPlayer) { g_player.f28 |= 1u << 17; AddTC(&g_player, 0x51, g_logBuf); }
    g_creature.f28 |= 1u << 15; AddTC(&g_creature, 0x4F, &g_opinionTC);
    g_creature.f20 |= 1u << 15; AddTC(&g_creature, 0x0F, &g_talkTC);
    if (P.hasWife) { g_creature.f38 |= 1u << 23; AddTC(&g_creature, 0xD7, &g_wifeTC); }
    g_creature.cur = P.actionGroup ? &g_action : 0; g_action.vtbl = g_actionVtbl; g_action.group = P.actionGroup;
    g_def.vtbl = g_defVtbl; g_def.ref = 1; g_def.layer = P.layer; g_def.f7c = P.flirting ? 0x40 : 0; g_def.f7d = P.allow ? 1 : 0;
    Model();
    CallUpdate(&g_root, 0);
    CHECK(strcmp(g_trace, g_exp) == 0);
    if (strcmp(g_trace, g_exp)) printf("  got    %s\n  expect %s\n", g_trace, g_exp);
    CHECK(g_root.delivered == g_expDelivered && g_root.speech == g_expSpeech);
    CHECK(g_def.ref == 1 && g_animLive == 0);
}

static void ClearLog() { memset(g_logBuf, 0, sizeof g_logBuf); memset(g_mgrBuf, 0, sizeof g_mgrBuf); }

int main()
{
    setvbuf(stdout, 0, _IONBF, 0);
    g_brain.vtbl = g_brainVtbl; g_brainVtbl[2] = (void*)Suspend; g_brain.creature = &g_creature;
    g_actionVtbl[27] = (void*)IsReady; g_defVtbl[1] = (void*)DefRelease;
    g_world.mgr = g_mgrBuf; g_mgc = &g_mgcObj; g_mgcObj.pm = &g_pm;
    ClearLog();
    // early exits
    Reset(); P.thingNull = true; Run();
    Reset(); P.thingDead = true; Run();
    Reset(); P.logOnThing = false; Run();                                              // no deed log: delivered, terminate
    Reset(); P.virt = true; P.playerNull = true; Run();                                // virtual deed, no player creature
    Reset(); P.virt = true; P.logOnPlayer = false; Run();
    // attitude: source / blurred, plain deed with an animation and a tag
    *Anim(5, 7) = &g_strWave; *Tag(5, 7) = 501; *Anim(5, 6) = &g_strNod; *Tag(5, 6) = 502;
    Reset(); P.hasSource = true; Run();
    Reset(); Run();
    Reset(); P.virt = true; Run();
    Reset(); P.look = 1; Run();                                                        // look-at fails
    Reset(); P.perform = 1; Run();                                                     // animation fails
    Reset(); P.deadAtAnim = 1; Run();
    Reset(); P.finalResult = 1; Run();                                                 // speech failed (final): delivered, no speech flag
    Reset(); P.retries = 3; P.allow = true; P.layer = 7; Run();                        // retry loop
    Reset(); P.retries = 3; P.doNothingFailAt = 2; Run();                              // retry loop broken by DoNothing
    *Playing(5) = 1; Reset(); Run(); *Playing(5) = 0;                                  // speech already playing for the deed
    Reset(); P.talking = 4; Run();
    Reset(); P.talking = 4; P.stopAt = 2; Run();
    // interruption wait
    Reset(); P.actionGroup = 50; P.readyAfter = 3; Run();
    Reset(); P.actionGroup = 49; P.readyAfter = 3; Run();
    Reset(); P.actionGroup = 60; P.readyAfter = 5; P.stopAt = 3; Run();
    Reset(); P.actionGroup = 60; P.readyAfter = 5; P.dieInLoop = 2; Run();            // the thing dies while waiting
    // greeting deeds: the opinion component's greet group
    *Anim(DEED_TOO_FREQUENT, 6) = &g_strShrug; *Anim(DEED_INDOORS_GREETING, 7) = &g_strEmpty;
    Reset(); P.deed = DEED_TOO_FREQUENT; Run();
    Reset(); P.deed = DEED_INDOORS_GREETING; P.hasSource = true; P.talking = 1; Run();
    // flirting: incomprehension tag when every test holds, else the deed's own
    *Tag(DEED_INCOMPREHENSION, 6) = 7300; *Tag(9, 6) = 900; *Anim(9, 6) = &g_strWave;
    Reset(); P.deed = 9; P.flirting = true; P.hasWife = true; Run();
    Reset(); P.deed = 9; P.flirting = true; P.hasWife = true; P.sex = 1; Run();
    Reset(); P.deed = 9; P.flirting = true; P.hasWife = true; P.ctype = 3; Run();
    Reset(); P.deed = 9; P.flirting = true; P.hasWife = false; Run();
    Reset(); P.deed = 9; P.flirting = true; P.hasWife = true; P.married = true; Run();
    Reset(); P.deed = 9; P.flirting = true; P.hasWife = true; P.courtable = true; Run();
    Reset(); P.deed = 9; P.hasWife = true; Run();
    // wife attitudes with no animation and no tag: the plain attitude's reaction instead
    static const int plain[5] = { 12, 9, 0, 8, 3 };
    for (int w = 0; w < 5; ++w) {
        *Tag(11, plain[w]) = 1100 + plain[w]; *Anim(11, plain[w]) = (w & 1) ? &g_strShrug : &g_strEmpty;
        Reset(); P.deed = 11; P.blurred = 13 + w; Run();
        Reset(); P.deed = 11; P.blurred = 13 + w; *Anim(11, 13 + w) = &g_strEmpty; Run(); *Anim(11, 13 + w) = 0;
        Reset(); P.deed = 11; P.blurred = 13 + w; *Tag(11, 13 + w) = 5; Run(); *Tag(11, 13 + w) = 0;      // own tag: no fallback
        Reset(); P.deed = 11; P.blurred = 13 + w; *Anim(11, 13 + w) = &g_strNod; Run(); *Anim(11, 13 + w) = 0;
    }
    Reset(); P.deed = 11; P.blurred = 12; Run();                                       // below the wife range: no fallback
    Reset(); P.deed = 12; P.blurred = 2; Run();                                        // no tag, no animation: delivered anyway
    if (g_fail) { printf("00960120_TEST FAIL\n"); return 1; }
    printf("00960120_TEST PASS (%d scenarios)\n", g_scn);
    return 0;
}