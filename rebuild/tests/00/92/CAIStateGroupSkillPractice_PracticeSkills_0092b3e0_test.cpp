// Behaviour test for 0092b3e0: src/0092b3e0.cpp copied verbatim, out-of-line callees stubbed below.
#include <cstdio>
#include <cstring>
// ---- source (verbatim) ----
// CAIStateGroup_SkillPractice::PracticeSkills() -- practise on a training dummy: without a dummy just idle (true); otherwise pick the practice animations for the dummy type (0 strike, 1 bow load/release, 2 spells; strike and bow unsheathe the weapon), walk to the dummy and repeat 5..14 times: look at the dummy, play the practice (bow: aim then fire; else one or two combat animations), and for strike/spell dummies sometimes wander off and walk back; any failure ends the practice (false); strike/bow sheathe again
// Grade: RELOCATION_MATCH (retry A 11a) -- the second world-seed random is a __forceinline Random() without named temporaries (seed*0x24A1+0x24DF stored, then *seed = GFROR13(*seed)); the first is written out with a named `long count = *seed % 10;` so the +5 is a lea into the repetitions register
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092B3E0, length 932 bytes (+ int3 padding), plain `ret`.
// Callees: GetDummy @ 0x0092A0C0, AIAction_DoNothing @ 0x0097E200, CCharString(const char*, long) @ 0x0099EBF0 /
// operator=(const char*) @ 0x0099EFE0 / operator!=(const char*) @ 0x0099E960 / ~CCharString @ 0x0099EAE0,
// CThingCreatureBase::GetCombatManager @ 0x00661ED0, CCombatManager::CreatureUnsheathWeapon @ 0x006EBDD0, GetPWorldSeed @
// 0x008FCFD0, GFROR13 @ 0x00497890, MoveToAppropriateDummy @ 0x0092A220, IsDummyTypeLegal @ 0x00929F70 (the "ApprenticeHero"
// labels are the propagated sibling names), CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / dtor @
// 0x00A01C10 (by-value CIntelligentPointer<const CThing>, vtable 0x01237F1C), AIAction_LookAtThing @ 0x0097D6F0,
// CCreatureAction_AimMissileWeapon ctor @ 0x00718710, CCreatureAction_FireMissileWeapon ctor @ 0x008D5110 / dtor @
// 0x0078ADF0, CCreatureAction_PlayCombatAnimation ctor @ 0x008B7FF0 / ~CCombatActionBase @ 0x0062E900,
// ~CCreatureActionBase @ 0x00693EF0, AIAction_PerformActionAndWait @ 0x0097DB70, AIAction_MinionWander @ 0x0097F870.
// Data: "ST_TRAINEE_STRIKE_DUMMY" @ 0x01291AD8, "ST_TRAINEE_FIRE_BOW_LOAD" @ 0x01291B20, "ST_TRAINEE_FIRE_BOW_RELEASE" @
// 0x01291B04, "ST_TRAINEE_SPELLS" @ 0x01291B3C, "" @ 0x0122D70E; look speed 0.0027777f (immediate).
// Layout: engine/CAIStateGroup_ApprenticeHero.h-style members (PDummy +0x18, dummy type +0x1c), modelled locally; retail
// CCreatureActionBase 0xa8 bytes (aim 0xb0, fire 0xc8), CCombatActionBase 0x114 (play-combat-animation 0x118).
// Shape: each practice step is `failed = name != "" && PerformActionAndWait(&Action(...), false) != 2;` (conditional
// temporaries, ebx flag bits 1/2/4/8, the bool in a frame byte); the failure exits are gotos to one `success = false`.
struct CThing;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
    CCharString& operator=(const char* text);            // 0x0099EFE0
    bool operator!=(const char* text) const;             // 0x0099E960
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {    // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    __forceinline CIntelligentPointerConstThing(const CThing* item) { SetPItem(item); }
};
struct CThingCreatureBase;
struct CCombatManager {
    void CreatureUnsheathWeapon(CThingCreatureBase* creature, long mode);   // 0x006EBDD0
};
struct CThingCreatureBase {
    CCombatManager* GetCombatManager();         // 0x00661ED0
};
struct CCreatureActionBase {                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_AimMissileWeapon : CCreatureActionBase {    // retail sizeof 0xb0
    CBaseIntelligentPointer PTarget;            // +0xa8
    CCreatureAction_AimMissileWeapon(CThingCreatureBase* creature);   // 0x00718710
};
struct CCreatureAction_FireMissileWeapon : CCreatureActionBase {   // retail sizeof 0xc8
    unsigned char _fire[0xc8 - 0xa8];
    CCreatureAction_FireMissileWeapon(CThingCreatureBase* creature, const CThing* target);   // 0x008D5110
    ~CCreatureAction_FireMissileWeapon();       // 0x0078ADF0
};
struct CCombatActionBase : CCreatureActionBase {    // retail sizeof 0x114
    unsigned char _combat[0x114 - 0xa8];
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCreatureAction_PlayCombatAnimation : CCombatActionBase {   // retail sizeof 0x118
    long Loop;                                  // +0x114
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* creature, const CCharString& animation, bool loop);   // 0x008B7FF0
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long frames, float speed);   // 0x0097D6F0
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    long AIAction_MinionWander(long movementType, CThing* building);   // 0x0097F870
    __forceinline unsigned long Random()
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24A1U + 0x24DFU;
        return *seed = GFROR13(*seed);
    }
};
struct CAIStateGroup_SkillPractice : CAIStateGroupBase {
    long FrameToCheckForDummy;                  // +0x14
    CThing* PDummy;                             // +0x18
    long DummyType;                             // +0x1c (0 strike, 1 bow, 2 spells)
    bool GetDummy();                            // 0x0092A0C0
    bool MoveToAppropriateDummy();              // 0x0092A220
    bool IsDummyTypeLegal(long type);           // 0x00929F70
    bool PracticeSkills();
};

