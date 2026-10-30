// CTCLook::ChangeHeadAngleXY(float targetAngle, float time) -- drive the head's XY angle (relative to the body) towards the target through the slow-in/fast-out integrator, the turn speed depending on whether a target is acquired
// Grade: RELOCATION_MATCH (retry wave 2 sec2: the PDB getters GetHeadAngularVelocityMaxXY / GetHeadTurnRangeXY as inline bodies; the range getter is `0.5f * (range * 1/360)`, which keeps the two multiplies apart). Was DIFFER(91v112): a named speed local spilled to the frame and `def * k * 0.5` folded into one constant.
// Leaf: ?ChangeHeadAngleXY@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00752120, length 112 bytes (`ret 8`).
// Name: FableWin.pdb + Ego_r.pdb `?ChangeHeadAngleXY@CTCLook@@AAEXMM@Z` (private); the landed binding spells it QAE (NAME_FIXES.tsv).
// Label correction: catalog "CTCLook::ChangeHeadAngleYZ" is propagated; this body works on HeadAngleRelToBodyXY/HeadVelocityXY and CentreHead
// (0x007528A0) calls it first with the XY argument pair, then the real ChangeHeadAngleYZ @ 0x00752190.
// Callee: `?GetHeadSettingsSlowInFastOut@CTCLook@@ABEXAAM0MMMMM@Z` @ 0x00751820 (FableWin + Ego_r: private const, float& in/out;
//   BSim audit maps 0x751820 to it; the landed binding spells it QAEXPAM0MMMMM, NAME_FIXES.tsv).
// Inline getters (PDB spellings, bodies not emitted): `?GetHeadAngularVelocityMaxXY@CTCLook@@AAEMXZ` (turn speed with / without a
//   target, .data float pairs {XY, YZ} 0x01382680 / 0x01382688), `?GetHeadTurnRangeXY@CTCLook@@ABEMXZ` (half the def's XY range in
//   turns: 1/360 0x01238E00, 0.5 0x0122F59C). The same getter body also matches GetHeadCreatureAnimFrame 0x00752510.
// Layout (retail): PDef +0x14, HeadAngleRelToBodyXY +0x2C, HeadVelocityXY +0x5C, TargetAcquired +0xA0 (PDB +0xA4; the retail
//   TurningAnims vector is 12 bytes). CLookDef (Ego_r.pdb) HeadTurnRangeXY +0x30 (degrees).
#include "engine/CLookDef.h"

struct SLookAnglePair { float XY; float YZ; };
extern SLookAnglePair g_HeadTurnSpeedTargetAcquired;   // 0x01382680 (.data tunables)
extern SLookAnglePair g_HeadTurnSpeedNoTarget;         // 0x01382688
extern SLookAnglePair g_HeadTurnAcceleration;          // 0x01382690

struct CLookDefPointer {
    const CLookDef* m_pDef;
    const CLookDef* operator->() const { return m_pDef; }
};

class CTCLook { public:
    unsigned char   _pad00[0x14];
    CLookDefPointer PDef;                       // +0x14
    unsigned char   _pad18[0x2c - 0x18];
    float           HeadAngleRelToBodyXY;       // +0x2c
    float           HeadAngleRelToBodyYZ;       // +0x30
    unsigned char   _pad34[0x5c - 0x34];
    float           HeadVelocityXY;             // +0x5c
    float           HeadVelocityYZ;             // +0x60
    unsigned char   _pad64[0xa0 - 0x64];
    bool            TargetAcquired;             // +0xa0 (PDB +0xa4)
private:
    float GetHeadAngularVelocityMaxXY() { return TargetAcquired ? g_HeadTurnSpeedTargetAcquired.XY : g_HeadTurnSpeedNoTarget.XY; }
    float GetHeadTurnRangeXY() const { return 0.5f * (PDef->HeadTurnRangeXY * 0.0027777778f); }
    void GetHeadSettingsSlowInFastOut(float& angle, float& velocity, float acceleration, float maxSpeed, float target, float minAngle, float maxAngle) const;   // 0x00751820
    void ChangeHeadAngleXY(float targetAngle, float time);
};

void CTCLook::ChangeHeadAngleXY(float targetAngle, float time)
{
    GetHeadSettingsSlowInFastOut(HeadAngleRelToBodyXY, HeadVelocityXY, g_HeadTurnAcceleration.XY * time, GetHeadAngularVelocityMaxXY() * time, targetAngle,
                                 GetHeadTurnRangeXY(), GetHeadTurnRangeXY());
}