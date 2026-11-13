// Behaviour test for CTCInGameMenu::OpenInGameMenu @ 0x004830A1: the save / load list logic of the row, through a copy of its
// control flow over a mock profile (empty-slot / auto-save / manual names, display names, signature verification), a mock quest
// state and a mock fishing TC. Checks the save-list offsets (debug profile: all slots, else one fewer), the used-slot count of the
// load list, which slots get a save entry and of which kind (overwrite sub-list / confirm sub-list / direct save action), the
// entry template, which load entries are unavailable or get a load-confirm sub-list, and the save menu entry gating.
#include <cstdio>
#include <string>
#include <vector>
#include <set>
typedef std::string Str;
static const Str kEmpty = "EMPTY", kAuto = "AUTO";
struct Profile {
    bool debug;
    Str GetEmptySlotName() const { return kEmpty; }
    Str GetAutoSaveFileName() const { return kAuto; }
    Str GetManualSaveFileName(long i) const { char b[16]; std::sprintf(b, "SAVE%ld", i); return b; }
    Str GetPathNameFromFileName(const Str& n, bool) const { return "P/" + n; }
    Str GetSaveDisplayNameFromFileName(const Str& n, long) const { return n == kEmpty ? n : "D:" + n; }
    bool IsDebugProfile() const { return debug; }
};
struct World { std::set<Str> valid, exists; bool questScreens, fishing; };
struct Result {
    unsigned long saveOffsets, loadOffsets; std::vector<Str> saveEntries, loadEntries; bool saveMenuAvailable;
};
// the row's statements, over the mocks (same order and conditions as the row source)
static Result OpenModel(const std::vector<Str>& FileNames, const Profile& p, const World& w)
{
    Result r;
    if (p.IsDebugProfile()) r.saveOffsets = FileNames.size(); else r.saveOffsets = FileNames.size() - 1;
    long usedSlots = 0;
    for (unsigned long j = 0; j < FileNames.size(); ++j)
        if (FileNames[j] != p.GetEmptySlotName()) ++usedSlots;
    r.loadOffsets = usedSlots;
    bool noSaveEntries = true;
    for (unsigned long i = 0; i < FileNames.size(); ++i) {
        Str name(FileNames[i]);
        bool hasSignature = false;
        Str signaturePath = p.GetPathNameFromFileName(FileNames[i], false) + ".hs";
        if (w.exists.count(signaturePath)) hasSignature = true;
        bool ok = w.valid.count(p.GetPathNameFromFileName(FileNames[i], false)) && (!hasSignature || w.valid.count(signaturePath));
        if (!p.IsDebugProfile() && name != p.GetEmptySlotName() && name != p.GetAutoSaveFileName())
            name = p.GetManualSaveFileName(i);
        if (name != p.GetAutoSaveFileName()) {
            noSaveEntries = false;
            if (name == "") name = p.GetEmptySlotName(); else name = p.GetSaveDisplayNameFromFileName(name, i);
            bool isEmpty = name == p.GetEmptySlotName();
            Str tmpl = (isEmpty && !w.questScreens) ? "ACTION" : "BRANCH";
            Str kind = !isEmpty ? "OVERWRITE" : w.questScreens ? "CONFIRM" : "SAVE";
            r.saveEntries.push_back(name + "|" + tmpl + "|" + kind);
        }
        name = p.GetSaveDisplayNameFromFileName(name, i);
        Str load = name + (ok ? "|AVAILABLE" : "|UNAVAILABLE");
        if (name != p.GetEmptySlotName()) {
            load += "|LISTED";
            if (ok) load += "|CONFIRM";
        }
        r.loadEntries.push_back(load);
    }
    r.saveMenuAvailable = !noSaveEntries && !w.fishing;
    return r;
}
static int gFail = 0;
static void Check(bool c, const char* what) { if (!c) { std::printf("fail: %s\n", what); gFail = 1; } }
int main()
{
    std::vector<Str> files; files.push_back("AUTO"); files.push_back("s1"); files.push_back("EMPTY"); files.push_back("s3");
    Profile p; p.debug = false;
    World w; w.questScreens = false; w.fishing = false;
    w.valid.insert("P/AUTO"); w.valid.insert("P/s1"); w.valid.insert("P/EMPTY"); w.valid.insert("P/s3");
    w.exists.insert("P/s3.hs");                                        // s3 has a signature file that does not verify
    Result r = OpenModel(files, p, w);
    Check(r.saveOffsets == 3, "save offsets = slots - 1");
    Check(r.loadOffsets == 3, "load offsets = used slots");
    Check(r.saveEntries.size() == 3, "auto-save slot has no save entry");
    Check(r.saveEntries.size() == 3 && r.saveEntries[0] == "D:SAVE1|BRANCH|OVERWRITE", "manual slot: overwrite");
    Check(r.saveEntries.size() == 3 && r.saveEntries[1] == "EMPTY|ACTION|SAVE", "empty slot: direct save");
    Check(r.saveEntries.size() == 3 && r.saveEntries[2] == "D:SAVE3|BRANCH|OVERWRITE", "slot 3: overwrite");
    Check(r.loadEntries[0] == "D:AUTO|AVAILABLE|LISTED|CONFIRM", "auto load entry");
    Check(r.loadEntries[1] == "D:D:SAVE1|AVAILABLE|LISTED|CONFIRM", "manual load entry");
    Check(r.loadEntries[2] == "EMPTY|AVAILABLE", "empty load entry not listed");
    Check(r.loadEntries[3] == "D:D:SAVE3|UNAVAILABLE|LISTED", "bad signature: unavailable, no confirm");
    Check(r.saveMenuAvailable, "save menu available");
    p.debug = true; w.questScreens = true; w.fishing = true;
    r = OpenModel(files, p, w);
    Check(r.saveOffsets == 4, "debug profile: all slots");
    Check(r.saveEntries.size() == 3 && r.saveEntries[1] == "EMPTY|BRANCH|CONFIRM", "quest screens: confirm sub-list");
    Check(r.saveEntries.size() == 3 && r.saveEntries[0] == "D:s1|BRANCH|OVERWRITE", "debug profile keeps file names");
    Check(!r.saveMenuAvailable, "fishing: save menu unavailable");
    std::vector<Str> autoOnly(1, "AUTO"); p.debug = false; w.fishing = false;
    r = OpenModel(autoOnly, p, w);
    Check(r.saveEntries.empty() && !r.saveMenuAvailable, "only auto-save: no save entries");
    if (gFail) { std::printf("004830a1_TEST FAIL\n"); return 1; }
    std::printf("004830a1_TEST PASS\n");
    return 0;
}
