// CAIStateGroup_JackDragon::StayIdle(float seconds) -- idle frame by frame for `seconds`; terminate the group as soon as the enemy dies or the group stops
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094FE30, length 126 bytes (+ int3 padding).
// Callees: the world accessor @ 0x008FCFC0 (PBrain->+0x1c->+0x24), CWorld::GetWorldFrame @ 0x0049D870 (a
// static returning the frame global @ 0x013B89BC; manifest "GFGetPreMainMemoryUsed" is propagated),
// CAIStateGroupBase::AIAction_DoNothing @ 0x0097E200, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CAIStateGroupBase::IsStopping @ 0x008FD010, CAIStateGroupBase::Terminate @ 0x008FCF40.
// Globals: the frames-per-second long @ 0x01375550 (`fidiv`).
//
// Shape: `GetPWorld()->GetWorldFrame()` still calls the accessor (static member through an object
// expression); the start frame lives in ebp, &PEnemy (+0x14) in edi (hoisted above the loop). A do/while:
// DoNothing, then the inlined IS_THING_ALIVE(PEnemy) / IsStopping exits sharing one Terminate tail, then
// `(float)(frame - start) / g_framesPerSecond < seconds` (`fild ; fidiv ; fcomp [arg] ; test ah,5 ; jnp`,
// the int difference parked in the `push ecx` slot). No engine header for the class: local model.
extern long g_framesPerSecond;                  // 0x01375550

struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    void* PBrain;                               // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
    CWorld* GetPWorld();                        // 0x008FCFC0
    long AIAction_DoNothing();                  // 0x0097E200
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_JackDragon : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    void StayIdle(float seconds);
};

void CAIStateGroup_JackDragon::StayIdle(float seconds)
{
    long start = GetPWorld()->GetWorldFrame();
    do {
        AIAction_DoNothing();
        if (!IS_THING_ALIVE(PEnemy) || IsStopping()) {
            Terminate();
            return;
        }
    } while ((float)(GetPWorld()->GetWorldFrame() - start) / g_framesPerSecond < seconds);
}
