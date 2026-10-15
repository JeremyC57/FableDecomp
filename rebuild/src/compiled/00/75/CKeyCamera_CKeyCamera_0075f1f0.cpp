// CKeyCamera::CKeyCamera(const CKeyCamera& other) -- copy constructor of a camera key frame: default-construct the event name, then copy every field (the name through CCharString::operator=)
// Leaf: ??0CKeyCamera
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075F1F0, length 122 bytes.
// Callees: CCharString::CCharString() @ 0x0099E4B0, CCharString::operator=(const CCharString&) @ 0x0099EFB0.
// Model: CKeyCamera from Ego_r.pdb (56 bytes): DeltaIndex +0, Pos +4, LookDirection +0x10, FOV +0x1c, Roll +0x20,
// ShuttleSpeed +0x24, Duration +0x28, PauseTime +0x2c, EventName +0x30 (CCharString), SpeedFraction +0x34.
// Shape: a user copy constructor whose body assigns in retail's store order (the two vectors as struct copies through
// re-based pointers, the scalars, the name, then Roll, SpeedFraction and DeltaIndex); EventName's default constructor
// runs first as the only member with one.
struct CCharString {
    void* PStringData;
    CCharString();                                 // 0x0099E4B0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct C3DVector {
    float X, Y, Z;
};
struct CKeyCamera {
    float       DeltaIndex;                        // +0x00
    C3DVector   Pos;                               // +0x04
    C3DVector   LookDirection;                     // +0x10
    float       FOV;                               // +0x1c
    float       Roll;                              // +0x20
    float       ShuttleSpeed;                      // +0x24
    float       Duration;                          // +0x28
    float       PauseTime;                         // +0x2c
    CCharString EventName;                         // +0x30
    float       SpeedFraction;                     // +0x34
    CKeyCamera(const CKeyCamera& other);
};

CKeyCamera::CKeyCamera(const CKeyCamera& other)
{
    Pos = other.Pos;
    LookDirection = other.LookDirection;
    FOV = other.FOV;
    ShuttleSpeed = other.ShuttleSpeed;
    Duration = other.Duration;
    PauseTime = other.PauseTime;
    EventName = other.EventName;
    Roll = other.Roll;
    SpeedFraction = other.SpeedFraction;
    DeltaIndex = other.DeltaIndex;
}