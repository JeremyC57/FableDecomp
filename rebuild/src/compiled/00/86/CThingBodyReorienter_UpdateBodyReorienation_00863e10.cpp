// CThingBodyReorienter::UpdateBodyReorienation() -- one frame of body reorientation: gather obstacles, look for a new angle, turn gradually, and re-fit to slopes every N frames
// Retail: Fable.exe (TLC, VC7.1) @ 0x00863E10, 83 bytes (+ int3 padding). (The misspelt name is the catalog's.)
// Callees: FillBodyReorientationPossibleIntersecting @ 0x00863CA0, CheckForNewBodyReorientationAngle @ 0x008637D0,
//          GradualReorientBody @ 0x00863280, ReorientBodyYZForSlopes @ 0x008633B0 (tail jump) -- all CThingBodyReorienter.
//          Global: the slope re-fit period in frames (long @ 0x01385AE4).
// Layout: PDB CThingBodyReorienter (0x2c bytes): BodyReorientationDone +0, ReorientingBody +1, ReorientationCount +4,
//   StillCheckingForNewBodyReorientationAngle +8, HaveFinalReorientationAngle +9, FinalReorientationAngleXY +0xc,
//   RotationDirection +0x10, HasPossibleIntersectingThings +0x14, PossibleIntersectingThings +0x18, Body +0x28.
//   Retail stops at +0x14 here (the vector's size difference does not matter).
// Shape: BodyReorientationDone is re-tested after the angle check (the check may finish the reorientation); the counter
//   is pre-incremented and `count % period == 0` is a signed `idiv`.
extern long g_BodyReorientSlopePeriod;  // 0x01385AE4

class CThingBodyReorienter { public:
    bool BodyReorientationDone;                         // +0x00
    bool ReorientingBody;                               // +0x01
    long ReorientationCount;                            // +0x04
    bool StillCheckingForNewBodyReorientationAngle;     // +0x08
    bool HaveFinalReorientationAngle;                   // +0x09
    float FinalReorientationAngleXY;                    // +0x0c
    long RotationDirection;                             // +0x10 ERotationDirection
    bool HasPossibleIntersectingThings;                 // +0x14
    void FillBodyReorientationPossibleIntersecting();   // 0x00863CA0
    void CheckForNewBodyReorientationAngle();           // 0x008637D0
    void GradualReorientBody();                         // 0x00863280
    void ReorientBodyYZForSlopes();                     // 0x008633B0
    void UpdateBodyReorienation();
};

void CThingBodyReorienter::UpdateBodyReorienation()
{
    if (BodyReorientationDone)
        return;
    if (!HasPossibleIntersectingThings)
        FillBodyReorientationPossibleIntersecting();
    if (StillCheckingForNewBodyReorientationAngle)
        CheckForNewBodyReorientationAngle();
    if (!BodyReorientationDone)
        GradualReorientBody();
    ++ReorientationCount;
    if (StillCheckingForNewBodyReorientationAngle && ReorientationCount % g_BodyReorientSlopePeriod == 0)
        ReorientBodyYZForSlopes();
}