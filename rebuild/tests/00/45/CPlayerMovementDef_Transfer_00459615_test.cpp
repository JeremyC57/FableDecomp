// Behaviour test for retail 0x00459615: the candidate source below is embedded verbatim (from src/00459615.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
#pragma optimize("s", on)
// CPlayerMovementDef::Transfer(CPersistContext&) -- serialize the sixteen player-movement fields in declaration order
// Retail: Fable.exe (TLC, VC7.1) @ 0x00459615, 189 bytes. Float fields use CPersistContext::Transfer(float&) 0x00431061,
//   long fields (+0x54, +0x58, +0x60) use Transfer(long&) 0x00431102.
// Layout: PDB (Ego_r) names and types; the TLC def base is 0x3c bytes (PDB 0x40), so retail offset = PDB offset - 4.
//   Local model: no engine/ header exists for CPlayerMovementDef or CPersistContext.
class CPersistContext { public:
    void Transfer(float&);                              // 0x00431061
    void Transfer(long&);                               // 0x00431102
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

// ===== harness =====
#include <stdio.h>
#include <string.h>
static int g_n; static int g_off[32]; static char g_kind[32]; static unsigned char* g_base;
void CPersistContext::Transfer(float& f) { g_off[g_n] = (int)((unsigned char*)&f - g_base); g_kind[g_n++] = 'f'; f = 1.5f; }
void CPersistContext::Transfer(long& l) { g_off[g_n] = (int)((unsigned char*)&l - g_base); g_kind[g_n++] = 'l'; l = 7; }
int main() {
    CPlayerMovementDef d; memset(&d, 0, sizeof d); CPersistContext c; g_base = (unsigned char*)&d;
    d.Transfer(c);
    int ok = sizeof(CPlayerMovementDef) == 0x7c && g_n == 16;
    for (int i = 0; ok && i < 16; ++i) { int o = 0x3c + 4 * i; ok = g_off[i] == o && g_kind[i] == ((o == 0x54 || o == 0x58 || o == 0x60) ? 'l' : 'f'); }
    ok = ok && d.NoRunningFramesToPreventSkidHalt == 7 && d.LeanAngleStepPerFrame == 1.5f;
    printf(ok ? "00459615_TEST PASS\n" : "00459615_TEST FAIL\n"); return ok ? 0 : 1;
}