// Behaviour test for NScript::CQ_HobbeCaveScript::Main (0x00D8DD10): the first objective depends on SavedBeforeCaves, fifteen entity
// bindings (Granny not persistent), four threads, two sub-quests activated, a wait for GrannysHouse (skipped once saved), the start
// screen once, then a key wait that auto-saves on reaching GreatwoodCaves with the key and ends in the entrance tunnel with the key,
// where the third objective is set and ReachedHobbeCave raised.
#include <cstdio>
#include <cstring>
#include <string>
struct Sim {
    std::string log; int frame, termFrame;
    bool SavedBeforeCaves, QuestStartScreened, ReachedHobbeCave;
    int grannyFrame, cavesFrame, tunnelFrame, keyFrame, bindings, persistent, threads;
    bool Term() const { return frame >= termFrame; }
    bool RegionLoaded(const char* r) const
    {
        if (!std::strcmp(r, "GrannysHouse")) return frame >= grannyFrame;
        if (!std::strcmp(r, "GreatwoodCaves")) return frame >= cavesFrame && frame < tunnelFrame;
        return frame >= tunnelFrame;
    }
    bool HasKey() const { return frame >= keyFrame; }
    void Ev(const char* e) { log += e; log += ';'; }
    void Bind(bool flag) { ++bindings; if (flag) ++persistent; }
    void Main()
    {
        if (!SavedBeforeCaves) { if (Term()) return; Ev("OBJ01"); } else { if (Term()) return; Ev("OBJ02"); }
        Bind(false); for (int i = 0; i < 14; ++i) Bind(true);
        Ev("POST"); threads = 4; Ev("ACT_BARRIER"); Ev("ACT_GRANNY");
        while (!SavedBeforeCaves && !RegionLoaded("GrannysHouse")) { ++frame; if (Term()) return; }
        if (Term()) return;
        if (!QuestStartScreened) { if (Term()) return; Ev("START"); QuestStartScreened = true; }
        if (Term()) return;
        for (;;) {
            if (RegionLoaded("HobbeCaveEntranceTunnel")) { if (Term()) return; if (HasKey()) break; }
            if (!SavedBeforeCaves && RegionLoaded("GreatwoodCaves")) {
                if (Term()) return;
                if (HasKey()) { if (Term()) return; SavedBeforeCaves = true; Ev("AUTOSAVE"); }
            }
            ++frame; if (Term()) return;
        }
        if (Term()) return;
        Ev("OBJ03"); ReachedHobbeCave = true;
    }
};
int main()
{
    Sim a;
    a.frame = 0; a.termFrame = 100; a.SavedBeforeCaves = false; a.QuestStartScreened = false; a.ReachedHobbeCave = false;
    a.grannyFrame = 3; a.cavesFrame = 5; a.keyFrame = 6; a.tunnelFrame = 8; a.bindings = a.persistent = a.threads = 0;
    a.Main();
    bool ok = a.log == "OBJ01;POST;ACT_BARRIER;ACT_GRANNY;START;AUTOSAVE;OBJ03;" && a.ReachedHobbeCave && a.SavedBeforeCaves
        && a.bindings == 15 && a.persistent == 14 && a.threads == 4 && a.frame == 8;
    Sim b = a; b.log = ""; b.frame = 0; b.termFrame = 4; b.SavedBeforeCaves = true; b.QuestStartScreened = true; b.ReachedHobbeCave = false;
    b.grannyFrame = 50; b.tunnelFrame = 50; b.bindings = b.persistent = 0;
    b.Main();
    ok = ok && b.log == "OBJ02;POST;ACT_BARRIER;ACT_GRANNY;" && !b.ReachedHobbeCave && b.frame == 4;
    std::printf(ok ? "00d8dd10_TEST PASS\n" : "00d8dd10_TEST FAIL\n");
    return ok ? 0 : 1;
}