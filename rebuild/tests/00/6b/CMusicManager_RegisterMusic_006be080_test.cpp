// Behaviour test for CMusicManager::RegisterMusic (0x006BE080): every music def entry becomes a registered entry whose source
// and destination names are "Data\Sound\" + the string-table text with ".wma" replaced by ".ogg"; the map is sorted once at the end.
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

class CMusicEntry { public: unsigned int ID; unsigned int Filename; float Gain; bool CacheToHD; };
struct CRegisteredMusicEntry { std::string Src; std::string Dst; float Gain; float Offset; bool CacheToHD; };
struct CPair { int first; CRegisteredMusicEntry second; };
static bool PairLess(const CPair& a, const CPair& b) { return a.first < b.first; }

static std::string GetString(unsigned int id)
{
    static const char* table[] = { "music\\town.wma", "music\\cave.wma", "music\\boss.ogg" };
    return table[id];
}
static void ReplaceSubstring(std::string& s, const std::string& from, const std::string& to)
{
    std::string::size_type p = s.find(from);
    if (p != std::string::npos) s.replace(p, from.size(), to);
}

class CMusicManager { public:
    std::vector<CMusicEntry> Entries;
    std::vector<CPair> Registered;
    bool Dirty;
    int sorts;
    void RegisterMusic()
    {
        for (unsigned int i = 0; i < Entries.size(); ++i) {
            const CMusicEntry& entry = Entries[i];
            std::string src = std::string("Data\\Sound\\") + GetString(entry.Filename);
            std::string dst = std::string("Data\\Sound\\") + GetString(entry.Filename);
            ReplaceSubstring(src, ".wma", ".ogg");
            ReplaceSubstring(dst, ".wma", ".ogg");
            CPair p;
            p.first = (int)entry.ID;
            p.second.Src = src; p.second.Dst = dst; p.second.Gain = entry.Gain; p.second.Offset = 0.0f;
            p.second.CacheToHD = entry.CacheToHD;
            Registered.push_back(p);
            Dirty = true;
        }
        if (Dirty) { std::sort(Registered.begin(), Registered.end(), PairLess); ++sorts; Dirty = false; }
    }
};

int main()
{
    CMusicManager m;
    m.Dirty = false; m.sorts = 0;
    CMusicEntry e1 = { 30, 0, 0.5f, true };
    CMusicEntry e2 = { 10, 1, 1.0f, false };
    CMusicEntry e3 = { 20, 2, 0.25f, true };
    m.Entries.push_back(e1); m.Entries.push_back(e2); m.Entries.push_back(e3);
    m.RegisterMusic();
    bool ok = m.Registered.size() == 3 && m.sorts == 1 && !m.Dirty;
    ok = ok && m.Registered[0].first == 10 && m.Registered[1].first == 20 && m.Registered[2].first == 30;
    ok = ok && m.Registered[0].second.Src == "Data\\Sound\\music\\cave.ogg" && m.Registered[0].second.Dst == m.Registered[0].second.Src;
    ok = ok && m.Registered[2].second.Src == "Data\\Sound\\music\\town.ogg" && m.Registered[2].second.Gain == 0.5f;
    ok = ok && m.Registered[1].second.Src == "Data\\Sound\\music\\boss.ogg" && m.Registered[1].second.CacheToHD;
    ok = ok && m.Registered[0].second.Offset == 0.0f && !m.Registered[0].second.CacheToHD;
    CMusicManager empty;
    empty.Dirty = false; empty.sorts = 0;
    empty.RegisterMusic();
    ok = ok && empty.sorts == 0 && empty.Registered.empty();
    std::printf(ok ? "006be080_TEST PASS\n" : "006be080_TEST FAIL\n");
    return ok ? 0 : 1;
}