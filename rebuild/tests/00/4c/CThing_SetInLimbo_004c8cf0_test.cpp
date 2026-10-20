// Behaviour test for CThing::SetInLimbo (0x004C8CF0): an unchanged limbo state does nothing; a change stores the
// bit, refreshes the scan inclusion, notifies every live component (with adding forbidden meanwhile), runs the
// virtual hook and sends one clear-primitives message.
#include <cstdio>
static int g_scanUpdates = 0, g_hooks = 0, g_messages = 0, g_notified = 0; static bool g_hookValue = false, g_notifiedValue = false, g_permittedDuringNotify = true;
struct CThing; static CThing* g_thing; static bool permits(CThing*);
struct CTCBase {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {}
    virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {}
    virtual void OnParentInLimbo_slot16(bool inLimbo) { ++g_notified; g_notifiedValue = inLimbo; if (permits(g_thing)) g_permittedDuringNotify = false; }
    void* ParentThing; bool ToBeRemoved;
};
struct SComponentPair { long first; CTCBase* second; };
struct CBase { virtual ~CBase() {} CBase() {} };
namespace NTCMessages { struct CMessageClearPrimitives : CBase { CMessageClearPrimitives() { } }; }
struct CMessage;
struct CThing {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {}
    virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {}
    virtual void OnInLimboChanged_slot14(bool inLimbo) { ++g_hooks; g_hookValue = inLimbo; }
    unsigned char _pad04[0x44 - 0x04]; SComponentPair* m_componentsFirst; SComponentPair* m_componentsLast; unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1; unsigned char CreatedByOwnerThingCreation : 1; unsigned char IncludeInThingScans : 1; unsigned char Activated : 1; unsigned char GloballyLoaded : 1; unsigned char InLimbo : 1; unsigned char KillLocked : 1; unsigned char CreatedBySerialisation : 1;
    unsigned char _pad92;
    unsigned char NotPausedDuringCutscenes : 1; unsigned char DeathPersistent : 1; unsigned char KillOnLevelUnload : 1; unsigned char ActivatedScript : 1; unsigned char LevelIsUnloading : 1; unsigned char IsAffectedByBulletTimeOverride : 1; unsigned char AddingTCsPermitted : 1;
    void UpdateThingScanInclusion() { ++g_scanUpdates; }
    void SendMessageW(CMessage* message) { ++g_messages; (void)message; }
    void SetInLimbo(bool inLimbo);
};
static bool permits(CThing* t) { return t->AddingTCsPermitted != 0; }
void CThing::SetInLimbo(bool inLimbo)
{
    if (InLimbo != inLimbo) {
        InLimbo = inLimbo;
        UpdateThingScanInclusion();
        AddingTCsPermitted = false;
        for (SComponentPair* it = m_componentsFirst; it != m_componentsLast; ++it) {
            CTCBase* tc = it->second;
            if (!tc->ToBeRemoved)
                tc->OnParentInLimbo_slot16(inLimbo);
        }
        AddingTCsPermitted = true;
        OnInLimboChanged_slot14(inLimbo);
        NTCMessages::CMessageClearPrimitives message;
        SendMessageW((CMessage*)&message);
    }
}
static int fail(const char* why) { std::printf("004c8cf0_TEST FAIL: %s\n", why); return 1; }
int main()
{
    static CTCBase tcs[3]; tcs[1].ToBeRemoved = true; static SComponentPair pairs[3]; for (int i = 0; i < 3; ++i) { pairs[i].first = i; pairs[i].second = &tcs[i]; }
    static CThing thing; g_thing = &thing; thing.m_componentsFirst = pairs; thing.m_componentsLast = pairs + 3; thing.InLimbo = 0; thing.KillLocked = 1; thing.GloballyLoaded = 1; thing.AddingTCsPermitted = 1; thing.KillOnLevelUnload = 1;
    thing.SetInLimbo(false);
    if (g_scanUpdates || g_hooks || g_messages || g_notified) return fail("unchanged state acted");
    thing.SetInLimbo(true);
    if (!thing.InLimbo || !thing.KillLocked || !thing.GloballyLoaded) return fail("limbo bit / neighbours");
    if (g_scanUpdates != 1 || g_notified != 2 || !g_notifiedValue || !g_permittedDuringNotify || g_hooks != 1 || !g_hookValue || g_messages != 1) return fail("change sequence");
    if (!thing.AddingTCsPermitted || !thing.KillOnLevelUnload) return fail("adding flag restored");
    thing.SetInLimbo(false);
    if (thing.InLimbo || g_notified != 4 || g_notifiedValue || g_hooks != 2 || g_hookValue || g_messages != 2) return fail("back out of limbo");
    std::printf("004c8cf0_TEST PASS\n");
    return 0;
}