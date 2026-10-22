// Grade: DIFFER(817v807) -- frame-slot packing: retail shares two small slots (+0x10 strings/def-out/emitter, +0x14 keys/effect/item) and puts the action temp at +0x1c; ours spreads the sword-block locals 8 bytes higher
// CAIStateGroup_JackOfBladesStage1::PowerUpSword() -- play SUMMON_INTO, then loop SUMMON_LOOP (holding the obstruction driver at 60) while the boss still has living helpers, terminating if the target or arena centre dies or the group stops; once still runnable, create the SWORD_OF_AEONS effect at the creature, keep it as the sword emitter and attach it to the primary-slot weapon's WEAPON_FOCUS_01, then play SUMMON_OUTOF and release the effects def
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00974770, length 807 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020, CCharString ctor @ 0x0099EBF0 / dtor @ 0x0099EAE0,
// CCreatureAction_PlayCombatAnimation ctor @ 0x008B7FF0, ~CCombatActionBase @ 0x0062E900, AIAction_PerformAction
// @ 0x0097D8B0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10, IsStopping @ 0x008FD010,
// CTCDriverED::SetValue @ 0x007FD3D0, CTCBoss::CountLivingHelperCreatures @ 0x007AEEC0, IsStillRunnable @ 0x00971B00,
// CDefClassBase::GetSubDef @ 0x00669580, CSpecialEffectsDef::CalcAndLookup @ 0x00669D30, CThing::GetPosition
// @ 0x004C73D0, CreateEffectThing @ 0x006E0880 (`__fastcall`), CIntelligentPointer<CThing> ctor @ 0x004A96F0 /
// operator= @ 0x004A9760, CThing::GetTCDriver<CTCDParticleEmitter> @ 0x004ACB50, CTCCarrying::GetThingInPrimarySlot
// @ 0x006A1440, CTCDParticleEmitter::AttachToThing @ 0x006E0BE0, Terminate @ 0x008FCF40.
// Layout: Ego_r.pdb CAIStateGroup_JackOfBladesBase PSwordEmitter +0x14, PArenaCentre +0x1c, PTargetThing +0x24,
// PillarMarkers/Pillars (retail 12-byte vectors) +0x34/+0x40; Stage1 PObstruction +0x54 (PDB +0x5c less two vectors).
struct C3DVector { float X, Y, Z; };
struct CTCBase;
struct CThing;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct CSpecialEffectsDef {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30
};
struct CDefClassBase {
    bool GetSubDef(CSpecialEffectsDef** out);         // 0x00669580
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x5c - 0x4c];
    CTCBase* PTCDriver;                               // +0x5c
    unsigned char _pad60[0x70 - 0x60];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;             // 0x004C73D0
    CDefClassBase* GetDef() const { return m_pDef; }
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> bool GetTCDriver(T** out);      // 0x004ACB50 (T = CTCDParticleEmitter)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
struct CTCBoss;
struct CThingCreatureBase : CThing {
    CTCBoss* GetBoss()
    {
        CTCBoss* boss;
        GetInterface(&boss, 0xad);
        return boss;
    }
    CSpecialEffectsDef* GetEffectsDef() const
    {
        CSpecialEffectsDef* def = 0;
        GetDef()->GetSubDef(&def);
        return def;
    }
};
struct CTCBoss {                                      // interface 0xad
    long CountLivingHelperCreatures() const;          // 0x007AEEC0
};
struct CTCDriverED {                                  // driver of interface 0xed (the obstruction)
    void SetValue(float value);                       // 0x007FD3D0
};
struct CTCCarrying {                                  // interface 0x46
    CThing* GetThingInPrimarySlot();                  // 0x006A1440
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    ~CBaseIntelligentPointer();                       // 0x00A01C10
    CThing* GetPItem() const;                         // 0x00A01B50
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item);                     // 0x004A96F0
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
};
struct CCombatActionBase : CCreatureActionBase {
    unsigned char _combat[0x6c];
    ~CCombatActionBase();                             // 0x0062E900
};
struct CCreatureAction_PlayCombatAnimation : CCombatActionBase {
    long m_loop;
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* creature, const CCharString& animation, bool loop);   // 0x008B7FF0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                    // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                 // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    bool IsStopping();                                // 0x008FD010
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
    void Terminate();                                 // 0x008FCF40
};
struct CAIStateGroup_JackOfBladesBase : CAIStateGroupBase {
    CIntelligentPointer<CThing> PSwordEmitter;        // +0x14
    CIntelligentPointer<CThing> PArenaCentre;         // +0x1c
    CBaseIntelligentPointer PTargetThing;             // +0x24
    long EnemyKnockDownFrame;                         // +0x2c
    long SpeechCounter;                               // +0x30
    unsigned char PillarMarkers[0x0c];                // +0x34
    unsigned char Pillars[0x0c];                      // +0x40
    bool IsStillRunnable();                           // 0x00971B00
};
struct CAIStateGroup_JackOfBladesStage1 : CAIStateGroup_JackOfBladesBase {
    bool HasAttacked;                                 // +0x4c
    long ZigZagCounter;                               // +0x50
    CBaseIntelligentPointer PObstruction;             // +0x54
    CBaseIntelligentPointer PShieldThing;             // +0x5c
    void PowerUpSword();
};

void CAIStateGroup_JackOfBladesStage1::PowerUpSword()
{
    CTCBoss* boss = GetCreature()->GetBoss();
    AIAction_PerformAction(&CCreatureAction_PlayCombatAnimation(GetCreature(), "SUMMON_INTO", true));
    do {
        if (!IS_THING_ALIVE(PTargetThing) || IsStopping() || !IS_THING_ALIVE(PArenaCentre)) {
            Terminate();
            return;
        }
        if (IS_THING_ALIVE(PObstruction)) {
            CThing* obstruction = PObstruction.GetPItem();
            CTCDriverED* driver;
            if (obstruction->HasTC(0xed))
                driver = (CTCDriverED*)obstruction->PTCDriver;
            driver->SetValue(60.0f);
        }
        AIAction_PerformAction(&CCreatureAction_PlayCombatAnimation(GetCreature(), "SUMMON_LOOP", true));
    } while (boss->CountLivingHelperCreatures() > 0);
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    CSpecialEffectsDef* effects = GetCreature()->GetEffectsDef();
    if (effects) {
        long effect;
        if (effects->CalcAndLookup("SWORD_OF_AEONS", &effect)) {
            CThing* sword = CreateEffectThing(effect, &GetCreature()->GetPosition(), false);
            if (sword != 0 && !(sword->m_flags91 & 1)) {
                {
                    CIntelligentPointer<CThing> item(sword);
                    PSwordEmitter = item;
                }
                CTCDParticleEmitter* emitter = 0;
                if (sword->GetTCDriver(&emitter)) {
                    CTCCarrying* carrying = 0;
                    GetCreature()->GetInterface(&carrying, 0x46);
                    CThing* weapon = carrying->GetThingInPrimarySlot();
                    emitter->AttachToThing(weapon, 10, "WEAPON_FOCUS_01", 0, 0);
                }
            }
        }
    }
    AIAction_PerformAction(&CCreatureAction_PlayCombatAnimation(GetCreature(), "SUMMON_OUTOF", true));
    if (effects) {
        if (--effects->RefCount == 0)
            effects->Destroy();
    }
}
