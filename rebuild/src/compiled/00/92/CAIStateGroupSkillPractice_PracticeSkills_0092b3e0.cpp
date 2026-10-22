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