// CThingBodyReorienter::GradualReorientBody() -- turn the body towards the final XY facing angle (angles in turns): wrap the remaining turn into [0, 1), take the short way round, snap and finish when it is within reach, else turn by at most half a turn this frame
// Grade: DIFFER(ours v retail) -- 262v290: retail keeps the turn difference in its frame float (fst after the subtraction, fld/fstp around fmod and += 1, two reloads before the 0.5 test) while the delta stays on the x87 stack; ours keeps the difference in a register (current/difference frame slots swapped). /Op reproduces the memory traffic for the difference but also spills the delta (318 bytes); a float& wrap helper changes nothing
// Leaf: ?GradualReorientBody@CThingBodyReorienter
// Retail: Fable.exe (TLC, VC7.1) @ 0x00863280, 290 bytes (+14 int3), `ret`.
// Callees: the body's physics component (Body +0x24 -> PhysicsTC +0x60) virtual slots 63 (+0xfc, GetFacingAngleXY, float)
//   and 66 (+0x108, SetFacingAngleXY(float)); _CIfmod @ 0x00BFEB28 (the fmod intrinsic, divisor the double 1.0
//   @ 0x01237F00). Globals: 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 0.5f 0x0122F59C, -0.5f 0x01238798, 1e-4f 0x0129BA3C (an
//   extern const: `eps + 0.5f` stays a run-time add).
// Layout: Ego_r.pdb CThingBodyReorienter (0x2C): BodyReorientationDone +0, StillCheckingForNewBodyReorientationAngle +8,
//   HaveFinalReorientationAngle +9, FinalReorientationAngleXY +0xC; retail Body +0x24 (PDB +0x28; the STLport vector
//   before it is 12 bytes).
// Shape: the wrap is `if (d < 0 || d >= 1) { d = fmod(d, 1.0); if (d < 0) d += 1; }` through the frame float; the short
//   way is a ternary kept on the x87 stack; the finish test compares |delta| with eps + 0.5; the clamp is an if/else-if
//   over -0.5 / 0.5 and the new angle `current + delta`.
extern "C" double __cdecl fmod(double x, double y);
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fmod, fabs)
extern const float g_fEpsilon;                  // 0x0129BA3C (1e-4f)
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetFacingAngleXY() const;      // slot 63 (+0xfc)
    virtual void v64(); virtual void v65();
    virtual void SetFacingAngleXY(float angle);  // slot 66 (+0x108)
};
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                   // +0x60
};
struct CThingBodyReorienter {
    bool  BodyReorientationDone;                 // +0x00
    bool  ReorientingBody;                       // +0x01
    long  ReorientationCount;                    // +0x04
    bool  StillCheckingForNewBodyReorientationAngle;   // +0x08
    bool  HaveFinalReorientationAngle;           // +0x09
    float FinalReorientationAngleXY;             // +0x0c
    long  RotationDirection;                     // +0x10 ERotationDirection
    bool  HasPossibleIntersectingThings;         // +0x14
    unsigned char _pad15[0x24 - 0x15];           // PossibleIntersectingThings (12-byte STLport vector) +0x18
    CThing* Body;                                // +0x24
    void GradualReorientBody();
};

void CThingBodyReorienter::GradualReorientBody()
{
    if (HaveFinalReorientationAngle) {
        float current = Body->PhysicsTC->GetFacingAngleXY();
        float difference = FinalReorientationAngleXY - current;
        if (difference < 0.0f || difference >= 1.0f) {
            difference = (float)fmod(difference, 1.0);
            if (difference < 0.0f)
                difference += 1.0f;
        }
        float delta = difference > 0.5f ? difference - 1.0f : difference;
        if (fabs(delta) < g_fEpsilon + 0.5f) {
            HaveFinalReorientationAngle = false;
            Body->PhysicsTC->SetFacingAngleXY(FinalReorientationAngleXY);
            if (!StillCheckingForNewBodyReorientationAngle)
                BodyReorientationDone = true;
            return;
        }
        if (delta < -0.5f)
            delta = -0.5f;
        else if (delta > 0.5f)
            delta = 0.5f;
        Body->PhysicsTC->SetFacingAngleXY(current + delta);
    }
}
