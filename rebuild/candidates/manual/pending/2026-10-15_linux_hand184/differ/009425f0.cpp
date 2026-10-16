// Grade: DIFFER(223v218) -- dead-argument-slot temporary: retail homes the uninitialised reached flag in the radius argument slot (lea ecx,[esp+38h]), ours gives it a push-ecx frame byte; also the final sete goes through dl | tried: inline difference + sqrt length, ternary movement type | next: lane gap: dead-argument-slot temporary
// CAIStateGroup_WorkAtSchool::MoveAndFace(C3DVector pos, float radius, C3DVector facePos) -- walk (run when 10 or
// more units away) to within `radius` of pos, repeating while the move is in progress, then turn to look at facePos
// the same way; true when both actions finish (2).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009425F0, length 218 bytes (+ int3 padding), `ret 1Ch`.
// Callees: CThing::GetPosition @ 0x004C73D0, CAIStateGroupBase::AIAction_MoveToPosProximity @ 0x0097E890 (2, pos by
// value, radius, &reached, movement type, true, false, true, no leader, true, 0.0f), AIAction_LookAtPos @ 0x0097D580
// (2, facePos by value, 0x11, 0.0027777f). Float 10.0f @ 0x01292A1C.
// Shape: the distance is the inline C3DVector difference (components built z-first, constructor arguments
// evaluated right to left) and sqrt length; movement type 3 (walk) or 5 (run); the radius lives in edi so the
// uninitialised `reached` flag takes its argument slot; the two do/while loops re-issue the action while it returns
// 4; the C3DVector arguments have a user copy constructor (built in the argument slot). Local model.
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

struct CThing;

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};

struct CThingPos {
    const C3DVector& GetPosition() const;       // 0x004C73D0
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingPos* PCreature;                       // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* reached, long movementType,
                                     bool a, bool b, bool c, CThing* leader, bool d, float e);   // 0x0097E890
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed);              // 0x0097D580
};

struct CAIStateGroup_WorkAtSchool : CAIStateGroupBase {
    bool MoveAndFace(C3DVector pos, float radius, C3DVector facePos);
};

bool CAIStateGroup_WorkAtSchool::MoveAndFace(C3DVector pos, float radius, C3DVector facePos)
{
    long movementType = (pos - PBrain->PCreature->GetPosition()).GetLength() < 10.0f ? 3 : 5;
    bool reached;
    long result;
    do {
        result = AIAction_MoveToPosProximity(2, pos, radius, &reached, movementType, true, false, true, 0, true, 0.0f);
    } while (result == 4);
    if (result != 2)
        return false;
    do {
        result = AIAction_LookAtPos(2, facePos, 0x11, 0.0027777f);
    } while (result == 4);
    return result == 2;
}
