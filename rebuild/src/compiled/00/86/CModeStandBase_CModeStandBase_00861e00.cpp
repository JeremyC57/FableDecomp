// CModeStandBase::CModeStandBase(CThing* parent) -- creature stand/locomotion mode: base mode, zeroed counters and speeds, empty anims
// Leaf: CModeStandBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00861E00, length 91 bytes (+5 int3). Vtable 0x01276D8C (RTTI CModeStandBase);
// the manifest label "CTCCreatureNavigation::CTCCreatureNavigation" is propagated.
// Callees: CTCCreatureModeManager::CModeBase::CModeBase(CThing*) @ 0x0073EAC0, CCharString::CCharString() @ 0x0099E4B0.
// Model: the PDB members of CModeStandBase (engine/CModeStandBase.h, sizeof 0x68, offsets identical in retail) as a local
// polymorphic class -- the generated header is a flat POD and cannot carry the base constructor and vtable. The six
// CCountedPointer<CAnimEntryLite> members are {Data, Info} pairs with an inline zeroing constructor.
// Shape: base constructor, then the derived vtable store, the scalar initialisers from one zero register (ebx, the bool
// as bl), the PoseAnimName constructor, then the twelve counted-pointer words.
struct CThing;

struct CModeBase {                          // CTCCreatureModeManager::CModeBase, sizeof 0x1c
    CModeBase(CThing* parent);              // 0x0073EAC0
    virtual void v00();
    CThing* ParentThing;                    // +0x04
    unsigned char WeaponBasedFilter[0x0c];  // +0x08 CAnimFilterOfGroup
    long FramesBeforeRemovingSelf;          // +0x14
    bool Active;                            // +0x18
};

struct CCharString {
    void* PStringData;                      // +0x00
    CCharString();                          // 0x0099E4B0
};

struct CAnimEntryPointer {                  // CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>
    void* Data;                             // +0x00
    void* Info;                             // +0x04
    CAnimEntryPointer() : Data(0), Info(0) { }
};

struct CModeStandBase : CModeBase {
    long IdleCounter;                       // +0x1c
    long FramesToKeepUpdatingMovement;      // +0x20
    bool ForceAddNewAnims;                  // +0x24
    float MaxRunSpeed;                      // +0x28
    float MaxJogSpeed;                      // +0x2c
    float MaxWalkSpeed;                     // +0x30
    CCharString PoseAnimName;               // +0x34
    CAnimEntryPointer PStandAnimation;      // +0x38
    CAnimEntryPointer PSlowWalkAnimation;   // +0x40
    CAnimEntryPointer PWalkAnimation;       // +0x48
    CAnimEntryPointer PJogAnimation;        // +0x50
    CAnimEntryPointer PRunAnimation;        // +0x58
    CAnimEntryPointer PSprintAnimation;     // +0x60
    CModeStandBase(CThing* parent);
    virtual void v00();
};

CModeStandBase::CModeStandBase(CThing* parent)
    : CModeBase(parent), IdleCounter(0), FramesToKeepUpdatingMovement(0), ForceAddNewAnims(false),
      MaxRunSpeed(0.0f), MaxJogSpeed(0.0f), MaxWalkSpeed(0.0f)
{
}