// Behaviour model of CPlayerGui::~CPlayerGui (0x0043A570): the static PGuiDef is released (count decremented, pointee deleted at zero)
// and nulled, then the members are destroyed in reverse declaration order, the base singleton last.
#include <cstdio>
#include <string>
static std::string g_log;
struct DefPointee { long Count; bool Deleted; };
struct DefPointer {
    DefPointee* Object;
    void Release() { if (Object && --Object->Count == 0) Object->Deleted = true; }
};
static DefPointee g_def = { 1, false };
static DefPointer g_PGuiDef = { &g_def };
struct Member { char Tag; explicit Member(char t) : Tag(t) { } ~Member() { g_log += Tag; } };
struct SingletonBase { ~SingletonBase() { g_log += 'S'; } };
struct PlayerGui : SingletonBase {
    Member HeroScreens, ScreenMessages, ScriptTexts, Handles, Subtitles;
    PlayerGui() : HeroScreens('H'), ScreenMessages('M'), ScriptTexts('T'), Handles('P'), Subtitles('U') { }
    ~PlayerGui() { g_PGuiDef.Release(); g_PGuiDef.Object = 0; g_log += 'R'; }
};
int main()
{
    { PlayerGui gui; }
    if (g_log != "RUPTMHS") { std::printf("FAIL order %s\n", g_log.c_str()); return 1; }
    if (!g_def.Deleted || g_def.Count != 0 || g_PGuiDef.Object != 0) { std::printf("FAIL release\n"); return 1; }
    g_def.Count = 2; g_def.Deleted = false; g_PGuiDef.Object = &g_def; g_log = "";
    { PlayerGui gui; }
    if (g_def.Deleted || g_def.Count != 1 || g_PGuiDef.Object != 0) { std::printf("FAIL shared\n"); return 1; }
    std::printf("0043a570_TEST PASS\n");
    return 0;
}