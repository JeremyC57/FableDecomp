// CCard::Move(const C3DVector& position, bool immediately) -- send a tavern-game card to a new local position: jump there at once, or start (or restart from the current point) an interpolated move
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F0660, length 144 bytes (ret 8). No callees.
// Layout (Ego_r.pdb CCard, 88 bytes, byte-packed vectors): MoveFraction +0x20, Moving +0x26, LocalPos +0x27,
// NewLocalPos +0x33, CurrentPos +0x3f, Width +0x4c (the landed CCard::Turn @ 0x008F0620 uses the same members).
// The three C3DVector members sit at odd offsets, so the class is modelled under #pragma pack(1) exactly as the
// PDB lays it out.
// Shape: `immediately` copies the target into LocalPos and NewLocalPos and returns. Otherwise a move already in
// progress restarts from CurrentPos (LocalPos = CurrentPos), a fresh one records LocalPos as CurrentPos and sets
// Moving; then NewLocalPos takes the target and MoveFraction restarts at 0 (an immediate dword store of 0.0f).
// The vector copies are plain 12-byte POD assignments.
#pragma pack(push, 1)
class C3DVector { public:
    float X, Y, Z;
};
class CCard {
public:
    void*     Thing;                          // +0x00 CThingObject*
    long      Name;                           // +0x04
    long      Value;                          // +0x08
    float     InitialRotation;                // +0x0c
    float     FinalRotation;                  // +0x10
    float     TurnSpeed;                      // +0x14
    float     TurnFraction;                   // +0x18
    float     MoveSpeed;                      // +0x1c
    float     MoveFraction;                   // +0x20
    bool      Turning;                        // +0x24
    bool      FaceDown;                       // +0x25
    bool      Moving;                         // +0x26
    C3DVector LocalPos;                       // +0x27
    C3DVector NewLocalPos;                    // +0x33
    C3DVector CurrentPos;                     // +0x3f
    unsigned char _pad4b;                     // +0x4b
    float     Width;                          // +0x4c
    float     Height;                         // +0x50
    long      Index;                          // +0x54
    void Move(const C3DVector& position, bool immediately);
};
#pragma pack(pop)

void CCard::Move(const C3DVector& position, bool immediately)
{
    if (immediately) {
        LocalPos = position;
        NewLocalPos = position;
        return;
    }
    if (Moving) {
        LocalPos = CurrentPos;
    } else {
        CurrentPos = LocalPos;
        Moving = true;
    }
    NewLocalPos = position;
    MoveFraction = 0.0f;
}