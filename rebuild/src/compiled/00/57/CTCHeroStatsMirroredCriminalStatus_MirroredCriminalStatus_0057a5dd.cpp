// CTCHeroStats::MirroredCriminalStatus::MirroredCriminalStatus() -- a clean criminal record for one region: no crimes, never seen by a guard, never exited, no entrance position or angle
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057A5DD, 45 bytes, plain `ret` (an unaligned address: a size-optimised TU).
// Leaf: ??0MirroredCriminalStatus
// Pragma: optimize("s", on) -- the `push 0Ah ; pop ecx` count idiom.
// Layout: Ego_r.pdb CTCHeroStats::MirroredCriminalStatus (0x44): Crimes long[10] +0x00, LastTimeSeenByGuard +0x28,
//         LastTimeExited +0x2c, ExitedUsingTeleporter +0x30, EntrancePosX/Y/Z +0x34/+0x38/+0x3c, EntranceAngle +0x40.
//         Callers 0x0057DFA0, 0x0057EAA7, 0x0057F17D, 0x0057F619, 0x00581310 (CTCHeroStats code).
// Shape: the Crimes array cleared with `rep stosd` (eax = 0 reused for the integer members), each float member its own
//        `fldz ; fstp`; the body order (loop, X/Y/Z, LastTimeSeenByGuard, the flag, LastTimeExited, angle) is retail's store
//        order (declaration order or an init list put LastTimeExited before the flag byte).
#pragma optimize("s", on)
class CTCHeroStats { public:
    struct MirroredCriminalStatus {
        long  Crimes[10];                            // +0x00
        long  LastTimeSeenByGuard;                   // +0x28
        long  LastTimeExited;                        // +0x2c
        bool  ExitedUsingTeleporter;                 // +0x30
        float EntrancePosX;                          // +0x34
        float EntrancePosY;                          // +0x38
        float EntrancePosZ;                          // +0x3c
        float EntranceAngle;                         // +0x40
        MirroredCriminalStatus();
    };
};
CTCHeroStats::MirroredCriminalStatus::MirroredCriminalStatus()
{
    for (long i = 0; i < 10; ++i)
        Crimes[i] = 0;
    EntrancePosX = 0.0f;
    EntrancePosY = 0.0f;
    EntrancePosZ = 0.0f;
    LastTimeSeenByGuard = 0;
    ExitedUsingTeleporter = false;
    LastTimeExited = 0;
    EntranceAngle = 0.0f;
}