// Grade: DIFFER(175v175) -- lane gap: dead-argument-slot temporary -- retail keeps the look-at X/Y in the 0xC frame and stores Z into the dead pos.X argument slot ([esp+14h]); ours needs a 0x10 frame (or, with a value ctor, evaluates Z first) | tried: operator+ via value ctor (right-to-left order), field-by-field named sum
// CAIStateGroup_ServeFood::WalkTo(C3DVector, C3DVector, C3DVector) -- walk to the position (repeating while the action
// Leaf: WalkTo
// returns 4); on arrival turn to look at position + look offset; report whether the look completed.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00922FC0, length 175 bytes (+ int3 padding), `ret 24h` (three by-value
// C3DVectors; the middle one is unused here).
// Callees: CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00 (repeat 2, pos, &reached, movement 3, true, true, 0,
// true, 0.0), CAIStateGroupBase::AIAction_LookAtPos @ 0x0097D580 (repeat 2, by-value C3DVector, flags 0x11, speed
// 0.0027777f).
// Shape: the landed do/while action loops; `pos + lookOffset` through an inline C3DVector operator+ (x87 adds stored to
// the frame); `return result == 2;` (`sete cl`). No engine header for this class.
struct CThing;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    C3DVector operator+(const C3DVector& other) const
    {
        C3DVector sum;                          // field by field in a named local (x first)
        sum.X = X + other.X;
        sum.Y = Y + other.Y;
        sum.Z = Z + other.Z;
        return sum;
    }
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
    C3DVector lookAt = pos + lookOffset;
    do {
        result = AIAction_LookAtPos(2, lookAt, 0x11, 0.0027777f);
    } while (result == 4);
    return result == 2;
}
