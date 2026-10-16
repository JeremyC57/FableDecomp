// CAIStateGroup_MinionReactToBurglary::Update() -- turn to face the burglar; once facing, play the ST_SURPRISED
// animation (sound pair 0x2B); then hand control back (0x008FCF40).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00958000, length 189 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::AIAction_LookAtThing @ 0x0097D6F0 (repeat 1, the by-value CIntelligentPointer
// copy built in its argument slot: ctor @ 0x00A01B10, vtable 0x01237F1C, SetPItem(GetPItem()) @ 0x00A01B90/0x00A01B50,
// flags 0x23, turn speed 1/72), CCharString(const char*, long) @ 0x0099EBF0 ("ST_SURPRISED" @ 0x01294E38),
// CCreatureAction_PlayAnimation ctor @ 0x008424F0, CAIStateGroupBase::AIAction_PerformAction @ 0x0097D8B0,
// ~CCreatureActionBase @ 0x00693EF0, ~CCharString @ 0x0099EAE0, the state-group finish call @ 0x008FCF40
// (manifest "CXMVPlaybackInfo" is propagated and wrong).
// Shape: the `== 2 && PerformAction(&temporary...)` full expression keeps the temporaries' construction flags in ebx
// (`mov ebx,3`, `test bl,2` / `test bl,1` before each destructor); the animation name is an IMPLICIT CCharString
// conversion temporary (a literal argument), which VC7.1 builds before every other argument. No engine header.
struct CThing;
struct CThingCreatureBase;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // vtable 0x01237F1C
    CIntelligentPointerConstThing(const CIntelligentPointerConstThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair(long first, long second) : m_first(first), m_second(second) { }
};

struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];                      // +0x04..+0xa7
    ~CCreatureActionBase();                         // 0x00693EF0
};

struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _anim[0x0c];                      // +0xa8..+0xb3
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                  // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad[0x0c];                       // +0x08..+0x13
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float turnSpeed);   // 0x0097D6F0
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0 (EAIActionReturn)
    void FinishUpdate();                            // 0x008FCF40
};

struct CAIStateGroup_MinionReactToBurglary : CAIStateGroupBase {
    CIntelligentPointerConstThing PBurglar;         // +0x14
    void Update();
};

void CAIStateGroup_MinionReactToBurglary::Update()
{
    AIAction_LookAtThing(1, PBurglar, 0x23, 0.013888889f) == 2 &&
        AIAction_PerformAction(&CCreatureAction_PlayAnimation(GetCreature(), "ST_SURPRISED",
            CSoundPair(0x2b, 0x2b), false, true, false, 0, 0));
    FinishUpdate();
}
