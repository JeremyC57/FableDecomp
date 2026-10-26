#include <cstdio>
class CCharString { public:};
static int g_called=0; static bool g_a=false;
struct Inner { void SetMode(bool x, const CCharString&){ g_called=1; g_a=x; } };
static char g_blk[0x40]; static Inner* g_cutscene=(Inner*)g_blk;
class CSubtitleRenderer { public: void SetCutsceneActionMode(bool, const CCharString&) const; };
void CSubtitleRenderer::SetCutsceneActionMode(bool a, const CCharString& name) const {
    ((Inner*)((char*)g_cutscene + 0x18))->SetMode(a, name);
}
int main(){ CSubtitleRenderer r; CCharString s; r.SetCutsceneActionMode(true, s);
 if(!g_called||!g_a){ std::printf("008922c0_TEST FAIL\n"); return 1;}
 std::printf("008922c0_TEST PASS\n"); return 0;}