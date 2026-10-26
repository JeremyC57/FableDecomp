// behaviour test for 0x00802590 CTCTargetingPlayer::SetAsTargetNextThing (body verbatim, callees stubbed)
#include <stdio.h>
#include <string.h>
// CTCTargetingPlayer::SetAsTargetNextThing() -- queues a "target the next thing" request: method 1 while a live thing is lock-targeted, else method 0
// Grade: RELOCATION_MATCH -- the method-0 arm is laid out right after the flag test when the condition is a negated inline IS_THING_ALIVE(locked) helper (`if (!IS_THING_ALIVE(locked)) method 0; else method 1;`); the open-coded `!locked || dead` and `locked && !dead` if/else forms always place the method-1 arm first
// Leaf: ?SetAsTargetNextThing@CTCTargetingPlayer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00802590, length 64 bytes.
// Callees: the virtual GetPTargetLockedThing() (vtable 0x0124280C slot 36 = 0x004E2328, `+0x90`), the global frame getter
// @ 0x0049D870 (`__cdecl`, as in the landed SetAsTargetNextThingInDirection 0x008023C0; manifest "GFGetPreMainMemoryUsed"
// is propagated).
// Layout: engine/CTCTargetingPlayer.h (Ego_r.pdb): TargetNextThing +0x3c, TargetNextThingMethod +0x40,
// FrameSetAsTargetNextThing +0x44. The header is a flattened POD (`void*` vptr) and the body makes a virtual call, so the
// class is a local polymorphic model with the header's member names and offsets. The locked thing's byte +0x91 bit 0 is
// its "being removed" flag (the same test as CTCTavernGame::GetHeroStats 0x005EB4B0).
// Shape: the inline SetAsTargetNextThingInDirection(method) (landed 0x008023C0) is expanded in both arms of an if/else, so
// retail tail-duplicates the three stores and the frame call; the 1 is hoisted into ecx and serves as both the method
// and the bool.
class CThing { public:
    unsigned char _pad_0x00[0x91];
    unsigned char m_removalFlags;            // +0x91 (bit 0)
};

typedef long ETargetNextThingMethod;

extern long GFGetCurrentFrame();             // 0x0049D870

class CTCTargetingPlayer { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual CThing* GetPTargetLockedThing();          // slot 36 (+0x90)
    unsigned char          _pad_0x04[0x34];           // CTCTargetingBase members
    long                   ZLockTargetSelectionTimer; // +0x38
    bool                   TargetNextThing;           // +0x3c
    bool                   ForcedToHaveNoTarget;      // +0x3d
    unsigned char          _pad_0x3e[2];
    ETargetNextThingMethod TargetNextThingMethod;     // +0x40
    long                   FrameSetAsTargetNextThing; // +0x44

    void SetAsTargetNextThingInDirection(ETargetNextThingMethod method)   // landed 0x008023C0, inline here
    {
        TargetNextThing = true;
        TargetNextThingMethod = method;
        FrameSetAsTargetNextThing = GFGetCurrentFrame();
    }
    void SetAsTargetNextThing();
};

// IS_THING_ALIVE on a raw pointer: set and the dead bit (+0x91 bit 0) clear (inlined)
inline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing != 0) {
        if (!(thing->m_removalFlags & 1))
            return true;
    }
    return false;
}
void CTCTargetingPlayer::SetAsTargetNextThing()
{
    CThing* locked = GetPTargetLockedThing();
    if (!IS_THING_ALIVE(locked))
        SetAsTargetNextThingInDirection(0);
    else
        SetAsTargetNextThingInDirection(1);
}


// ---- stubs ----
void CTCTargetingPlayer::v00() { }
void CTCTargetingPlayer::v01() { }
void CTCTargetingPlayer::v02() { }
void CTCTargetingPlayer::v03() { }
void CTCTargetingPlayer::v04() { }
void CTCTargetingPlayer::v05() { }
void CTCTargetingPlayer::v06() { }
void CTCTargetingPlayer::v07() { }
void CTCTargetingPlayer::v08() { }
void CTCTargetingPlayer::v09() { }
void CTCTargetingPlayer::v10() { }
void CTCTargetingPlayer::v11() { }
void CTCTargetingPlayer::v12() { }
void CTCTargetingPlayer::v13() { }
void CTCTargetingPlayer::v14() { }
void CTCTargetingPlayer::v15() { }
void CTCTargetingPlayer::v16() { }
void CTCTargetingPlayer::v17() { }
void CTCTargetingPlayer::v18() { }
void CTCTargetingPlayer::v19() { }
void CTCTargetingPlayer::v20() { }
void CTCTargetingPlayer::v21() { }
void CTCTargetingPlayer::v22() { }
void CTCTargetingPlayer::v23() { }
void CTCTargetingPlayer::v24() { }
void CTCTargetingPlayer::v25() { }
void CTCTargetingPlayer::v26() { }
void CTCTargetingPlayer::v27() { }
void CTCTargetingPlayer::v28() { }
void CTCTargetingPlayer::v29() { }
void CTCTargetingPlayer::v30() { }
void CTCTargetingPlayer::v31() { }
void CTCTargetingPlayer::v32() { }
void CTCTargetingPlayer::v33() { }
void CTCTargetingPlayer::v34() { }
void CTCTargetingPlayer::v35() { }
static CThing* g_locked; static int g_lockedCalls;
CThing* CTCTargetingPlayer::GetPTargetLockedThing() { g_lockedCalls++; return g_locked; }
static long g_frame = 500;
long GFGetCurrentFrame() { return g_frame; }

static int fails;
static void check(int c, const char* w) { if (!c) { fails++; printf("FAIL %s\n", w); } }

int main()
{
    CTCTargetingPlayer* tc = new CTCTargetingPlayer;
    static CThing thing;
    // 1: no locked target -> method 0
    tc->TargetNextThing = false; tc->TargetNextThingMethod = 7; tc->FrameSetAsTargetNextThing = 0; g_locked = 0;
    tc->SetAsTargetNextThing();
    check(g_lockedCalls == 1 && tc->TargetNextThing && tc->TargetNextThingMethod == 0 && tc->FrameSetAsTargetNextThing == 500, "no target");
    // 2: live locked target -> method 1
    tc->TargetNextThing = false; g_locked = &thing; thing.m_removalFlags = 0xfe; g_frame = 600;
    tc->SetAsTargetNextThing();
    check(tc->TargetNextThing && tc->TargetNextThingMethod == 1 && tc->FrameSetAsTargetNextThing == 600, "live target");
    // 3: dead locked target -> method 0
    tc->TargetNextThing = false; thing.m_removalFlags = 0x01; g_frame = 700;
    tc->SetAsTargetNextThing();
    check(tc->TargetNextThing && tc->TargetNextThingMethod == 0 && tc->FrameSetAsTargetNextThing == 700 && g_lockedCalls == 3, "dead target");
    if (!fails) printf("00802590_TEST PASS\n");
    return fails;
}