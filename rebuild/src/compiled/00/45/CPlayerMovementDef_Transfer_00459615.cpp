// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
#pragma optimize("s", on)
// CPlayerMovementDef::Transfer(CPersistContext&) -- serialize the sixteen player-movement fields in declaration order
// Retail: Fable.exe (TLC, VC7.1) @ 0x00459615, 189 bytes. Float fields use CPersistContext::Transfer(float&) 0x00431061,
//   long fields (+0x54, +0x58, +0x60) use Transfer(long&) 0x00431102.
// Layout: PDB (Ego_r) names and types; the TLC def base is 0x3c bytes (PDB 0x40), so retail offset = PDB offset - 4.
//   Local model: no engine/ header exists for CPlayerMovementDef or CPersistContext.
class CPersistContext { public:
    template <class T> void Transfer(T& value);                              // 0x00431061
};
class CPlayerMovementDef { public:
    unsigned char _base[0x3c];                          // CDefClassBase/CParentDefClassBase
    float JoystickMovementStationaryBand;               // +0x3c
    float JoystickMovementSlowBand;                     // +0x40
    float JoystickMovementWalkingBand;                  // +0x44
    float JoystickMovementJoggingBand;                  // +0x48
    float MinRequiredAngleFor180;                       // +0x4c
    float SecondsToStoreLastMovementFor;                // +0x50
    long  NoMovementFramesToCheckFor180;                // +0x54
    long  NoMovementFramesToCheckFor180PriorStraightLine; // +0x58
    float MinNoRunSecondsForSkidHaltMove;               // +0x5c
    long  NoRunningFramesToPreventSkidHalt;             // +0x60
    float MaxAllowedAngleChangeForSkidHalt;             // +0x64
    float MaxTurnSpeed;                                 // +0x68
    float MaxTurnSpeedHeavyWeapon;                      // +0x6c
    float LeanAngleTurnFactor;                          // +0x70
    float LeanAngleLimitAngle;                          // +0x74
    float LeanAngleStepPerFrame;                        // +0x78
    void Transfer(CPersistContext&);
};
void CPlayerMovementDef::Transfer(CPersistContext& ctx) {
    ctx.Transfer(JoystickMovementStationaryBand);
    ctx.Transfer(JoystickMovementSlowBand);
    ctx.Transfer(JoystickMovementWalkingBand);
    ctx.Transfer(JoystickMovementJoggingBand);
    ctx.Transfer(MinRequiredAngleFor180);
    ctx.Transfer(SecondsToStoreLastMovementFor);
    ctx.Transfer(NoMovementFramesToCheckFor180);
    ctx.Transfer(NoMovementFramesToCheckFor180PriorStraightLine);
    ctx.Transfer(MinNoRunSecondsForSkidHaltMove);
    ctx.Transfer(NoRunningFramesToPreventSkidHalt);
    ctx.Transfer(MaxAllowedAngleChangeForSkidHalt);
    ctx.Transfer(MaxTurnSpeed);
    ctx.Transfer(MaxTurnSpeedHeavyWeapon);
    ctx.Transfer(LeanAngleTurnFactor);
    ctx.Transfer(LeanAngleLimitAngle);
    ctx.Transfer(LeanAngleStepPerFrame);
}