bool CAIStateGroup_SkillPractice::PracticeSkills()
{
    long repetitions = 0;
    GetDummy();
    if (PDummy == 0) {
        AIAction_DoNothing();
        return true;
    }
    CCharString firstAnim("");
    CCharString secondAnim("");
    switch (DummyType) {
    case 0:
        firstAnim = "ST_TRAINEE_STRIKE_DUMMY";
        GetCreature()->GetCombatManager()->CreatureUnsheathWeapon(GetCreature(), 4);
        break;
    case 1:
        firstAnim = "ST_TRAINEE_FIRE_BOW_LOAD";
        secondAnim = "ST_TRAINEE_FIRE_BOW_RELEASE";
        GetCreature()->GetCombatManager()->CreatureUnsheathWeapon(GetCreature(), 5);
        break;
    case 2:
        firstAnim = "ST_TRAINEE_SPELLS";
        break;
    }
    unsigned long* seed = GetPWorldSeed();
    *seed = *seed * 0x24A1U + 0x24DFU;
    *seed = GFROR13(*seed);
    long count = *seed % 10;
    repetitions = count + 5;
    bool success = true;
    if (!MoveToAppropriateDummy() || !IsDummyTypeLegal(DummyType))
        goto failed;
    for (long i = 0; i < repetitions; ++i) {
        if (AIAction_LookAtThing(1, CIntelligentPointerConstThing(PDummy), 1, 0.0027777f) != 2)
            goto failed;
        bool stepFailed;
        if (DummyType == 1) {
            stepFailed = firstAnim != ""
                && AIAction_PerformActionAndWait(&CCreatureAction_AimMissileWeapon(GetCreature()), false) != 2;
            if (stepFailed)
                goto failed;
            stepFailed = secondAnim != ""
                && AIAction_PerformActionAndWait(&CCreatureAction_FireMissileWeapon(GetCreature(), PDummy), false) != 2;
        } else {
            stepFailed = firstAnim != ""
                && AIAction_PerformActionAndWait(&CCreatureAction_PlayCombatAnimation(GetCreature(), firstAnim, true), false) != 2;
            if (stepFailed)
                goto failed;
            stepFailed = secondAnim != ""
                && AIAction_PerformActionAndWait(&CCreatureAction_PlayCombatAnimation(GetCreature(), secondAnim, true), false) != 2;
        }
        if (stepFailed)
            goto failed;
        if ((DummyType == 0 || DummyType == 2) && (Random() & 3) == 0) {
            if (AIAction_MinionWander(3, 0) != 2 || !MoveToAppropriateDummy())
                goto failed;
        }
        if (!IsDummyTypeLegal(DummyType))
            goto failed;
    }
    goto done;
failed:
    success = false;
done:
    if (DummyType >= 0 && DummyType <= 1)
        GetCreature()->GetCombatManager()->CreatureUnsheathWeapon(GetCreature(), 1);
    return success;
}

