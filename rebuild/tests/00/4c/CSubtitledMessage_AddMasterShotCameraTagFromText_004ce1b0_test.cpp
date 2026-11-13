// Behaviour test for CSubtitledMessage::AddMasterShotCameraTagFromText 0x004CE1B0: the same parse / lookup / insert logic over a
// small model of CStringParser (skip whitespace, match an item, read until a delimiter) and the symbol map.
#include <cstdio>
#include <cstring>
#include <string>
#include <map>
#include <vector>
#include <utility>

struct Parser {
    std::string s; size_t pos;
    Parser(const std::string& t) : s(t), pos(0) { }
    void SkipWhitespace() { while (pos < s.size() && (s[pos] == ' ' || s[pos] == '\t')) ++pos; }
    bool SkipPastNextItemIfIs(const char* item) {
        SkipWhitespace(); size_t n = std::strlen(item);
        if (s.compare(pos, n, item) == 0) { pos += n; return true; }
        return false;
    }
    std::string ReadAsStringUntilString(const char* stop) {
        size_t e = s.find(stop, pos); if (e == std::string::npos) e = s.size();
        std::string r = s.substr(pos, e - pos); while (!r.empty() && r[r.size()-1] == ' ') r.erase(r.size()-1);
        pos = e; return r;
    }
};
std::map<std::string, long> g_symbols;
bool FindSymbol(const std::string& n, long& v) { std::map<std::string,long>::iterator i = g_symbols.find(n); if (i == g_symbols.end()) return false; v = i->second; return true; }
struct Tag { long posPerson, position, focusPerson, focus, zoom, view; };
struct Msg { std::vector<std::pair<float, Tag> > tags; bool dirty; Msg() : dirty(false) { } };

bool AddMasterShot(Msg& m, float time, const std::string& text)
{
    Tag tag; tag.posPerson = 0; tag.focusPerson = 0;
    long position, focus;
    Parser parser(text);
    parser.SkipWhitespace();
    if (!parser.SkipPastNextItemIfIs("(")) return false;
    std::string first = parser.ReadAsStringUntilString(",");
    if (!parser.SkipPastNextItemIfIs(",")) return false;
    parser.SkipWhitespace();
    std::string second = parser.ReadAsStringUntilString(",");
    if (!parser.SkipPastNextItemIfIs(",")) return false;
    parser.SkipWhitespace();
    std::string third = parser.ReadAsStringUntilString(")");
    if (!parser.SkipPastNextItemIfIs(")")) return false;
    if (FindSymbol(first, position)) {
        tag.position = position;
        if (!FindSymbol(second, focus)) return false;
        tag.focus = focus; tag.zoom = 0; tag.view = 1;
        if (third != "") { long view; if (FindSymbol(third, view)) tag.view = view; else return false; }
        m.tags.push_back(std::make_pair(time, tag)); m.dirty = true;
        return true;
    }
    return false;
}

int main()
{
    g_symbols["CONVERSATION_CAMERA_LEFT_SHOULDER"] = 1;
    g_symbols["CONVERSATION_CAMERA_FOCUS_50"] = 4;
    g_symbols["CONVERSATION_CAMERA_VIEW_CLOSE"] = 2;
    Msg m;
    bool ok = true;
    ok &= AddMasterShot(m, 1.5f, "(CONVERSATION_CAMERA_LEFT_SHOULDER, CONVERSATION_CAMERA_FOCUS_50, CONVERSATION_CAMERA_VIEW_CLOSE)");
    ok &= m.tags.size() == 1 && m.tags[0].first == 1.5f && m.tags[0].second.position == 1 && m.tags[0].second.focus == 4
          && m.tags[0].second.view == 2 && m.tags[0].second.zoom == 0 && m.tags[0].second.posPerson == 0 && m.dirty;
    ok &= AddMasterShot(m, 2.0f, "(CONVERSATION_CAMERA_LEFT_SHOULDER, CONVERSATION_CAMERA_FOCUS_50, )");
    ok &= m.tags.size() == 2 && m.tags[1].second.view == 1;
    ok &= !AddMasterShot(m, 3.0f, "CONVERSATION_CAMERA_LEFT_SHOULDER, x, y)");
    ok &= !AddMasterShot(m, 3.0f, "(CONVERSATION_CAMERA_LEFT_SHOULDER, CONVERSATION_CAMERA_FOCUS_50)");
    ok &= !AddMasterShot(m, 3.0f, "(UNKNOWN, CONVERSATION_CAMERA_FOCUS_50, )");
    ok &= !AddMasterShot(m, 3.0f, "(CONVERSATION_CAMERA_LEFT_SHOULDER, UNKNOWN, )");
    ok &= !AddMasterShot(m, 3.0f, "(CONVERSATION_CAMERA_LEFT_SHOULDER, CONVERSATION_CAMERA_FOCUS_50, BAD)");
    ok &= m.tags.size() == 2;
    if (!ok) { std::printf("004ce1b0_TEST FAIL\n"); return 1; }
    std::printf("004ce1b0_TEST PASS\n");
    return 0;
}