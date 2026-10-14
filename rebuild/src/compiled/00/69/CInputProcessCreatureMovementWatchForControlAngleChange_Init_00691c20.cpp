// CInputProcessCreatureMovementWatchForControlAngleChange::Init() -- reset LastJoyDirection (+0x28, a C2DVector) to the zero vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00691C20, length 35 bytes (+ int3 padding). Vtable 0x01237F74 slot 2. No callees.
// Layout from Ego_r.pdb (48 bytes): CInputProcessGameBase (0x28) then C2DVector LastJoyDirection +0x28.
// Shape: the zero vector is built as a temporary in an 8-byte frame (two immediate stores) and copied into the member through
// eax/edx -- an inline C2DVector(float, float) constructor assigned to the member.
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct CInputProcessCreatureMovementWatchForControlAngleChange {
    virtual void v00(); virtual void v01();
    virtual void Init();                            // slot 2
    unsigned char _pad04[0x24];                     // +0x04 CAInputProcess and the game-base pointers
    C2DVector     LastJoyDirection;                 // +0x28
};

void CInputProcessCreatureMovementWatchForControlAngleChange::Init()
{
    LastJoyDirection = C2DVector(0.0f, 0.0f);
}