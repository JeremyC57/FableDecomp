// CAIStateGroup_SummonerLightningAttack::Update() -- with a live target and the summoner free to act: mark the target's stealth as in danger, sheathe any live weapon in the primary slot, turn to face the target, play the lightning-orb-into action, wait out any running action, play the ground-smash out-of action and schedule the next preemptive use from the def's delay; always terminates
// Leaf: ?Update@CAIStateGroup_SummonerLightningAttack
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096FCD0, length 568 bytes (0x128 frame: three action temporaries share it).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (0x36 stealth, ternary
//   pick) / const @ 0x004365B0 (0x46 carrying), CTCStealth::SetAsInDanger @ 0x00747B80, CTCCarrying::GetThingInPrimarySlot
//   @ 0x006A1440, CCreatureAction_SummonerSheatheWeapon ctor @ 0x008CBF00, CCreatureAction_SummonerLightningOrbInto ctor
//   @ 0x008CBD90 / dtor @ 0x00719C30, the ground-smash out-of action ctor @ 0x00970240 (unlabelled), ~CCombatActionBase
//   @ 0x0062E900, AIAction_CombatPerformActionAndWait @ 0x0097DA40, AIAction_LookAtThing @ 0x0097D6F0 (the target passed
//   by value through CIntelligentPointer<const CThing>(const CIntelligentPointer<CThing>&) @ 0x007C2930),
//   IS_THING_ALIVE<CIntelligentPointer<CThing>> @ 0x004CC340 (__fastcall), IsPerformingAnyAction @ 0x006622A0,
//   AIAction_DoNothing @ 0x0097E200, GetPWorld @ 0x008FCFC0 (evaluated for the static GetWorldFrame @ 0x0049D870),
//   Terminate @ 0x008FCF40; frame rate global 0x01375550, ftol 0x00BFEA70.
// Layout: Ego_r.pdb CAIStateGroup_SummonerLightningAttack (PTarget +0x14, PDef +0x1c, NextFrameForPreemptiveUse +0x20) over
//   the landed CAIStateGroupBase model (the generated header flattens the vptr into __vftable/_pad bytes); creature flag
//   byte +0x1c8 bit 1 (retail), CSummonerDef +0x38 the lightning delay (retail).
// Shape: the sheathe loop is `for (;;)` with `break`/`continue` exits and `if (result != 1) continue; Terminate(); return;`
//   (keeps retail's unrotated loop and the in-loop Terminate epilogue); every perform result is a NAMED long (an inline
//   `== 1` on the call materialises a bool because of the temporary's destructor); the delay and the frame rate are read
//   into locals before the GetWorldFrame call.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterfaceX(T** ppOut, long interfaceType)
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
    template<class T> __forceinline bool GetInterfaceC(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { public:
    unsigned char _pad94[0x1c8 - 0x94];
    unsigned char CreatureFlags1c8;             // +0x1c8 bit 1: busy (retail)
    bool IsPerformingAnyAction();               // 0x006622A0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerThing : CBaseIntelligentPointer { };
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {
    CIntelligentPointerConstThing(const CIntelligentPointerThing& other);   // 0x007C2930
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointerThing& pointer);   // 0x004CC340
inline bool IsTargetAlive(const CIntelligentPointerThing& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CTCStealth { public:                             // interface 0x36
    void SetAsInDanger(CThing* source, long status);   // 0x00747B80
};
class CTCCarrying { public:                            // interface 0x46
    CThing* GetThingInPrimarySlot();            // 0x006A1440
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
};
class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _combat[0x6c];
    virtual ~CCombatActionBase();                       // 0x0062E900
};
class CCreatureAction_SummonerSheatheWeapon : public CCombatActionBase { public:
    unsigned char Flag;                         // +0x114
    CCreatureAction_SummonerSheatheWeapon(CThingCreatureBase* owner);   // 0x008CBF00
};
class CCreatureAction_SummonerLightningOrbInto : public CCombatActionBase { public:
    CBaseIntelligentPointer PTarget;            // +0x114
    CCreatureAction_SummonerLightningOrbInto(CThingCreatureBase* owner, CThing* target);   // 0x008CBD90
    ~CCreatureAction_SummonerLightningOrbInto();   // 0x00719C30
};
class CCreatureAction_SummonerLightningOrbOutOfGroundSmash : public CCombatActionBase { public:
    CCreatureAction_SummonerLightningOrbOutOfGroundSmash(CThingCreatureBase* owner);   // 0x00970240
};
class CWorld { public:
    static long GetWorldFrame();                // 0x0049D870
};
extern long g_FramesPerSecond;                  // 0x01375550
class CSummonerDef { public:
    unsigned char _pad[0x38];
    float LightningPreemptiveDelay;             // +0x38
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0
    void Terminate();                           // 0x008FCF40
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long frames, float speed);   // 0x0097D6F0
    long AIAction_CombatPerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DA40
};
struct CAIStateGroup_SummonerLightningAttack : CAIStateGroupBase {
    CIntelligentPointerThing PTarget;           // +0x14
    const CSummonerDef* PDef;                   // +0x1c CDefPointer<const CSummonerDef>
    long NextFrameForPreemptiveUse;             // +0x20
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    virtual void Update();
};

void CAIStateGroup_SummonerLightningAttack::Update()
{
    if (IsTargetAlive(PTarget) && !(GetCreature()->CreatureFlags1c8 & 2)) {
        CTCStealth* stealth;
        if (PTarget.GetPItem()->GetInterfaceX(&stealth, 0x36))
            stealth->SetAsInDanger(GetCreature(), 4);
        CTCCarrying* carrying;
        if (((const CThing*)GetCreature())->GetInterfaceC(&carrying, 0x46)) {
            for (;;) {
                CThing* weapon = carrying->GetThingInPrimarySlot();
                if (!weapon)
                    break;
                if (weapon->m_flags91 & 1)
                    continue;
                long result = AIAction_CombatPerformActionAndWait(&CCreatureAction_SummonerSheatheWeapon(GetCreature()), false);
                if (result != 1)
                    continue;
                Terminate();
                return;
            }
        }
        if (IsTargetAlive(PTarget)) {
            if (AIAction_LookAtThing(1, PTarget, 1, 0.013888889f) != 1) {
                if (IS_THING_ALIVE(PTarget)) {
                    CThingCreatureBase* creature = GetCreature();
                    long result = AIAction_CombatPerformActionAndWait(&CCreatureAction_SummonerLightningOrbInto(creature, PTarget.GetPItem()), false);
                    if (result == 1) {
                        Terminate();
                        return;
                    }
                }
                while (GetCreature()->IsPerformingAnyAction()) {
                    if (AIAction_DoNothing() == 1) {
                        Terminate();
                        return;
                    }
                }
                long result = AIAction_CombatPerformActionAndWait(&CCreatureAction_SummonerLightningOrbOutOfGroundSmash(GetCreature()), false);
                if (result != 1) {
                    float delay = PDef->LightningPreemptiveDelay;
                    CWorld* world = GetPWorld();
                    long fps = g_FramesPerSecond;
                    NextFrameForPreemptiveUse = (long)((float)world->GetWorldFrame() + (float)fps * delay);
                }
            }
        }
    }
    Terminate();
}