#include <cstdio>
#include <cstring>

// Model of CRuntimeScriptedEntityPersist::LoadFromGameStateScript: the persist map is emptied, the text is skipped up
// to the first START_NEW_ENTITY_SCRIPT, and every START_NEW_ENTITY_SCRIPT <uid> <script> END_NEW_ENTITY_SCRIPT block
// adds (uid, script) to the map and marks it dirty; a dirty map is sorted by uid once at the end.
struct CPersistInfo { unsigned long long UID; char PersistScript[64]; };
struct CEntry { unsigned long long first; CPersistInfo second; };

struct CParser {
    const char* s; unsigned int pos;
    void ReadAsStringUntilString(const char* end, char* out)
    {
        const char* hit = std::strstr(s + pos, end);
        unsigned int n = hit ? (unsigned int)(hit - (s + pos)) : (unsigned int)std::strlen(s + pos);
        if (out) { std::memcpy(out, s + pos, n); out[n] = 0; }
        pos += n;
    }
    void SkipWhitespace() { while (s[pos] == ' ' || s[pos] == '\n' || s[pos] == '\t') ++pos; }
    void SkipUntilWhitespace() { while (s[pos] && s[pos] != ' ' && s[pos] != '\n' && s[pos] != '\t') ++pos; }
    bool NextItemIs(const char* item) { SkipWhitespace(); return std::strncmp(s + pos, item, std::strlen(item)) == 0; }
    bool SkipPastNextItemIfIs(const char* item)
    {
        if (!NextItemIs(item)) return false;
        pos += (unsigned int)std::strlen(item);
        return true;
    }
    unsigned long long ReadInt64()
    {
        SkipWhitespace();
        unsigned long long v = 0;
        while (s[pos] >= '0' && s[pos] <= '9') v = v * 10 + (unsigned long long)(s[pos++] - '0');
        return v;
    }
};

struct CPersist {
    CEntry data[8]; int count; bool Dirty;
    void LoadFromGameStateScript(const char* text)
    {
        count = 0;
        CParser parser; parser.s = text; parser.pos = 0;
        parser.ReadAsStringUntilString("START_NEW_ENTITY_SCRIPT", 0);
        while (parser.SkipPastNextItemIfIs("START_NEW_ENTITY_SCRIPT")) {
            parser.SkipUntilWhitespace();
            CPersistInfo info;
            info.UID = parser.ReadInt64();
            parser.SkipWhitespace();
            parser.ReadAsStringUntilString("END_NEW_ENTITY_SCRIPT", info.PersistScript);
            data[count].first = info.UID; data[count].second = info; ++count;
            Dirty = true;
            parser.SkipPastNextItemIfIs("END_NEW_ENTITY_SCRIPT");
        }
        if (Dirty) {
            for (int i = 1; i < count; ++i)
                for (int j = i; j > 0 && data[j].first < data[j - 1].first; --j) { CEntry t = data[j]; data[j] = data[j - 1]; data[j - 1] = t; }
            Dirty = false;
        }
    }
};

int main()
{
    CPersist p; p.Dirty = false; p.count = 5;
    p.LoadFromGameStateScript("header stuff\nSTART_NEW_ENTITY_SCRIPT 900 alpha;END_NEW_ENTITY_SCRIPT\n"
                              "START_NEW_ENTITY_SCRIPT 12 beta;END_NEW_ENTITY_SCRIPT\nSTART_NEW_ENTITY_SCRIPT 77 gamma;END_NEW_ENTITY_SCRIPT\ntrailer");
    if (p.count != 3 || p.Dirty) { std::printf("00cb8440_TEST FAIL\n"); return 1; }
    if (p.data[0].first != 12 || p.data[1].first != 77 || p.data[2].first != 900 || p.data[2].second.UID != 900
        || std::strcmp(p.data[0].second.PersistScript, "beta;") != 0 || std::strcmp(p.data[2].second.PersistScript, "alpha;") != 0)
        { std::printf("00cb8440_TEST FAIL\n"); return 1; }
    p.LoadFromGameStateScript("nothing here");
    if (p.count != 0 || p.Dirty) { std::printf("00cb8440_TEST FAIL\n"); return 1; }
    std::printf("00cb8440_TEST PASS\n");
    return 0;
}