// ---- stubs ----
CCharString::CCharString(const char* text, long) { PStringData = (void*)text; }
CCharString::~CCharString() {}
CCharString& CCharString::operator=(const char* text) { PStringData = (void*)text; return *this; }
bool CCharString::operator!=(const char* text) const { return std::strcmp((const char*)PStringData, text) != 0; }
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointerConstThing::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(const CThing* item) { m_pItem = (CThing*)item; }
static CCombatManager g_cm;
static long g_unsheath[8]; static int g_nUnsheath;
CCombatManager* CThingCreatureBase::GetCombatManager() { return &g_cm; }
void CCombatManager::CreatureUnsheathWeapon(CThingCreatureBase*, long mode) { g_unsheath[g_nUnsheath++] = mode; }
void CCreatureActionBase::v00() {}
CCreatureActionBase::~CCreatureActionBase() {}
static int g_aim, g_fire, g_play, g_playSecond;
CCreatureAction_AimMissileWeapon::CCreatureAction_AimMissileWeapon(CThingCreatureBase*) { g_aim++; }
CCreatureAction_FireMissileWeapon::CCreatureAction_FireMissileWeapon(CThingCreatureBase*, const CThing*) { g_fire++; }
CCreatureAction_FireMissileWeapon::~CCreatureAction_FireMissileWeapon() {}
CCombatActionBase::~CCombatActionBase() {}
CCreatureAction_PlayCombatAnimation::CCreatureAction_PlayCombatAnimation(CThingCreatureBase*, const CCharString& a, bool)
{ if (std::strcmp((const char*)a.PStringData, "ST_TRAINEE_SPELLS") == 0 || std::strcmp((const char*)a.PStringData, "ST_TRAINEE_STRIKE_DUMMY") == 0) g_play++; else g_playSecond++; }
unsigned long __fastcall GFROR13(unsigned long v) { return (v >> 13) | (v << 19); }
CAIStateGroupBase::~CAIStateGroupBase() {}
static unsigned long g_seed;
unsigned long* CAIStateGroupBase::GetPWorldSeed() { return &g_seed; }
static int g_nothing, g_look, g_lookFailAt, g_wander, g_perform;
long CAIStateGroupBase::AIAction_DoNothing() { g_nothing++; return 2; }
long CAIStateGroupBase::AIAction_LookAtThing(long, CIntelligentPointerConstThing p, long, float) { ++g_look; return g_look == g_lookFailAt ? 1 : 2; }
long CAIStateGroupBase::AIAction_PerformActionAndWait(CCreatureActionBase*, bool) { g_perform++; return 2; }
long CAIStateGroupBase::AIAction_MinionWander(long, CThing*) { g_wander++; return 2; }
static CThing* g_dummy; static bool g_moveOk = true; static int g_legalCalls;
bool CAIStateGroup_SkillPractice::GetDummy() { PDummy = g_dummy; return g_dummy != 0; }
bool CAIStateGroup_SkillPractice::MoveToAppropriateDummy() { return g_moveOk; }
bool CAIStateGroup_SkillPractice::IsDummyTypeLegal(long) { g_legalCalls++; return true; }

static int fails = 0;
static void check(bool c, const char* m) { if (!c) { std::printf("FAIL %s\n", m); fails++; } }
static unsigned long lcg(unsigned long& s) { s = s * 0x24A1u + 0x24DFu; s = (s >> 13) | (s << 19); return s; }
static void reset(unsigned long seed) {
    g_seed = seed; g_nUnsheath = 0; g_aim = g_fire = g_play = g_playSecond = 0;
    g_nothing = g_look = g_wander = g_perform = 0; g_lookFailAt = -1; g_moveOk = true; g_legalCalls = 0;
}

int main()
{
    CAIBrain brain; std::memset(&brain, 0, sizeof brain);
    CThingCreatureBase creature; brain.PCreature = &creature;
    static double dummyStore[4]; CThing* dummy = (CThing*)dummyStore;
    CAIStateGroup_SkillPractice g; g.PBrain = &brain; g.PDummy = 0;

    // no dummy: idle, true
    reset(1); g_dummy = 0; g.DummyType = 0;
    check(g.PracticeSkills() == true && g_nothing == 1 && g_look == 0, "no dummy");

    // strike dummy: reps = r%10+5 combat animations, a wander when the next random & 3 == 0
    for (unsigned long seed = 1; seed < 40; seed += 7) {
        reset(seed); g_dummy = dummy; g.DummyType = 0;
        unsigned long s = seed; long reps = (long)(lcg(s) % 10) + 5; int wanders = 0;
        for (long i = 0; i < reps; ++i) if ((lcg(s) & 3) == 0) wanders++;
        bool ok = g.PracticeSkills();
        check(ok && g_play == reps && g_playSecond == 0 && g_look == reps && g_wander == wanders, "strike counts");
        check(g_nUnsheath == 2 && g_unsheath[0] == 4 && g_unsheath[1] == 1, "strike unsheath");
        check(g_seed == s, "strike seed");
    }
    // bow dummy: aim + fire per repetition, no wander, unsheath 5 then 1
    reset(77); g.DummyType = 1;
    { unsigned long s = 77; long reps = (long)(lcg(s) % 10) + 5;
      check(g.PracticeSkills() && g_aim == reps && g_fire == reps && g_wander == 0 && g_play == 0, "bow counts");
      check(g_nUnsheath == 2 && g_unsheath[0] == 5 && g_unsheath[1] == 1, "bow unsheath"); }
    // spells: no unsheath
    reset(5); g.DummyType = 2;
    check(g.PracticeSkills() && g_nUnsheath == 0 && g_play > 0, "spells");
    // look failure on the 2nd pass: false, sheathe still runs
    reset(9); g.DummyType = 0; g_lookFailAt = 2;
    check(g.PracticeSkills() == false && g_look == 2 && g_play == 1 && g_nUnsheath == 2 && g_unsheath[1] == 1, "look fails");
    // cannot reach the dummy: false, no looks
    reset(9); g.DummyType = 1; g_moveOk = false;
    check(g.PracticeSkills() == false && g_look == 0 && g_nUnsheath == 2, "no move");
    if (fails == 0) std::printf("0092b3e0_TEST PASS\n");
    return fails;
}