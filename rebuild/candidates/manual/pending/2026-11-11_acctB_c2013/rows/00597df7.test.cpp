// Behaviour test for 0x00597DF7 NUISystem::CFrontEndManager::GetSavedGameNames(std::vector<CWideString>&): a self-contained host
// model of the row's logic (wide text as unsigned short, hand-rolled containers; plain C++98 for g++ and VC7.1).
#include <cstdio>
#include <cstring>

struct WStr {                                   // CWideString stand-in (unsigned short text)
    unsigned short t[32];
    WStr() { t[0] = 0; }
    WStr(const char* s) { int i = 0; for (; s[i] && i < 31; ++i) t[i] = (unsigned short)(unsigned char)s[i]; t[i] = 0; }
    bool empty() const { return t[0] == 0; }
    bool operator==(const WStr& o) const { int i = 0; for (; t[i] && t[i] == o.t[i]; ++i) {} return t[i] == o.t[i]; }
    bool operator!=(const WStr& o) const { return !(*this == o); }
    bool EqualsNoCase(const WStr& o) const {
        int i = 0;
        for (;; ++i) {
            unsigned short a = t[i], b = o.t[i];
            if (a >= 'A' && a <= 'Z') a = (unsigned short)(a + 32);
            if (b >= 'A' && b <= 'Z') b = (unsigned short)(b + 32);
            if (a != b) return false;
            if (!a) return true;
        }
    }
};
struct Time { long stamp; Time() : stamp(0) {} };

template<class T> struct Vec {                  // vector stand-in
    T v[16]; int n;
    Vec() : n(0) {}
    void clear() { n = 0; }
    void resize(int m) { for (int i = n; i < m; ++i) v[i] = T(); n = m; }
    void push_back(const T& x) { v[n++] = x; }
    int erase(int i) { for (int k = i; k + 1 < n; ++k) v[k] = v[k + 1]; --n; return i; }
    T& operator[](int i) { return v[i]; }
    int size() const { return n; }
};

struct Profile {                                // CUserProfileManager stand-in
    WStr empty_text; Profile() : empty_text("EMPTY") {}
    bool debug; WStr autosave; long max_slots; WStr slots[8]; Vec<WStr> sorted; Vec<Time> sorted_times;
    void GetSortedSaveNamesAndTimes(Vec<WStr>& names, Vec<Time>& times) { names = sorted; times = sorted_times; }
    bool IsDebugProfile() const { return debug; }
    WStr GetAutoSaveFileName() const { return autosave; }
    long GetMaxSaveSlots() const { return max_slots; }
    const WStr& GetSaveSlotFileName(long i) const { return slots[i]; }
    WStr GetEmptySlotName() const { return empty_text; }
    WStr GetPathNameFromFileName(const WStr& name, bool) const {   // a name starting with 'm' is "missing": empty path
        if (name.t[0] == 'm') return WStr();
        WStr p("P"); int i = 0; for (; name.t[i] && i < 30; ++i) p.t[i + 1] = name.t[i]; p.t[i + 1] = 0; return p;
    }
};
static Time ModifiedTime(const WStr& path) { Time t; t.stamp = 100 + path.t[1]; return t; }

struct FrontEnd {                               // CFrontEndManager stand-in
    Profile* profile;
    Vec<Time> m_vFileTimes; Vec<int> FileRegionNames, FileRegionMiniMapNames; Vec<long> FilePlayTimes;
    void AddRegionAndTimeInfo(const WStr& path) { FileRegionNames.push_back(path.t[1]); FileRegionMiniMapNames.push_back(path.t[1]); FilePlayTimes.push_back(7); }
    void Blank() { FileRegionNames.push_back(0); FileRegionMiniMapNames.push_back(0); FilePlayTimes.push_back(0); }
    void GetSavedGameNames(Vec<WStr>& names) {
        names.clear(); m_vFileTimes.clear(); FileRegionNames.clear(); FileRegionMiniMapNames.clear(); FilePlayTimes.clear();
        profile->GetSortedSaveNamesAndTimes(names, m_vFileTimes);
        if (profile->IsDebugProfile()) {
            for (long i = 0; i < (long)names.size(); ++i) {
                if (names[i] != WStr("")) {
                    WStr path = profile->GetPathNameFromFileName(names[i], false);
                    if (path != WStr("")) { AddRegionAndTimeInfo(path); m_vFileTimes[i] = ModifiedTime(path); }
                    else { m_vFileTimes[i] = Time(); Blank(); }
                } else { names[i] = profile->GetEmptySlotName(); m_vFileTimes[i] = Time(); Blank(); }
            }
        } else {
            bool found_autosave = false;
            WStr autosave_name = profile->GetAutoSaveFileName();
            for (int it = 0; it != names.size(); ++it)
                if (names[it].EqualsNoCase(autosave_name)) { found_autosave = true; break; }
            long first_slot = 0;
            if (found_autosave) first_slot = 1;
            long max_slots = profile->GetMaxSaveSlots();
            names.resize(max_slots + first_slot);
            m_vFileTimes.resize(max_slots + first_slot);
            if (found_autosave) {
                names[0] = profile->GetAutoSaveFileName();
                WStr path = profile->GetPathNameFromFileName(profile->GetAutoSaveFileName(), false);
                if (path != WStr("")) { m_vFileTimes[0] = ModifiedTime(path); AddRegionAndTimeInfo(path); }
                else { m_vFileTimes[0] = Time(); Blank(); }
            }
            for (long i = first_slot; i < max_slots + first_slot; ++i) {
                names[i] = profile->GetSaveSlotFileName(i - first_slot);
                if (names[i] != WStr("")) {
                    WStr path = profile->GetPathNameFromFileName(names[i], false);
                    if (path != WStr("")) { m_vFileTimes[i] = ModifiedTime(path); AddRegionAndTimeInfo(path); }
                    else { m_vFileTimes[i] = Time(); Blank(); }
                } else { names[i] = profile->GetEmptySlotName(); m_vFileTimes[i] = Time(); Blank(); }
            }
            int time_it = 0;
            for (int name_it = 0; name_it != names.size() && time_it != m_vFileTimes.size(); ) {
                if (names[name_it] == WStr("")) { name_it = names.erase(name_it); time_it = m_vFileTimes.erase(time_it); }
                else { ++name_it; ++time_it; }
            }
        }
    }
};

