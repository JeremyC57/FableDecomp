// Behaviour test for 0x008508B0 CContextSensitiveItemDef::TargetCreatureLikelyToReactToDeed (see src/008508b0.cpp): false
// when the target's 0x1F component is stopped/flagged at level >= 4 or its 0x4F table does not react to the deed, false when
// the target is not in SG_DEED_REACTION_ROOT, false for deed-reaction items (1..0x5B) when the target's 0x0F component is
// idle and the hero's deed-effects def lacks AllowSpeechOnNonPureAISpeaker (or the hero has no deed log); else true.
#include <cstdio>
struct C1F { bool Flag18; bool Running; long Level; };
struct C4F { unsigned char Reacts[8]; };
struct C0F { bool Busy; };
struct Target { C1F* c1f; C4F* c4f; C0F* c0f; bool InGroup; };
struct Hero { bool HasLog; bool Allow[8]; };
static bool Likely(long extra2, Hero* h, Target* t, int deed)
{
    if ((t->c1f && (t->c1f->Flag18 || !t->c1f->Running) && t->c1f->Level >= 4) || (t->c4f && !t->c4f->Reacts[deed]))
        return false;
    if (!t->InGroup) return false;
    if (extra2 > 0 && extra2 < 0x5c) {
        if (t->c0f && !t->c0f->Busy && (!h->HasLog || !h->Allow[deed])) return false;
    }
    return true;
}
int main()
{
    C1F busy = { false, false, 5 }, fine = { false, true, 5 };
    C4F r = { { 1, 0, 1, 1, 1, 1, 1, 1 } };
    C0F idle = { false };
    Target t = { &fine, &r, &idle, true };
    Hero h = { true, { 1, 1, 0, 1, 1, 1, 1, 1 } };
    bool ok = Likely(3, &h, &t, 0) && !Likely(3, &h, &t, 1) && !Likely(3, &h, &t, 2) && Likely(0, &h, &t, 2);
    t.c1f = &busy; ok = ok && !Likely(0, &h, &t, 0);
    t.c1f = 0; t.InGroup = false; ok = ok && !Likely(0, &h, &t, 0);
    t.InGroup = true; h.HasLog = false; ok = ok && !Likely(5, &h, &t, 0) && Likely(0x5c, &h, &t, 0);
    std::printf(ok ? "008508b0_TEST PASS\n" : "008508b0_TEST FAIL\n");
    return ok ? 0 : 1;
}