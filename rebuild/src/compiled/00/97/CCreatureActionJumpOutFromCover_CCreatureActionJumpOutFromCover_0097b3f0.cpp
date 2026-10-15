// CCreatureAction_JumpOutFromCover::CCreatureAction_JumpOutFromCover(CThingCreatureBase*, CThing*, const C3DVector&) -- base CCombatActionBase(owner, thing, 150, GetSoundPair(owner, "SWORD_SWING")), then the landing position copied in
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097B3F0, length 117 bytes. Vtable 0x0129888C.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetSoundPair @ 0x008561A0 (`__fastcall`, hidden-pointer CSoundPair),
// CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the landed two-argument combat-action family (0x008C4F50); the vtable store precedes the three dword copies of the position
// (+0x114..+0x11c, alternating edx/ecx straight off the argument pointer): three scalar members initialised from the vector's
// components (a struct-typed member copies through a `lea` of the member instead); dwords, so no FPU is involved.
struct CThingCreatureBase;
struct CThing;

struct CCharString {                        // one pointer; the default length makes a literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};
struct CSoundPair {                         // a user constructor makes the 8-byte result return through the hidden pointer
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
CSoundPair __fastcall GetSoundPair(CThingCreatureBase* owner, const CCharString& name);   // 0x008561A0

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);                  // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);   // 0x00857F30
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};
struct C3DVector { long X, Y, Z; };         // floats in the engine; copied as dwords here
struct CCreatureAction_JumpOutFromCover : CCombatActionBase {   // vtable 0x0129888C
    long m_destinationX;                    // +0x114 where the jump lands (descriptive; floats copied as dwords)
    long m_destinationY;                    // +0x118
    long m_destinationZ;                    // +0x11c
    virtual void v00();
    CCreatureAction_JumpOutFromCover(CThingCreatureBase* owner, CThing* thing, const C3DVector& destination);
};

CCreatureAction_JumpOutFromCover::CCreatureAction_JumpOutFromCover(CThingCreatureBase* owner, CThing* thing, const C3DVector& destination)
    : CCombatActionBase(owner, thing, 150, GetSoundPair(owner, "SWORD_SWING"))
    , m_destinationX(destination.X)
    , m_destinationY(destination.Y)
    , m_destinationZ(destination.Z)
{
}