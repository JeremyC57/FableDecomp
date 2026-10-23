// Behaviour test for CEngineWaterRenderer::RenderIceBackground @ 0x00B75CA0 (restated with fakes).
#include <cstdio>
#include <cstring>

struct Log {
    char text[512];
    void add(const char* s) { std::strcat(text, s); std::strcat(text, ";"); }
};
static Log g_log;

struct Fake {
    float bump[4];
    int bones[4];
    unsigned long states[6];
    unsigned long prepareFlags;
    unsigned long boneCount;
};
static Fake g;

struct Ice {
    bool enableBump;
    float scale;
    int iceTex, envTex;
};

static void RenderIceBackground(const Ice& w)
{
    g_log.add("open");
    g_log.add(w.iceTex ? "tex0" : "tex0null");
    g_log.add(w.envTex ? "tex1" : "tex1null");
    g.states[0] = 0xdc8; g.states[1] = 0xdc8; g.states[2] = 0xdcc; g.states[3] = 0xdd0;
    g_log.add("vs");
    g_log.add("ps");
    float bump = 0.0f;
    if (w.enableBump)
        bump = w.scale * 0.5f;
    for (int i = 0; i < 4; ++i) g.bump[i] = bump;
    g.boneCount = 4;
    for (int i = 0; i < 4; ++i) g.bones[i] = i;
    g.prepareFlags = 0x604;
    g_log.add("alphaed");
    g.states[4] = 0; g.states[5] = 0;
    g_log.add("opaque");
    g_log.add("restore");
}

int main()
{
    int fails = 0;
    std::memset(&g, 0, sizeof g);
    std::memset(&g_log, 0, sizeof g_log);
    Ice w; w.enableBump = true; w.scale = 3.0f; w.iceTex = 1; w.envTex = 0;
    RenderIceBackground(w);
    if (std::strcmp(g_log.text, "open;tex0;tex1null;vs;ps;alphaed;opaque;restore;") != 0) ++fails;
    if (g.bump[0] != 1.5f || g.bump[3] != 1.5f || g.boneCount != 4 || g.prepareFlags != 0x604) ++fails;
    if (g.states[2] != 0xdcc || g.states[3] != 0xdd0) ++fails;
    w.enableBump = false;
    RenderIceBackground(w);
    if (g.bump[1] != 0.0f) ++fails;
    if (fails == 0) std::printf("00b75ca0_TEST PASS\n"); else std::printf("00b75ca0_TEST FAIL %d\n", fails);
    return fails;
}