// CInputProcessJumpingAndRolling::Init() -- reset the pulse timers (+0x30/+0x34), the pulse vector (+0x28, the zero C2DVector), StickPulled (+0x4c) and WFLastRoll (+0x50, -1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A4010, length 40 bytes (+ int3 padding). Vtable 0x01238050 slot 2. No callees.
// Layout from Ego_r.pdb CInputProcessJumpingAndRolling (0x64 there, 0x54 in retail: the two std::lists are 4-byte STLport lists, not 12):
// PulseVector +0x28, PulseStartTimer +0x30, PulseEndTimer +0x34, Pulses +0x38, InitialFacingDirections +0x3c, FacingDirection +0x40,
// StickPulled +0x4c, WFLastRoll +0x50.
// Shape: the zero vector is an inline C2DVector(0, 0) temporary in an 8-byte frame (its stores are kept, the loads forwarded from the
// zero register through edx), the timers from the zero register, the byte from al, the -1 an immediate.
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class C3DVector { public: float X, Y, Z; };
class CInputProcessJumpingAndRolling { public:             // vtable 0x01238050
    virtual void v00(); virtual void v01();
    virtual void Init();                            // slot 2
    unsigned char _pad04[0x24];                     // +0x04 CAInputProcess and the game-base pointers
    C2DVector     PulseVector;                      // +0x28
    long          PulseStartTimer;                  // +0x30
    long          PulseEndTimer;                    // +0x34
    void*         Pulses;                           // +0x38 list<pair<C2DVector,long>> (STLport, one node pointer)
    void*         InitialFacingDirections;          // +0x3c list<C3DVector>
    C3DVector     FacingDirection;                  // +0x40
    bool          StickPulled;                      // +0x4c
    long          WFLastRoll;                       // +0x50
};

void CInputProcessJumpingAndRolling::Init()
{
    PulseStartTimer = 0;
    PulseEndTimer = 0;
    PulseVector = C2DVector(0.0f, 0.0f);
    StickPulled = false;
    WFLastRoll = -1;
}