static int fails = 0;
static void check(bool c, int line) { if (!c) { std::printf("check failed at line %d\n", line); ++fails; } }
#define CHECK(c) check((c), __LINE__)

int main()
{
    Profile p; FrontEnd f; f.profile = &p; Vec<WStr> names;
    // debug profile: every listed name is kept; an empty name becomes the empty-slot text
    p.debug = true; p.sorted.push_back(WStr("a")); p.sorted.push_back(WStr("")); p.sorted.push_back(WStr("mb"));
    p.sorted_times.resize(3);
    f.GetSavedGameNames(names);
    CHECK(names.size() == 3 && names[0] == WStr("a") && names[1] == WStr("EMPTY") && names[2] == WStr("mb"));
    CHECK(f.m_vFileTimes[0].stamp == 100 + 'a' && f.m_vFileTimes[1].stamp == 0 && f.m_vFileTimes[2].stamp == 0);
    CHECK(f.FileRegionNames.size() == 3 && f.FileRegionNames[0] == 'a' && f.FileRegionNames[1] == 0 && f.FilePlayTimes[0] == 7);
    // normal profile with the autosave listed (case-insensitive): autosave first, then the slots
    p.debug = false; p.sorted.clear(); p.sorted.push_back(WStr("AutoSave")); p.sorted.push_back(WStr("x"));
    p.autosave = WStr("autosave"); p.max_slots = 3; p.slots[0] = WStr("s1"); p.slots[1] = WStr(""); p.slots[2] = WStr("ms3");
    f.GetSavedGameNames(names);
    CHECK(names.size() == 4 && f.m_vFileTimes.size() == 4);
    CHECK(names[0] == WStr("autosave") && names[1] == WStr("s1") && names[2] == WStr("EMPTY") && names[3] == WStr("ms3"));
    CHECK(f.m_vFileTimes[0].stamp == 100 + 'a' && f.m_vFileTimes[1].stamp == 100 + 's' && f.m_vFileTimes[3].stamp == 0);
    CHECK(f.FileRegionNames.size() == 4 && f.FileRegionNames[0] == 'a' && f.FileRegionNames[1] == 's' && f.FileRegionNames[3] == 0);
    // no autosave listed: only the slots; an empty slot name is the empty-slot text, so nothing is swept
    p.sorted.clear(); p.sorted.push_back(WStr("x"));
    f.GetSavedGameNames(names);
    CHECK(names.size() == 3 && names[0] == WStr("s1") && names[1] == WStr("EMPTY") && f.FilePlayTimes.size() == 3);
    // the final sweep drops names that are still empty (empty-slot text itself empty) together with their times
    p.empty_text = WStr(""); p.sorted.push_back(WStr("autosave"));
    f.GetSavedGameNames(names);
    CHECK(names.size() == 3 && f.m_vFileTimes.size() == 3);
    CHECK(names[0] == WStr("autosave") && names[1] == WStr("s1") && names[2] == WStr("ms3"));
    CHECK(f.m_vFileTimes[0].stamp == 100 + 'a' && f.m_vFileTimes[1].stamp == 100 + 's' && f.m_vFileTimes[2].stamp == 0);
    if (fails) { std::printf("00597df7_TEST FAIL\n"); return 1; }
    std::printf("00597df7_TEST PASS\n");
    return 0;
}
