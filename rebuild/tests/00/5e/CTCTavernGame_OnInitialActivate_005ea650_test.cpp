// Test for 0x005EA650 CTCTavernGame::OnInitialActivate: self-contained behaviour model -- the game is reset (not played, state 0,
// no game master, not in session, base bet 0, prize not given, no draw) and each of the three fonts is fetched by the def's
// font name, the old font's count block released and the new one shared (reference counted as the retail CCountedPointer).
#include <cstdio>
#include <cstring>

struct Block { int refs; int deleted; };
struct FontPtr {
    const char* data; Block* info;
    FontPtr() : data(0), info(0) {}
    FontPtr(const FontPtr& o) : data(o.data), info(o.info) { if (info) ++info->refs; }
    ~FontPtr() { Release(); }
    void Release() { if (info && --info->refs == 0) ++info->deleted; data = 0; info = 0; }
    FontPtr& operator=(const FontPtr& o)
    {
        if (info != o.info) { Release(); data = o.data; info = o.info; if (info) ++info->refs; }
        return *this;
    }
};
static Block g_blocks[3];
static int g_requests = 0;
static FontPtr GetFont(const char* name)
{
    ++g_requests;
    FontPtr p; p.data = name; p.info = &g_blocks[g_requests % 3]; ++p.info->refs;
    return p;
}
struct Def { const char* score; const char* target; const char* stats; int refs; };
struct Game {
    bool played, inSession, prizeGiven, draw; int state, baseBet; void* gameMaster;
    FontPtr scoreFont, targetFont, statsFont;
    void OnInitialActivate(Def& def)
    {
        played = false; state = 0; gameMaster = 0; inSession = false;
        ++def.refs;
        baseBet = 0;
        scoreFont = GetFont(def.score);
        targetFont = GetFont(def.target);
        statsFont = GetFont(def.stats);
        prizeGiven = false; draw = false;
        --def.refs;
    }
};

static int fail(int n) { std::printf("005ea650_TEST FAIL %d\n", n); return 1; }

int main()
{
    Def def = { "FONT_SCORE", "FONT_TARGET", "FONT_STATS", 1 };
    int dummy = 0;
    Game g;
    g.played = true; g.inSession = true; g.prizeGiven = true; g.draw = true; g.state = 9; g.baseBet = 50; g.gameMaster = &dummy;
    g.OnInitialActivate(def);
    if (g.played || g.inSession || g.prizeGiven || g.draw || g.state != 0 || g.baseBet != 0 || g.gameMaster) return fail(1);
    if (std::strcmp(g.scoreFont.data, "FONT_SCORE") || std::strcmp(g.targetFont.data, "FONT_TARGET") ||
        std::strcmp(g.statsFont.data, "FONT_STATS")) return fail(2);
    if (def.refs != 1 || g_requests != 3) return fail(3);
    if (g_blocks[1].refs != 1 || g_blocks[2].refs != 1 || g_blocks[0].refs != 1) return fail(4);
    g.OnInitialActivate(def);   // re-activation replaces the fonts and releases the old blocks
    if (g_requests != 6 || g_blocks[1].refs != 1 || g_blocks[0].deleted != 0) return fail(5);
    std::printf("005ea650_TEST PASS\n");
    return 0;
}