// CAIStateGroup_MinionCuriosity::MoveToCuriosityProximity(float) -- walk towards CuriosityPos until within the
// given radius, repeating while the move action reports "in progress" (4); true when it reports "arrived" (2).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00933A40, length 86 bytes (+ int3 padding), `ret 4`.
// Callee: CAIStateGroupBase::AIAction_MoveToPosProximity @ 0x0097E890 (12 arguments: repeat mode 2, the
// C3DVector CuriosityPos (+0x14) BY VALUE, the radius argument, &reached, movement type 3, true, false, true, no
// leader, true, 0.0f) -- the same signature the landed AvoidRegionExit::Update uses.
// Shape: `bool reached = false;` (the explicit `mov byte ptr [esp+0Bh],0` in the high byte of the `push ecx`
// slot), a do/while loop re-issuing the action while the result is 4 (the loop head is 16-byte aligned with
// the `mov edi,edi` pad), then `return result == 2;` as a full-register bool (`xor edx,edx ; sete dl ; mov
// al,dl`). The by-value C3DVector is built in its argument slot (`sub esp,0Ch ; mov eax,esp`) because it has
// a user copy constructor. Base layout local; no engine header for this class.
class CThing;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];               // +0x04..+0x13
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* reached, long movementType,
                                     bool a, bool b, bool c, CThing* leader, bool d, float e);   // 0x0097E890
};

struct CAIStateGroup_MinionCuriosity : CAIStateGroupBase {
    C3DVector CuriosityPos;                 // +0x14
    bool MoveToCuriosityProximity(float radius);
};

bool CAIStateGroup_MinionCuriosity::MoveToCuriosityProximity(float radius)
{
    bool reached = false;
    long result;
    do {
        result = AIAction_MoveToPosProximity(2, CuriosityPos, radius, &reached, 3, true, false, true, 0, true, 0.0f);
    } while (result == 4);
    return result == 2;
}