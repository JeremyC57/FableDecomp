// CAIStateGroupBase::AIAction_MoveToPreCalcRoute(const CThing& routeThing, bool& speed, EMovementType anim, const CAIFilterBase& condition, bool extra) -- walk to the route thing, then (on arrival) along its precalculated route to the destination, polling the condition each step
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097F690, length 226 bytes (`ret 14h`).
// Callees: CThing::GetPos @ 0x004C73D0, CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00 (mode 2, C3DVector by
// value, then 1,0,1,0), the condition's virtual slot 0, CAIStateGroupBase::SetUpPreCalcNav @ 0x0097F5B0.
// Shape: two `do { r = MoveToPos(...); if (!condition) return 3; } while (r == 4);` loops; the first returns any
// result other than 2; the POD vector argument is copied into the argument area dword by dword.
// Signature: Ego_r.pdb `?AIAction_MoveToPreCalcRoute@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCThing@@AA_NW4EMovementType@@
// ABVCAIFilterBase@1@_N@Z` and AIAction_MoveToPos(EAIActionRepeat, C3DVector, bool&, EMovementType, bool, bool, const CThing*,
// bool, float). These parameter types (a reference, an enum, a bool) give retail's register assignment (speed ebx,
// anim edi, extra ebp) and the loop result's spill into the dead condition slot. All-`long` parameters permute them.
class C3DVector
{ public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }   // user copy ctor: the by-value argument is built in place
};

class CThing { public:
    const C3DVector& GetPos() const;           // 0x004C73D0
};

enum EMovementType { MOVEMENT_DEFAULT };
enum { AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };

class CAIStateGroupBase
{ public:
    class CAIFilterBase                            // 4 bytes (Ego_r.pdb): the caller's "keep going" test, virtual slot 0
    { public:
        virtual bool IsStillValid() const;
    };
    virtual void v00();
    unsigned char _pad04[0x10];
    long AIAction_MoveToPos(long mode, C3DVector pos, bool& speed, EMovementType anim, bool extra,
                            bool b1, const CThing* target, bool b3, float f);          // 0x0097EC00
    C3DVector SetUpPreCalcNav(const CThing& routeThing);                                // 0x0097F5B0
    long AIAction_MoveToPreCalcRoute(const CThing& routeThing, bool& speed, EMovementType anim,
                                     const CAIFilterBase& condition, bool extra);
};

long CAIStateGroupBase::AIAction_MoveToPreCalcRoute(const CThing& routeThing, bool& speed, EMovementType anim,
                                                    const CAIFilterBase& condition, bool extra)
{
    long result;
    do
    {
        result = AIAction_MoveToPos(2, routeThing.GetPos(), speed, anim, extra, true, 0, true, 0.0f);
        if (!condition.IsStillValid())
            return AI_ACTION_ABORTED;
    } while (result == AI_ACTION_RUNNING);
    if (result != AI_ACTION_DONE)
        return result;
    C3DVector destination = SetUpPreCalcNav(routeThing);
    do
    {
        result = AIAction_MoveToPos(2, destination, speed, anim, extra, true, 0, true, 0.0f);
        if (!condition.IsStillValid())
            return AI_ACTION_ABORTED;
    } while (result == AI_ACTION_RUNNING);
    return result;
}