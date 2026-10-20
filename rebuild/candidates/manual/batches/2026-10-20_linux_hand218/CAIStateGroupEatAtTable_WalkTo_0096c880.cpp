// CAIStateGroup_EatAtTable::WalkTo(C3DVector pos, CRightHandedSet axes) -- walk to the seat (running when far and in the right mode), then look along its facing; true when both actions complete
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096C880, 277 bytes (`ret 24h`). Ego_r.pdb: ?WalkTo@CAIStateGroup_EatAtTable@@IAE_NVC3DVector@@VCRightHandedSet@@@Z.
// Callees: the creature's movement-mode getter 0x00662000 (manifest "CPixelShader::CountReferences" is propagated),
// CThing::GetPosition 0x004C73D0, AIAction_MoveToPos 0x0097EC00 (repeat 2, C3DVector by value, &reached, movement type,
// true, true, no leader, true, 0.0f), AIAction_LookAtPos 0x0097D580 (repeat 2, C3DVector by value, 0x11, 1/360);
// 8.0f pool 0x0125916C.
// Shape: movement type 3, or 5 when the creature's mode is 1 and the seat is more than 8 units away (`(pos -
// position).GetLength()` with the difference built Z first by the value ctor); `do ... while (r == 4)` re-issues both
// actions; the look target is three named float sums `pos.X + axes.Forward.X` (VC loads the right operand first;
// the Z sum is homed in the dead `pos.X` argument slot) passed as `C3DVector(x, y, z)` (a C3DVector local takes three
// frame words); the turn speed literal is 0.0027777f (not 1.0f/360.0f); the look-at runs twice; `return r == 2;` (full-register bool).
// Leaf: WalkTo@CAIStateGroup_EatAtTable
#include <math.h>
struct CThing;

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    void operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};

struct CRightHandedSet {
    C3DVector Up;                                  // +0x00
    C3DVector Forward;                             // +0x0c
};

struct CThingCreatureBase {
    long GetMovementMode();                        // 0x00662000
    const C3DVector& GetPosition() const;          // 0x004C73D0
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                 // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                              // +0x04
    unsigned char _pad[0x0c];                      // +0x08..+0x13
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movementType, bool a, bool b,
                            CThing* leader, bool c, float d);   // 0x0097EC00
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float turnSpeed);   // 0x0097D580
};

struct CAIStateGroup_EatAtTable : CAIStateGroupBase {
    bool WalkTo(C3DVector pos, CRightHandedSet axes);
};

bool CAIStateGroup_EatAtTable::WalkTo(C3DVector pos, CRightHandedSet axes)
{
    long movementType = 3;
    if (PBrain->PCreature->GetMovementMode() == 1 && (pos - PBrain->PCreature->GetPosition()).GetLength() > 8.0f)
        movementType = 5;
    bool reached;
    long result;
    do {
        result = AIAction_MoveToPos(2, pos, &reached, movementType, true, true, 0, true, 0.0f);
    } while (result == 4);
    if (result != 2)
        return false;
    float x = pos.X + axes.Forward.X;
    float y = pos.Y + axes.Forward.Y;
    float z = pos.Z + axes.Forward.Z;
    long i = 0;
    do {
        result = AIAction_LookAtPos(2, C3DVector(x, y, z), 0x11, 0.0027777f);
    } while (result == 4 || ++i < 2);
    return result == 2;
}
