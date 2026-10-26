// CTCRotate::CTCRotate(CThing*) -- thing component that spins its owner: CTCBase(parent), rotation axis left unset, speed 0, frame counters 0, flags cleared, an empty name and an index of -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D2B2A, length 57 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (the manifest label "CEngineSubPrimitive2DClockSprite" is propagated; landed
// as the CTCBase constructor in 0x007509A0), CCharString::CCharString() @ 0x0099E4B0. Vtable 0x0123A024 = CTCRotate.
// Layout from CTCRotate::SetRotationInfo (0x0072A530): axis C3DVector +0xc (no constructor code), rotation speed float
// +0x18, frame count +0x24 (seconds * frames-per-second, through __ftol2).
// Shape: a size-optimised TU (`push dword ptr [esp+8]` pass-through, `and dword ptr [..],0` zero stores, `or [..],-1`) =
// `#pragma optimize("s", on)`; the scalar initialisers ahead of the CCharString member are scheduled around the vtable store.
#pragma optimize("s", on)
class CThing;

class CCharString { public:
    void* m_pData;
    CCharString();                           // 0x0099E4B0
};

class CTCBase { public:
    virtual void v00();
    unsigned char _body[0x8];                // ParentThing +4, flags +8/+9
    CTCBase(CThing* parent);                 // 0x00686800
};

class CTCRotate : public CTCBase { public:
    float         m_axis[3];                 // +0x0c C3DVector (not initialised)
    float         m_speed;                   // +0x18
    long          m_unknown1c;               // +0x1c (not initialised)
    long          m_framesElapsed;           // +0x20
    long          m_framesTotal;             // +0x24
    bool          m_finished;                // +0x28
    CCharString   m_name;                    // +0x2c
    long          m_index;                   // +0x30
    bool          m_active;                  // +0x34
    virtual void v00();
    CTCRotate(CThing* parent);
};

CTCRotate::CTCRotate(CThing* parent)
    : CTCBase(parent)
    , m_speed(0.0f)
    , m_framesElapsed(0)
    , m_framesTotal(0)
    , m_finished(false)
    , m_name()
    , m_index(-1)
    , m_active(false)
{
}