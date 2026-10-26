// CKeyCamera::operator=(const CKeyCamera& other) -- assignment of a 0x38-byte scripted-camera key (floats and two vectors copied, the event name through CCharString::operator=)
// Leaf: ??4CKeyCamera
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075EF00, length 111 bytes.
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0 (EventName at +0x30).
// Label: the manifest's "CEnginePrimitive2DText::operator=" is propagated; the callers are the CKeyCamera vector helpers
// (_Copy_backward @ 0x0075F0E0 steps 0x38 bytes, the insert @ 0x0075FAE0 -- the landed _Uninit_copy<CKeyCamera*> @ 0x0075F3E0
// names the element), and the PDB CKeyCamera is exactly 56 bytes with this layout.
// Shape: the memberwise copy written out in the body (VC7.1 out-lines a record whose member has its own assignment): the
// floats as dwords, each C3DVector (a plain POD) as a nested record copy (`lea` of both sides + three moves), the
// CCharString through its operator=, SpeedFraction last; returns *this.
class CCharString { public:
    void* PStringData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class C3DVector { public:
    float X, Y, Z;
};

struct CKeyCamera {
    float       DeltaIndex;                        // +0x00
    C3DVector   Pos;                               // +0x04
    C3DVector   LookDirection;                     // +0x10
    float       FOV;                               // +0x1C
    float       Roll;                              // +0x20
    float       ShuttleSpeed;                      // +0x24
    float       Duration;                          // +0x28
    float       PauseTime;                         // +0x2C
    CCharString EventName;                         // +0x30
    float       SpeedFraction;                     // +0x34
    CKeyCamera& operator=(const CKeyCamera& other);
};

CKeyCamera& CKeyCamera::operator=(const CKeyCamera& other)
{
    DeltaIndex = other.DeltaIndex;
    Pos = other.Pos;
    LookDirection = other.LookDirection;
    FOV = other.FOV;
    Roll = other.Roll;
    ShuttleSpeed = other.ShuttleSpeed;
    Duration = other.Duration;
    PauseTime = other.PauseTime;
    EventName = other.EventName;
    SpeedFraction = other.SpeedFraction;
    return *this;
}