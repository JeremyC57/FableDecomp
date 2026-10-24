// _global::GetUpdateTimeForThing(const CThing& thing, const CWorld& world) -- one frame's update time for a thing: 1 / fps seconds, divided by the bullet-time scale while bullet time is active and the thing is affected by it
// Leaf: ?GetUpdateTimeForThing@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00787C62, 67 bytes, __fastcall (thing in ecx, world in edx), float in
//   st(0), plain `ret`.
// Identity: Ego_r.pdb `?GetUpdateTimeForThing@?A0xedf0dd2a@@YIMABVCThing@@ABVCWorld@@@Z` (a file-local function of the
//   trap TU); callers CTCMeleeTrap::UpdateAnimation 0x0078856C and CTCTrapBase::FrameUpdate 0x007888C5 (both landed).
// Callees: CThing::IsAffectedByBulletTime() const @ 0x004C9920, CBulletTime::GetTimeScale() @ 0x004C5FA0 (float).
// Data: g_framesPerSecond (long) @ 0x01375550, 1.0f @ 0x0122DED8.
// Layout: CWorld's bullet-time controller at +0x68 (active flag +0x0c, controller +0x04), as the landed
//   CTCAnimatingObject::FrameUpdate 0x007F8E20 which inlines the same expression.
// Shape: `float seconds = 1.0f / g_framesPerSecond; if (!bullet time active) return seconds; if
//   (thing.IsAffectedByBulletTime()) return seconds / scale; return seconds;` -- three separate returns: `fild ; fdivr`
//   keeps the value on the x87 stack for the early exit and spills it to the `push ecx` frame word (`fst`) for the
//   other two (`fld` / `fdivr` from the slot); the `&&` form merges two exits into one `fld`. `thing` stays in ecx
//   untouched for the member call; the function pointer below only keeps the file-local function emitted. The trap TU is size-optimised with frame pointers: `optimize("y", off)` + `optimize("s", on)` (as the landed
//   CTCProjectileTrap::TriggerTrap 0x00787CA5).
#pragma optimize("y", off)
#pragma optimize("s", on)
extern long g_framesPerSecond;                           // 0x01375550

class CThing {
public:
    bool IsAffectedByBulletTime() const;                 // 0x004C9920
};

struct CBulletTime {
    unsigned char _pad0[4];
    void*         m_pController;                         // +0x04
    unsigned char _pad1[4];
    bool          m_active;                              // +0x0c
    float GetTimeScale();                                // 0x004C5FA0
    bool IsActive() const { return m_active || m_pController != 0; }
};

struct CWorld {
    unsigned char _pad[0x68];
    CBulletTime*  m_pBulletTime;                         // +0x68
};

namespace {

float __fastcall GetUpdateTimeForThing(const CThing& thing, const CWorld& world)
{
    float seconds = 1.0f / g_framesPerSecond;
    if (!world.m_pBulletTime->IsActive())
        return seconds;
    if (thing.IsAffectedByBulletTime())
        return seconds / world.m_pBulletTime->GetTimeScale();
    return seconds;
}

}

float (__fastcall* g_keepGetUpdateTimeForThing)(const CThing&, const CWorld&) = GetUpdateTimeForThing;