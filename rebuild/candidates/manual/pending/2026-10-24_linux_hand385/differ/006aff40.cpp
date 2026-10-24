// Grade: DIFFER(225v212) -- the second bearing: retail keeps it on the x87 stack (`fcom 0 ; fst [esp+4]`, `fcom 1.0`, `fstp st0 ; fld [esp+4]` only before fmod, no store of the fmod result) and every path joins one `fsubr [esp]`; ours spills and reloads it like the first (`fst ; fcomp ; fld`) and keeps a second `fld [esp] ; fsub [esp+4]` tail. The first bearing and the virtual calls match.
// CTCPhysicsBase::GetTurnSpeedAngleXY() const -- the change of facing in the XY plane between the old and the last orientation, in turns
// Leaf: ?GetTurnSpeedAngleXY@CTCPhysicsBase@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AFF40, length 212 bytes (+ int3 padding), thiscall, plain `ret`,
//   float result on st0.
// Callees: the TC's virtual GetLastRHSet() const (slot 72, +0x120, named by the landed CThingCreatureBase::OnDie
//   0x00663F50) and slot 73 (+0x124, the previous-frame set; "GetOldRHSet" is descriptive, after UpdateOldRHSet
//   0x006AF740), _CIfmod @ 0x00BFEB28 (fmod intrinsic). Constants: 1/(2*pi) float 0x01237F08, 0.0f 0x0122DEDC,
//   1.0f 0x0122DED8, 1.0 double 0x01237F00.
// Identity: Ego_r.pdb `?GetTurnSpeedAngleXY@CTCPhysicsBase@@QBEMXZ` (name set only): retail keeps the donor order
//   UpdateCollisionGroundIneractions 0x006AFF00, GetTurnSpeedAngleXY, GetTurnSpeedAngleYZ (0x006B0020),
//   SetAsMoving 0x006B0130; the one caller, CThingCreatureBase::WorldUpdate, calls it on the creature's
//   physics TC (+0x60) and stores the result. The catalog's "ComputeAngleDelta" is a descriptive auto label.
// Layout: CRightHandedSet (Ego_r) Up +0x00, Forward +0x0c; only Forward.X / Forward.Y are read.
// Shape: both sets fetched first (edi, esi); each angle is atan2(F.X, F.Y) / 2pi (`fpatan`, `fmul`) wrapped into
//   [0, 1) by an inline helper (outside the range: fmod by 1.0, then +1 when negative); the result is
//   last - old (`fsubr [esp]`).
// Tried: an inline WrapTurns(float) over `atan2(float,float) * K` with named results (225v212), the second
//   wrap as the direct return operand (225v212), a multi-return helper (227v212), and this landed
//   __forceinline GetBearingInTurns form (225v212): the FIRST bearing always matches byte for byte.
#include <math.h>
#pragma intrinsic(atan2, fmod)

class C3DVector {
public:
    float X, Y, Z;
};

class CRightHandedSet {
public:
    C3DVector Up;                                           // +0x00
    C3DVector Forward;                                      // +0x0c
};

// Bearing of a direction in turns, wrapped into [0, 1) (the landed 0x008F4D60 helper; name descriptive).
static __forceinline float GetBearingInTurns(float x, float y)
{
    float turns = (float)atan2((double)x, (double)y) * 0.15915494f;     // 1/(2pi) 0x01237F08
    if (turns < 0.0f || turns >= 1.0f) {
        turns = (float)fmod((double)turns, 1.0);
        if (turns < 0.0f)
            turns += 1.0f;
    }
    return turns;
}

class CTCPhysicsBase {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071();
    virtual const CRightHandedSet& GetLastRHSet() const;   // slot 72 (+0x120)
    virtual const CRightHandedSet& GetOldRHSet() const;    // slot 73 (+0x124, descriptive)
    float GetTurnSpeedAngleXY() const;
};

float CTCPhysicsBase::GetTurnSpeedAngleXY() const
{
    const CRightHandedSet& last = GetLastRHSet();
    const CRightHandedSet& old = GetOldRHSet();
    float last_bearing = GetBearingInTurns(last.Forward.X, last.Forward.Y);
    float old_bearing = GetBearingInTurns(old.Forward.X, old.Forward.Y);
    return last_bearing - old_bearing;
}
