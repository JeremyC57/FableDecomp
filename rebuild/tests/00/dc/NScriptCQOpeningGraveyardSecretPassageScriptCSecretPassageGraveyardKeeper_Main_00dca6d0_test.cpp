// Behaviour test for NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper::Main (0x00DCA6D0): the
// keeper is made untouchable until the keeper cut-scene starts, meets the hero once (CS_OPENGRAVE_KEEPERMEET, doors_open), then
// each frame: complains when hit, gives the pre-skeleton directions (FIRST once, then SECOND) or the post-skeleton mumbling cycle
// (FIRST, SECOND, THIRD, FOURTH, THIRD, FOURTH ...), speaks the text of a nearby talk tag unless the door changed and removes the
// tag, and comments once on each ceremonial artifact found (helmet, armour, sword, shield, in that order).
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Keeper {
    int Frames, TerminateAt, HitMask, TalkMask, TagMask, SkeletonAt, ArtifactAt[4];
    bool DoorsOpen, DoorChange, Found[4], Killable;
    long TalkCount;
    bool T() const { return Frames >= TerminateAt; }
    void NSF() { ++Frames; }
    bool Bit(int m) const { return Frames < 31 && ((m >> Frames) & 1); }
    void Main() {
        if (T()) return;
        Killable = false;
        Killable = true;
        if (!DoorsOpen) { Log("KEEPERMEET"); DoorsOpen = true; }
        TalkCount = 0;
        static const char* const art[4] = { "HELM", "ARMOUR", "SWORD", "SHIELD" };
        for (; !T(); NSF()) {
            if (Bit(HitMask)) Log("HIT");
            if (Bit(TalkMask)) {
                if (Frames >= SkeletonAt) {
                    if (TalkCount < 2) { Log("POST_FIRST"); TalkCount = 2; }
                    else if (TalkCount == 2) { TalkCount = 3; Log("POST_SECOND"); }
                    else if (TalkCount == 3) { TalkCount = 4; Log("POST_THIRD"); }
                    else if (TalkCount == 4) { TalkCount = 3; Log("POST_FOURTH"); }
                } else if (TalkCount == 0) { Log("PRE_FIRST"); TalkCount = 1; }
                else { TalkCount = 1; Log("PRE_SECOND"); }
            }
            if (Bit(TagMask)) { if (!DoorChange) Log("tag_text"); Log("remove_tag"); }
            for (int i = 0; i < 4; ++i)
                if (!Found[i] && Frames >= ArtifactAt[i]) { Found[i] = true; Log(art[i]); }
        }
    }
};
int main()
{
    Keeper a = { 0, 12, (1 << 1), (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10), (1 << 4), 5,
                 { 11, 99, 7, 99 }, false, false, { false, false, false, false }, false, 0 };
    a.Main();
    bool ok = !std::strcmp(g_log, "KEEPERMEET;HIT;PRE_FIRST;PRE_SECOND;tag_text;remove_tag;POST_FIRST;POST_SECOND;POST_THIRD;SWORD;"
                                  "POST_FOURTH;POST_THIRD;POST_FOURTH;HELM;")
              && a.DoorsOpen && a.Found[0] && a.Found[2] && !a.Found[1] && a.TalkCount == 3 && a.Frames == 12;
    g_log[0] = 0;
    Keeper b = { 0, 3, 0, 0, (1 << 1), 99, { 0, 0, 99, 99 }, true, true, { true, false, false, false }, false, 0 };
    b.Main();
    ok = ok && !std::strcmp(g_log, "ARMOUR;remove_tag;") && b.Found[1] && b.Frames == 3;
    g_log[0] = 0;
    Keeper c = { 0, 0, 0, 0, 0, 0, { 0, 0, 0, 0 }, false, false, { false, false, false, false }, false, 0 };
    c.Main();
    ok = ok && g_log[0] == 0 && !c.DoorsOpen;
    std::printf(ok ? "00dca6d0_TEST PASS\n" : "00dca6d0_TEST FAIL\n");
    return ok ? 0 : 1;
}