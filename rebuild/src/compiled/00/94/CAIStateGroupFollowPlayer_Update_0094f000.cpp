// CAIStateGroup_FollowPlayer::Update() -- if the followed thing is alive, keeps moving to within FollowDistance of it (idling a tick after each arrival) until a move does not succeed, then terminates
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094F000, length 125 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem
// @ 0x00A01B90 (the by-value argument copy), CAIStateGroupBase::AIAction_MoveToThingProximity @ 0x0097E380 (ten
// arguments), CAIStateGroupBase::AIAction_DoNothing @ 0x0097E200, CAIStateGroupBase::Terminate @ 0x008FCF40.
// Shape: `if (alive) { while (MoveToThingProximity(..) == 2) DoNothing(); } Terminate();` (the alive test is NOT
// repeated: `jmp` back to the argument pushes; a `while (alive)` loop is rotated by the compiler) -- the followed
// pointer's address formed once into ebx; the move arguments right to left: 0.0f, the four flags, movement type 5,
// the address of an uninitialised `bool` local (high byte of the `push ecx` slot), the follow distance float
// member (+0x1c, loaded as a dword before the constant pushes), the by-value CIntelligentPointer<const CThing>
// converted from the member (inline converting copy constructor, vtable 0x01237F1C), repeat 1.
// Model: local (engine/CAIStateGroup_FollowPlayer.h flattens the base).
struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                  // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {          // CIntelligentPointer<CThing>
    virtual void v00();
};

struct CIntelligentPointerConstThing : CBaseIntelligentPointer {     // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

enum EAIActionRepeat { AI_ACTION_REPEAT_NONE, AI_ACTION_REPEAT_ONCE };
enum EAIActionReturn { AI_ACTION_RETURN_NONE, AI_ACTION_RETURN_1, AI_ACTION_RETURN_SUCCEEDED };
enum EMovementType { MOVEMENT_TYPE_0, MOVEMENT_TYPE_1, MOVEMENT_TYPE_2, MOVEMENT_TYPE_3, MOVEMENT_TYPE_4, MOVEMENT_TYPE_FOLLOW };

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    void* PBrain;                                   // +0x04
    long ScriptAIPriority;                          // +0x08
    void* Name_PStringData;                         // +0x0c
    unsigned char Terminated;                       // +0x10
    unsigned char Priority;                         // +0x11
    unsigned char Enabled;                          // +0x12
    unsigned char _pad_0x13;
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat, CIntelligentPointerConstThing target, float proximity,
        bool* pArrived, EMovementType movement, bool a, bool b, bool c, bool d, float e);    // 0x0097E380
    long AIAction_DoNothing();                      // 0x0097E200
    void Terminate();                               // 0x008FCF40
};

struct CAIStateGroup_FollowPlayer : CAIStateGroupBase {
    CIntelligentPointerThing PThingToFollow;        // +0x14
    float FollowDistance;                           // +0x1c
    virtual void Update();
};

void CAIStateGroup_FollowPlayer::Update()
{
    if (PThingToFollow.GetPItem() != 0 && !(PThingToFollow.GetPItem()->m_flags91 & 1)) {
        bool arrived;
        while (AIAction_MoveToThingProximity(AI_ACTION_REPEAT_ONCE, PThingToFollow, FollowDistance, &arrived,
                MOVEMENT_TYPE_FOLLOW, true, false, true, true, 0.0f) == AI_ACTION_RETURN_SUCCEEDED)
            AIAction_DoNothing();
    }
    Terminate();
}