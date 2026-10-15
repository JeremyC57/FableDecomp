// NCreatureMode::CModeCarryingStandBase::GetMovementSpeed(long, long, long movementType) -- the mode's walk (+0x30) or run (+0x28) ceiling for movement types 3 and 5, 0.0 for anything else or when the thing (+4) is an AI creature (type byte +0x10 == 2)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00861900, length 45 bytes.
// No callees. Vtable slot 7 of CModeCarryingStandBase (rtti-vtables.tsv; the chunk label CModeStandBase::GetMovementSpeed names the base whose layout engine/CModeStandBase.h gives: MaxRunSpeed +0x28, MaxWalkSpeed +0x30). Shape: 0.0f (0x0122DEDC) loaded first and dropped (`fstp st(0)`) on the two member paths, the subtract chain of the type compare, `ret 0Ch`.
namespace NCreatureMode {
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char m_type;                          // +0x10 (2 = an AI creature)
};

struct CModeBase {                                 // CTCCreatureModeManager::CModeBase, vtable 0x012673CC
    virtual ~CModeBase();
    CThing* m_pThing;                              // +0x04
};

struct CModeStandBase : CModeBase {                // engine/CModeStandBase.h offsets
    unsigned char _pad08[0x14];                    // +0x08 .. +0x1c
    long          IdleCounter;                     // +0x1c
    long          FramesToKeepUpdatingMovement;    // +0x20
    bool          ForceAddNewAnims;                // +0x24
    float         MaxRunSpeed;                     // +0x28
    float         MaxJogSpeed;                     // +0x2c
    float         MaxWalkSpeed;                    // +0x30
};

struct CModeCarryingStandBase : CModeStandBase {
    virtual float GetMovementSpeed(long a, long b, long movementType);
};

float CModeCarryingStandBase::GetMovementSpeed(long a, long b, long movementType)
{
    float speed = 0.0f;
    if (m_pThing->m_type != 2) {
        switch (movementType) {
        case 3:
            speed = MaxWalkSpeed;
            break;
        case 5:
            speed = MaxRunSpeed;
            break;
        }
    }
    return speed;
}
}