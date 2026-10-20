// Behaviour test for CTCKrakenTentacle::FrameUpdate @ 0x007F14F0 (self-contained model of the candidate logic).
#include <stdio.h>
#include <string.h>
static char g_log[64]; static long g_worldFrame = 100;
struct CTCCreatureFlags9 { unsigned char m_canBeTargeted; };
struct CThing { bool Dead; float EmergeFraction; CTCCreatureFlags9* flags; bool killed;
    bool GetInterface(CTCCreatureFlags9** out) { if (flags) { *out = flags; return true; } return false; }
    void Kill(bool) { killed = true; strcat(g_log, "K"); } };
struct Ptr { CThing* item; CThing* GetPItem() const { return item; } };
static inline bool IS_THING_ALIVE(const Ptr& p) { if (p.GetPItem() != 0) { if (!p.GetPItem()->Dead) return true; } return false; }
struct Model {
    CThing* ParentThing; Ptr PKraken; bool CanStrike, Tracking, IsStriking; long QueuedStrikeTick; bool DoneDeathStuff;
    void FrameUpdate_Base() { strcat(g_log, "B"); }
    void CreateHeadEffect() { strcat(g_log, "H"); }
    void RemoveObstruction() { strcat(g_log, "O"); }
    void TrackTarget() { strcat(g_log, "T"); }
    void ReturnToDesiredPos(bool) { strcat(g_log, "R"); }
    void PerformStrikeDamage() { strcat(g_log, "S"); }
    void FrameUpdate()
    {
        FrameUpdate_Base();
        CanStrike = false;
        if (!IS_THING_ALIVE(PKraken)) { ParentThing->Kill(false); return; }
        {
            if (!DoneDeathStuff) {
                CThing* thing = ParentThing;
                if (thing->EmergeFraction < 1.0f) {
                    CTCCreatureFlags9* f; if (thing->GetInterface(&f)) f->m_canBeTargeted = 0;
                    CreateHeadEffect(); RemoveObstruction(); DoneDeathStuff = true;
                } else if (!IsStriking) { if (Tracking) TrackTarget(); else ReturnToDesiredPos(false); }
            }
            Tracking = false;
            if (QueuedStrikeTick > 0 && g_worldFrame >= QueuedStrikeTick) PerformStrikeDamage();
        }
    }
};
int main()
{
    bool ok = true;
    CTCCreatureFlags9 flags = { 1 }; CThing owner = { false, 2.0f, &flags, false }; CThing kraken = { false, 0, 0, false };
    Model m = { &owner, { &kraken }, true, true, false, 0, false };
    m.FrameUpdate(); ok = ok && strcmp(g_log, "BT") == 0 && !m.CanStrike && !m.Tracking;           // emerged: tracks the target, then clears Tracking
    g_log[0] = 0; m.FrameUpdate(); ok = ok && strcmp(g_log, "BR") == 0;                             // not tracking: returns to rest
    g_log[0] = 0; m.IsStriking = true; m.QueuedStrikeTick = 100; m.FrameUpdate(); ok = ok && strcmp(g_log, "BS") == 0;   // striking: no move; queued strike fires at its frame
    g_log[0] = 0; m.QueuedStrikeTick = 101; m.FrameUpdate(); ok = ok && strcmp(g_log, "B") == 0;    // not yet
    g_log[0] = 0; m.IsStriking = false; owner.EmergeFraction = 0.5f; m.FrameUpdate();
    ok = ok && strcmp(g_log, "BHO") == 0 && m.DoneDeathStuff && flags.m_canBeTargeted == 0;       // below one: head effect once, untargetable
    g_log[0] = 0; m.Tracking = true; m.FrameUpdate(); ok = ok && strcmp(g_log, "B") == 0;          // done: nothing more
    g_log[0] = 0; kraken.Dead = true; m.FrameUpdate(); ok = ok && strcmp(g_log, "BK") == 0 && owner.killed;   // dead kraken: the tentacle thing is killed
    g_log[0] = 0; m.PKraken.item = 0; m.FrameUpdate(); ok = ok && strcmp(g_log, "BK") == 0;
    if (!ok) { printf("007f14f0_TEST FAIL\n"); return 1; }
    printf("007f14f0_TEST PASS\n"); return 0;
}
