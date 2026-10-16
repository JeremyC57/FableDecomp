// CAIStateGroupBase::AIAction_MoveToPreCalcRoute(routeThing, speed, anim, condition, extra) -- walk to the route thing, then (on arrival) along its precalculated route to the destination, polling the condition each step
// Grade: DIFFER -- register allocation only: the parameters land in ebx/ebp/edi in a different assignment (retail ebx=arg after routeThing, edi=next, ebp=extra) and the loop result spills into another dead parameter slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097F690, length 226 bytes (`ret 14h`).
// Callees: CThing::GetPosition @ 0x004C73D0, CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00 (mode 2, C3DVector by
// value, then 1,0,1,0), the condition's virtual slot 0, CAIStateGroupBase::SetUpPreCalcNav @ 0x0097F5B0.
// Shape: two `do { r = MoveToPos(...); if (!condition) return 3; } while (r == 4);` loops; the first returns any
// result other than 2; the POD vector argument is copied into the argument area dword by dword.
struct C3DVector
{
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }   // user copy ctor: the by-value argument is built in place
};

struct CThing
{
    const C3DVector& GetPosition() const;           // 0x004C73D0
};

struct CAIRunCondition                              // the caller's "keep going" test, virtual slot 0
{
    virtual bool IsStillValid() const;
};

enum { AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };

struct CAIStateGroupBase
{
    virtual void v00();
    unsigned char _pad04[0x10];
    long AIAction_MoveToPos(long mode, C3DVector pos, long speed, long anim, long extra,
                            bool b1, bool b2, bool b3, bool b4);                       // 0x0097EC00
    long AIAction_MoveToPosProximity(long mode, C3DVector pos, float proximity, long speed, long anim, long extra,
                                     bool b1, bool b2, bool b3, bool b4, bool b5);    // 0x0097E890
    C3DVector SetUpPreCalcNav(const CThing* routeThing);                                // 0x0097F5B0
    long AIAction_MoveToPreCalcRoute(const CThing* routeThing, long speed, long anim,
                                     const CAIRunCondition& condition, long extra);
    long AIAction_MoveToPreCalcRouteProximity(float proximity, const CThing* routeThing, long speed, long anim,
                                              const CAIRunCondition& condition, long extra);
};

long CAIStateGroupBase::AIAction_MoveToPreCalcRoute(const CThing* routeThing, long speed, long anim,
                                                    const CAIRunCondition& condition, long extra)
{
    long result;
    do
    {
        result = AIAction_MoveToPos(2, routeThing->GetPosition(), speed, anim, extra, true, false, true, false);
        if (!condition.IsStillValid())
            return AI_ACTION_ABORTED;
    } while (result == AI_ACTION_RUNNING);
    if (result != AI_ACTION_DONE)
        return result;
    C3DVector destination = SetUpPreCalcNav(routeThing);
    do
    {
        result = AIAction_MoveToPos(2, destination, speed, anim, extra, true, false, true, false);
        if (!condition.IsStillValid())
            return AI_ACTION_ABORTED;
    } while (result == AI_ACTION_RUNNING);
    return result;
}
