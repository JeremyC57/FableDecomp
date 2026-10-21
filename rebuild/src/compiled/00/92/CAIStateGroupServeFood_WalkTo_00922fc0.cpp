// CAIStateGroup_ServeFood::WalkTo(C3DVector, C3DVector, C3DVector) -- walk to the position (repeating while the action returns 4); on arrival turn to look at position + look offset; report whether the look completed.
// Leaf: WalkTo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00922FC0, length 175 bytes (+ int3 padding), `ret 24h` (three by-value
// C3DVectors; the middle one is unused here).
// Callees: CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00 (repeat 2, pos, &reached, movement 3, true, true, 0,
// true, 0.0), CAIStateGroupBase::AIAction_LookAtPos @ 0x0097D580 (repeat 2, by-value C3DVector, flags 0x11, speed
// 0.0027777f).
// Shape: the landed do/while action loops; the look-at point is three named float sums (x, y, z in source order) fed to
// the value constructor, so VC7.1 keeps X/Y in the 0xC frame and homes Z in the dead pos.X argument slot as retail (an
// operator+ through the value constructor evaluates z first; a field-by-field sum needs a 0x10 frame);
// `return result == 2;` (`sete cl`). No engine header for this class.
struct CThing;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // +0x04..+0x13
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movementType, bool a, bool b,
                            CThing* leader, bool c, float d);   // 0x0097EC00
};

struct CAIStateGroup_ServeFood : CAIStateGroupBase {
    bool WalkTo(C3DVector pos, C3DVector facing, C3DVector lookOffset);
};

bool CAIStateGroup_ServeFood::WalkTo(C3DVector pos, C3DVector facing, C3DVector lookOffset)
{
    bool reached;
    long result;
    do {
        result = AIAction_MoveToPos(2, pos, &reached, 3, true, true, 0, true, 0.0f);
    } while (result == 4);
    if (result != 2)
        return false;
    float x = pos.X + lookOffset.X;
    float y = pos.Y + lookOffset.Y;
    float z = pos.Z + lookOffset.Z;
    C3DVector lookAt(x, y, z);
    do {
        result = AIAction_LookAtPos(2, lookAt, 0x11, 0.0027777f);
    } while (result == 4);
    return result == 2;
}