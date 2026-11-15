// Behaviour test for CTCInGameMenu::OpenInGameMenuPC @ 0x004804DE: the save / load list logic of the row, through a copy of its
// control flow over a mock profile (empty-slot / auto-save / manual names, display names, signature verification), a mock quest
// state and a mock fishing TC. Checks the save-list offsets (debug profile: all slots, else one fewer), the used-slot count of the
// load list, which slots get a save entry and of which kind (overwrite sub-list / confirm sub-list / direct save action) with
// its template, the action set of each accept / back sub-list (OBSERVE_EVENT x2 on selection, IGNORE_EVENT x2 on inactive, SAVE
// or LOAD on accept, SEND_EVENT(BACK) on back), which frame receives the sub-list's containers, which load entries are
// unavailable or get a load-confirm sub-list, the save menu entry gating, and the yes / no dialog's centred position.
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
struct World { std::set<Str> valid, exists; bool questScreens, fishing; float width, height, scaleX, scaleY; };
struct Result {
    unsigned long saveOffsets, loadOffsets; std::vector<Str> saveEntries, loadEntries; bool saveMenuAvailable;
    float dialogX, dialogY;
};
// the six actions of an accept / back sub-list (row: observeAccept .. backAction), and the frame its containers go to
static Str SubListActions(const Str& acceptType, const Str& frame)
{
    Str s = "SEL:OBSERVE(accept),OBSERVE(back);INACTIVE:IGNORE(accept),IGNORE(back);";
    s += "ACCEPT:" + acceptType + ";BACK:SEND_EVENT(BACK);FRAME:" + frame;
    return s;
}
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
            Str tmpl = (isEmpty && !w.questScreens) ? "PC_MENU_ENTRY_TEMPLATE" : "PC_MENU_ENTRY_BRANCH_TEMPLATE";
            Str kind;
            if (!isEmpty) kind = "OVERWRITE{" + SubListActions("SAVE", "save") + "}";
            else if (w.questScreens) kind = "CONFIRM{" + SubListActions("SAVE", "load") + "}";
            else kind = "SAVE";
            r.saveEntries.push_back(name + "|" + tmpl + "|" + kind);
        }
        name = p.GetSaveDisplayNameFromFileName(name, i);
        Str load = name + (ok ? "|AVAILABLE" : "|UNAVAILABLE");
        if (name != p.GetEmptySlotName()) {
            load += "|LISTED";
            if (ok) load += "|CONFIRM{" + SubListActions("LOAD", "load") + "}";
        }
        r.loadEntries.push_back(load);
    }
    r.saveMenuAvailable = !noSaveEntries && !w.fishing;
    r.dialogY = w.height * 0.5f - w.scaleY * 52.0f;
    r.dialogX = (w.width * 0.5f) - w.scaleX * 104.0f;
    return r;
}
static int gFail = 0;
static void Check(bool c, const char* what) { if (!c) { std::printf("fail: %s\n", what); gFail = 1; } }
int main()
{
    std::vector<Str> files; files.push_back("AUTO"); files.push_back("s1"); files.push_back("EMPTY"); files.push_back("s3");
    Profile p; p.debug = false;
    World w; w.questScreens = false; w.fishing = false; w.width = 1024.0f; w.height = 768.0f; w.scaleX = 1.0f; w.scaleY = 2.0f;
    w.valid.insert("P/AUTO"); w.valid.insert("P/s1"); w.valid.insert("P/EMPTY"); w.valid.insert("P/s3");
    w.exists.insert("P/s3.hs");                                        // s3 has a signature file that does not verify
    Result r = OpenModel(files, p, w);
    const Str saveOverwrite = "OVERWRITE{" + SubListActions("SAVE", "save") + "}";
    Check(r.saveOffsets == 3, "save offsets = slots - 1");
    Check(r.loadOffsets == 3, "load offsets = used slots");
    Check(r.saveEntries.size() == 3, "auto-save slot has no save entry");
    Check(r.saveEntries.size() == 3 && r.saveEntries[0] == "D:SAVE1|PC_MENU_ENTRY_BRANCH_TEMPLATE|" + saveOverwrite, "manual slot: overwrite");
    Check(r.saveEntries.size() == 3 && r.saveEntries[1] == "EMPTY|PC_MENU_ENTRY_TEMPLATE|SAVE", "empty slot: direct save");
    Check(r.saveEntries.size() == 3 && r.saveEntries[2] == "D:SAVE3|PC_MENU_ENTRY_BRANCH_TEMPLATE|" + saveOverwrite, "slot 3: overwrite");
    Check(r.loadEntries[0] == "D:AUTO|AVAILABLE|LISTED|CONFIRM{" + SubListActions("LOAD", "load") + "}", "auto load entry");
    Check(r.loadEntries[1] == "D:D:SAVE1|AVAILABLE|LISTED|CONFIRM{" + SubListActions("LOAD", "load") + "}", "manual load entry");
    Check(r.loadEntries[2] == "EMPTY|AVAILABLE", "empty load entry not listed");
    Check(r.loadEntries[3] == "D:D:SAVE3|UNAVAILABLE|LISTED", "bad signature: unavailable, no confirm");
    Check(r.saveMenuAvailable, "save menu available");
    Check(r.dialogX == 512.0f - 104.0f && r.dialogY == 384.0f - 104.0f, "yes / no dialog centred by the UI scale");
    p.debug = true; w.questScreens = true; w.fishing = true;
    r = OpenModel(files, p, w);
    Check(r.saveOffsets == 4, "debug profile: all slots");
    Check(r.saveEntries.size() == 3 && r.saveEntries[1] == "EMPTY|PC_MENU_ENTRY_BRANCH_TEMPLATE|CONFIRM{" + SubListActions("SAVE", "load") + "}",
          "quest screens: confirm sub-list, containers in the load frame");
    Check(r.saveEntries.size() == 3 && r.saveEntries[0] == "D:s1|PC_MENU_ENTRY_BRANCH_TEMPLATE|" + saveOverwrite, "debug profile keeps file names");
    Check(!r.saveMenuAvailable, "fishing: save menu unavailable");
    std::vector<Str> autoOnly(1, "AUTO"); p.debug = false; w.fishing = false;
    r = OpenModel(autoOnly, p, w);
    Check(r.saveEntries.empty() && !r.saveMenuAvailable, "only auto-save: no save entries");
    if (gFail) { std::printf("004804de_TEST FAIL\n"); return 1; }
    std::printf("004804de_TEST PASS\n");
    return 0;
}