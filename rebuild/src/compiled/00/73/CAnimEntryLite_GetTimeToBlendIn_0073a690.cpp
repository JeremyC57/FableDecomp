// CAnimEntryLite::GetTimeToBlendIn(CThing* thing) -- the cached transition time, scaled by bullet time when the thing is affected by it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073A690, length 71 bytes (+ int3 padding).
// Callees: CThing::IsAffectedByBulletTime() @ 0x004C9920, CBulletTime::GetTimeScale() @ 0x004C5FA0 (float),
// __ftol2 @ 0x00BFEA70.
// Globals: the application object @ 0x013B86A0 (+0x24 game, +0x68 bullet time).
// Layout: PDB CAnimEntryLite (0x10): BankIndex, DelayTime +0x04, TransitionTime +0x08, AnimFlags +0x0c.
// Shape: the same body as CAnimationEntry::GetTimeToBlendIn @ 0x005DC230 with the member TransitionTime in
// place of the component lookup: it is read into esi and spilled to the one frame slot (`push ecx`) because
// `fimul dword ptr [esp+4]` wants the integer in memory; `(long)(scale * time)` goes through __ftol2.
struct CThing {
    bool IsAffectedByBulletTime();              // 0x004C9920
};

// The bullet-time controller hanging off the game object (+0x68): a pointer at +0x04 and an "active" byte
// at +0x0c decide whether the time scale applies; GetTimeScale() @ 0x004C5FA0 returns the x87 multiplier.
struct CBulletTime {
    unsigned char _pad0[4];
    void* m_pController;                        // +0x04
    unsigned char _pad1[4];
    bool m_active;                              // +0x0c
    float GetTimeScale();                       // 0x004C5FA0
    bool IsActive() const { return m_active || m_pController != 0; }
};

struct CGame {
    unsigned char _pad[0x68];
    CBulletTime* m_pBulletTime;                 // +0x68
};

struct CApplication {
    unsigned char _pad[0x24];
    CGame* m_pGame;                             // +0x24
};

extern CApplication* g_pApplication;            // 0x013B86A0

struct CAnimEntryLite {
    long BankIndex;                             // +0x00
    long DelayTime;                             // +0x04
    long TransitionTime;                        // +0x08
    long AnimFlags;                             // +0x0c
    long GetTimeToBlendIn(CThing* thing) const;
};

long CAnimEntryLite::GetTimeToBlendIn(CThing* thing) const
{
    long time = TransitionTime;
    if (thing->IsAffectedByBulletTime()) {
        CBulletTime* bulletTime = g_pApplication->m_pGame->m_pBulletTime;
        if (bulletTime->IsActive())
            return (long)(bulletTime->GetTimeScale() * time);
    }
    return time;
}