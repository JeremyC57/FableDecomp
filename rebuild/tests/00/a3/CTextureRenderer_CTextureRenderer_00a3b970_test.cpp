// Behaviour test for 0x00A3B970 ~CTextureRenderer: delete the texture only when it is set and owned (its dtor runs Uninitialise),
// then ~CBaseVideoRenderer.
#include <cstdio>
#include <cstring>
static char g_log[32];
static void Log(const char* s) { std::strcat(g_log, s); }
static int g_uninitialised = 0;
class CTexture { public: ~CTexture() { Uninitialise(); } void Uninitialise() { ++g_uninitialised; Log("U"); } void* PD3DTexture; unsigned long Bits; };
class CBaseVideoRenderer { public: virtual ~CBaseVideoRenderer() { Log("V"); } };
class CTextureRenderer : public CBaseVideoRenderer {
public:
    CTextureRenderer(CTexture* texture, bool own) : Texture(texture), CreateOwnTexture(own) { }
    ~CTextureRenderer() { if (Texture && CreateOwnTexture) delete Texture; }
    void* PVideoSys;
    CTexture* Texture;
    int TextureFormat, Width, Height, Pitch;
    bool CreateOwnTexture;
};
int main()
{
    CTexture external;
    g_log[0] = 0;
    CBaseVideoRenderer* r = new CTextureRenderer(new CTexture, true);
    delete r;
    bool ok = std::strcmp(g_log, "UV") == 0 && g_uninitialised == 1;
    g_log[0] = 0;
    r = new CTextureRenderer(&external, false);
    delete r;
    ok = ok && std::strcmp(g_log, "V") == 0;
    g_log[0] = 0;
    r = new CTextureRenderer(0, true);
    delete r;
    ok = ok && std::strcmp(g_log, "V") == 0 && g_uninitialised == 1;
    std::printf(ok ? "00a3b970_TEST PASS\n" : "00a3b970_TEST FAIL\n");
    return ok ? 0 : 1;
}