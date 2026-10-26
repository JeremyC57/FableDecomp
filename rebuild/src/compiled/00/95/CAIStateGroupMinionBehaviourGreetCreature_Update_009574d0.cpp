// CAIStateGroup_MinionBehaviourGreetCreature::Update() -- while the creature to greet is alive, turn to face it;
// once facing, play the SG_CHICKEN_WANDER animation and wait for it; finish the update either way.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009574D0, length 239 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (IS_THING_ALIVE, twice), CAIStateGroupBase::AIAction_LookAtThing
// @ 0x0097D6F0 (repeat 1, by-value CIntelligentPointer copy built in its argument slot: ctor @ 0x00A01B10, vtable
// 0x01237F1C, SetPItem @ 0x00A01B90; flags 0xB, turn speed 1/72), CCharString(const char*, long) @ 0x0099EBF0
// ("SG_CHICKEN_WANDER" @ 0x012593EC), CCreatureAction_PlayAnimation(creature, anim, float, sound, ...) @ 0x00843A80,
// CAIStateGroupBase::AIAction_PerformActionAndWait @ 0x0097DB70, ~CCreatureActionBase @ 0x00693EF0, ~CCharString
// @ 0x0099EAE0, the state-group finish call @ 0x008FCF40 (manifest "CXMVPlaybackInfo" is propagated and wrong).
// Shape: a NAMED CCharString in the inner block; the action result is a named long (compared after the action dtor); `if (result != 2) { FinishUpdate(); return; }` inside the block
// (FinishUpdate before the string dtor on that path), the common FinishUpdate after it. No engine header.
class CThing;
class CThingCreatureBase;

struct CBaseIntelligentPointer {
    // (the dead bit lives at +0x91 of the thing)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // vtable 0x01237F1C
    CIntelligentPointerConstThing(const CIntelligentPointerConstThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];                      // +0x04..+0xa7
    ~CCreatureActionBase();                         // 0x00693EF0
};

class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];                      // +0xa8..+0xb3
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation, float blend,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x00843A80
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                  // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad[0x0c];                       // +0x08..+0x13
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float turnSpeed);   // 0x0097D6F0
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool flag);   // 0x0097DB70 (EAIActionReturn)
    void FinishUpdate();                            // 0x008FCF40
};

struct CThingAlive { unsigned char _pad[0x91]; unsigned char m_flags91; };

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(((CThingAlive*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CAIStateGroup_MinionBehaviourGreetCreature : CAIStateGroupBase {
    CIntelligentPointerConstThing PCreatureToGreet; // +0x14
    void Update();
};

void CAIStateGroup_MinionBehaviourGreetCreature::Update()
{
    if (IS_THING_ALIVE(PCreatureToGreet) && AIAction_LookAtThing(1, PCreatureToGreet, 0xb, 0.013888889f) == 2) {
        CCharString animation("SG_CHICKEN_WANDER");
        long result = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), animation, 0.0f,
                CSoundPair(), false, true, false, 0, 0), false);
        if (result != 2) {
            FinishUpdate();
            return;
        }
    }
    FinishUpdate();
}