// CCard::Turn() -- starts a card flip: Turning set, TurnFraction reset, the rotation pair chosen from the
// current face (down: 0.5 -> 0, up: 0 -> 0.5) and FaceDown toggled.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F0620, length 54 bytes.
// Callees: none.
// PDB CCard: +0x0C InitialRotation, +0x10 FinalRotation, +0x18 TurnFraction, +0x24 Turning, +0x25 FaceDown.
// The old FaceDown is read once into a register (`cmp al,1` is the `== true` test, the `cmp al,dl ; sete`
// is the toggle computed from that same byte).
struct CCard {
    void*   Thing;                               // +0x00
    long    Name;                                // +0x04
    long    Value;                               // +0x08
    float   InitialRotation;                     // +0x0C
    float   FinalRotation;                       // +0x10
    float   TurnSpeed;                           // +0x14
    float   TurnFraction;                        // +0x18
    float   MoveSpeed;                           // +0x1C
    float   MoveFraction;                        // +0x20
    bool    Turning;                             // +0x24
    bool    FaceDown;                            // +0x25
    bool    Moving;                              // +0x26
    void Turn();
};

void CCard::Turn()
{
    bool faceDown = FaceDown;
    Turning = true;
    TurnFraction = 0.0f;
    if (faceDown == true) {
        InitialRotation = 0.5f;
        FinalRotation = 0.0f;
    } else {
        InitialRotation = 0.0f;
        FinalRotation = 0.5f;
    }
    FaceDown = !faceDown;
}
