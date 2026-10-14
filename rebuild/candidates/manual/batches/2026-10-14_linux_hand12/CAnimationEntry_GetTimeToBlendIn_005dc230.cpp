// CAnimationEntry::GetTimeToBlendIn(CThing* thing) -- the transition-in time component value, scaled by bullet time when the thing is affected by it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DC230, length 88 bytes (+ int3 padding).
// Callees: CAnimationEntry::GetTransitionInTimeComponent(T**) @ 0x005E0280 (writes the component of type 1 from
// the entry's map, or the default one, through its out pointer -- the slot is re-read after the call), CThing::IsAffectedByBulletTime()
// @ 0x004C9920, CBulletTime::GetTimeScale() @ 0x004C5FA0 (float), __ftol2 @ 0x00BFEA70.
// Globals: the application object @ 0x013B86A0 (+0x24 game, +0x68 bullet time).
// Shape: the inline getter's out-pointer local lives in the single frame slot (`push ecx`); the component's
// value (+0x08) is read into esi and spilled to the SAME slot (the getter's local is out of scope) because `fimul dword ptr [esp+4]` wants
// the integer in memory; `(long)(scale * time)` goes through __ftol2. The unscaled value returns in eax.
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

struct CAnimComponentTransitionInTime {
    unsigned char _pad[8];                      // vptr, ref count
    long TransitionInTime;                      // +0x08
};


struct CAnimationEntry {
    long BankIndex;                             // +0x00
    long AnimName;                              // +0x04 CDefString
    long GroupName;                             // +0x08 CDefString
    void* Components;                           // +0x0c CIVCountedPointer<CAnimComponentMap>
    void GetTransitionInTimeComponent(CAnimComponentTransitionInTime** ppComponent) const;   // 0x005E0280 (out pointer)
    long GetAnimationTransitionInTime() const      // the inline getter: its out-pointer local dies with it
    {
        CAnimComponentTransitionInTime* pComponent;
        GetTransitionInTimeComponent(&pComponent);
        return pComponent->TransitionInTime;
    }
    long GetTimeToBlendIn(CThing* thing) const;
};

long CAnimationEntry::GetTimeToBlendIn(CThing* thing) const
{
    long time = GetAnimationTransitionInTime();
    if (thing->IsAffectedByBulletTime()) {
        CBulletTime* bulletTime = g_pApplication->m_pGame->m_pBulletTime;
        if (bulletTime->IsActive())
            return (long)(bulletTime->GetTimeScale() * time);
    }
    return time;
}
