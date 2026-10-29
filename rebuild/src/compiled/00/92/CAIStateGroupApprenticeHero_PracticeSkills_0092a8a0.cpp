// CAIStateGroup_ApprenticeHero::PracticeSkills() -- practise on a training dummy: without a dummy just idle (true); otherwise pick the practice animations for the dummy type (0 strike, 1 bow load/release, 2 spells) and give the creature the practice sword or bow in its hand slot, walk to the dummy and repeat 5..14 times: look at the dummy, play the practice (bow: aim then fire; else one or two combat animations), and for strike/spell dummies sometimes wander off and walk back; any failure ends the practice (false); the sword/bow is destroyed again afterwards
// Grade: RELOCATION_MATCH -- the landed SkillPractice twin with const char* arguments for the carried-object calls (temporaries built before the pushes)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092A8A0, 1088 bytes, plain ret.
// Twin: the landed CAIStateGroup_SkillPractice::PracticeSkills 0x0092B3E0 (same callees and step shape); this one gives
//   and destroys carried objects (0x008FD0A0 / DestroyItem 0x008FD1B0) instead of unsheathing, and looks with the shared
//   g_LibraryLookTurnSpeed 0x0138CBA8. Literals "OBJECT_IRON_LONGSWORD" 0x01291A50, "OBJECT_YEW_LONGBOW" 0x01291AF0,
//   "CARRY_SLOT_RIGHT_HAND" 0x01265440, "CARRY_SLOT_LEFT_HAND" 0x01265428.
class CThing;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
    const CCharString& operator=(const char* text);            // 0x0099EFE0
    bool operator!=(const char* text) const;             // 0x0099E960
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {    // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    __forceinline CIntelligentPointerConstThing(const CThing* item) { SetPItem(item); }
};
class CThingCreatureBase;
class CCombatManager { public:
    void CreatureUnsheathWeapon(CThingCreatureBase* creature, long mode);   // 0x006EBDD0
};
class CThingCreatureBase { public:
    CCombatManager& GetCombatManager();         // 0x00661ED0
};
class CCreatureActionBase { public:                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    virtual ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_AimMissileWeapon : public CCreatureActionBase { public:    // retail sizeof 0xb0
    CBaseIntelligentPointer PTarget;            // +0xa8
    CCreatureAction_AimMissileWeapon(CThingCreatureBase* creature);   // 0x00718710
};
class CCreatureAction_FireMissileWeapon : public CCreatureActionBase { public:   // retail sizeof 0xc8
    unsigned char _fire[0xc8 - 0xa8];
    CCreatureAction_FireMissileWeapon(CThingCreatureBase* creature, const CThing* target);   // 0x008D5110
    ~CCreatureAction_FireMissileWeapon();       // 0x0078ADF0
};
class CCombatActionBase : public CCreatureActionBase { public:    // retail sizeof 0x114
    unsigned char _combat[0x114 - 0xa8];
    virtual ~CCombatActionBase();                       // 0x0062E900
};
class CCreatureAction_PlayCombatAnimation : public CCombatActionBase { public:   // retail sizeof 0x118
    long Loop;                                  // +0x114
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* creature, const CCharString& animation, bool loop);   // 0x008B7FF0
};
extern float g_LibraryLookTurnSpeed;            // 0x0138CBA8
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    protected:
    unsigned long* GetPWorldSeed() const;             // 0x008FCFD0
    public:
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
struct CAIStateGroup_ApprenticeHero : CAIStateGroupBase {
    long FrameToCheckForDummy;                  // +0x14
    CThing* PDummy;                             // +0x18
    long DummyType;                             // +0x1c (0 strike, 1 bow, 2 spells)
    bool GetDummy();                            // 0x0092A0C0
    bool MoveToAppropriateDummy();              // 0x0092A220
    bool IsDummyTypeLegal(long type);           // 0x00929F70
    bool PracticeSkills();
    void GiveCarriedObject(const CCharString& object, const CCharString& slot);   // 0x008FD0A0
    void DestroyItem(const CCharString& slot);  // 0x008FD1B0
};

bool CAIStateGroup_ApprenticeHero::PracticeSkills()
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
        GiveCarriedObject("OBJECT_IRON_LONGSWORD", "CARRY_SLOT_RIGHT_HAND");
        break;
    case 1:
        firstAnim = "ST_TRAINEE_FIRE_BOW_LOAD";
        secondAnim = "ST_TRAINEE_FIRE_BOW_RELEASE";
        GiveCarriedObject("OBJECT_YEW_LONGBOW", "CARRY_SLOT_LEFT_HAND");
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
        if (AIAction_LookAtThing(1, CIntelligentPointerConstThing(PDummy), 1, g_LibraryLookTurnSpeed) != 2)
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
    switch (DummyType) {
    case 0:
        DestroyItem("CARRY_SLOT_RIGHT_HAND");
        break;
    case 1:
        DestroyItem("CARRY_SLOT_LEFT_HAND");
        break;
    }
    return success;
}