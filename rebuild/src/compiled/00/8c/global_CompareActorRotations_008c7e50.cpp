// CompareActorRotations(CThingCreatureBase* creature, CThing* target) -- is the target more than 70 degrees off the creature's facing (XY bearing as a 0..1 turn fraction against the body's facing angle)
// Leaf: ?CompareActorRotations@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C7E50, length 173 bytes (+ int3 padding), `__fastcall` (creature in
//   ecx, target in edx), plain `ret`.
// Identity: callers 0x008C80C4, 0x008C811A, 0x008C816A (the landed IsToCauseRecoil overrides of the bandit-king attack /
//   stuck / unstuck actions 0x008C8080 / 0x008C80E0 / 0x008C8130) and 0x008C932B; they declare it
//   `bool __fastcall CompareActorRotations(CThingCreatureBase*, CThing*)` (manifest name; no PDB symbol -- descriptive).
// Callees: CThing::GetPos() const @ 0x004C73D0 (twice), the physics body's (+0x60) virtual slot 63 (+0xFC,
//   GetFacingAngleXY, descriptive), AngleDifference(float, float) @ 0x00A441E0 (`__stdcall`), _CIfmod @ 0x00BFEB28.
// Constants: 1/(2*pi) float 0x01237F08, 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 1.0 double 0x01237F00, 70/360 float 0x012817F0.
// Shape: the bearing is `(float)atan2(dx, dy) * INV_TWO_PI` (fpatan, then a float `fmul dword`; a double product
//   gives `fmul qword`) kept in a float local (`fst` into the `push ecx` frame word); the body pointer is fetched into
//   a local right after the positions (retail reuses esi early); an inline wrap folds the fraction into [0, 1) only
//   when it is outside (fmod by 1.0, +1 when negative); the facing is fetched after the angle argument is pushed
//   (AngleDifference(facing, angle), arguments right to left); the result goes into a named float and an inline
//   float fabsf (`(float)fabs` written in the condition schedules `pop edi` before the fcomp); `> 70/360` returns
//   through `mov al,1` / `xor al,al`.
extern "C" double __cdecl atan2(double y, double x);
extern "C" double __cdecl fmod(double x, double y);
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(atan2, fmod, fabs)

class C3DVector { public:
    float X, Y, Z;
};

class CPhysicsBody {                                    // the thing's physics body at +0x60
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062();
    virtual float GetFacingAngleXY();                   // slot 63 (+0xFC)
};

class CThing {
public:
    unsigned char _pad00[0x60];
    CPhysicsBody* m_pBody;                              // +0x60
    const C3DVector& GetPos() const;               // 0x004C73D0
    CPhysicsBody* GetBody() const { return m_pBody; }
};

class CThingCreatureBase : public CThing {
};

float __stdcall AngleDifference(float from, float to);  // 0x00A441E0

static const float INV_TWO_PI = 0.15915494f;            // 0x01237F08
static const float MAX_FACING_DIFFERENCE = 0.19444445f; // 70 / 360, 0x012817F0

inline float fabsf(float x) { return (float)fabs(x); }

inline float WrapTurnFraction(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod(angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

bool __fastcall CompareActorRotations(CThingCreatureBase* creature, CThing* target)
{
    const C3DVector& from = creature->GetPos();
    const C3DVector& to = target->GetPos();
    CPhysicsBody* body = creature->GetBody();
    float angle = (float)atan2(to.X - from.X, to.Y - from.Y) * INV_TWO_PI;
    angle = WrapTurnFraction(angle);
    float difference = AngleDifference(body->GetFacingAngleXY(), angle);
    if (fabsf(difference) > MAX_FACING_DIFFERENCE)
        return true;
    return false;
}