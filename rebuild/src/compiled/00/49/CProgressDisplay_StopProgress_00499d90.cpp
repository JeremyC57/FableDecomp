// CProgressDisplay::StopProgress(bool showStoppingText, bool clearScreen) -- end the loading/progress display: optionally clear the screen to black (or render a final "Stopping progress display" frame), then release every progress, guild-seal and loading-screen texture and the player GUI's graphics
// Leaf: ?StopProgress@CProgressDisplay@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00499D90, length 530 bytes (padded to 544), `ret 8` (vtable slot 2).
// Callees: GFGetSystemManager 0x009A4EC0; CDisplayManager BeginRender 0x009BEF20 / EndRender 0x009BEF50 / SwapScreens
// 0x009BEEB0; the render manager's Draw2DBox 0x009DE3F0 / Render2DDrawList 0x009DA9F0; CCharString(const char*, long)
// 0x0099EBF0 / ~CCharString 0x0099EAE0; RenderProgress 0x00499880; CTexture::Uninitialise 0x009F9F70;
// vector<CTexture>::erase(first, last) 0x0049B3B0; the player GUI's graphics release 0x00437570 (g_pPlayerGui 0x013B8790);
// globals: progress-enabled bool 0x01375445, suppress bool 0x013B89D3, the two black-screen bools 0x013B89C0 / 0x013B89C1;
// screen size doubles 0x013961E8 / 0x013961F0.
// Layout: Ego_r.pdb CProgressDisplay with 12-byte retail vectors (ProgressTextures +0x04, GuildSealTextures +0x10,
// LoadingScreenTextures +0x1c) and the flags 0x10 lower (DebugDisplay +0x78, Initialised +0x79, BlackScreen +0x7b), as the
// landed RenderProgress 0x00499880.
// Shape: a named C2DBoxF local (a temporary schedules its zero X0/Y0 stores early); the black CRGBColour temporary reuses the dead `clearScreen` argument slot; each texture loop recomputes the size.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class C2DBoxF { public:
    float X0, Y0, X1, Y1;
    C2DBoxF(float x0, float y0, float x1, float y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) {}
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CRenderManager { public:
    bool Draw2DBox(const C2DBoxF& box, const CRGBColour& colour, long flags);   // 0x009DE3F0
    void Render2DDrawList(bool wait);                  // 0x009DA9F0
};
class CDisplayManager { public:
    void*           __vftable;
    void*           SystemManager;                     // +0x04
    CRenderManager* PRenderManager;                    // +0x08
    unsigned char   _pad0c[0x214 - 0x0c];
    bool            Rendering;                         // +0x214
    void BeginRender();                                // 0x009BEF20
    void EndRender();                                  // 0x009BEF50
    void SwapScreens();                                // 0x009BEEB0
};
class CSystemManager { public:
    unsigned char    _pad00[0x60];
    CDisplayManager* DisplayManager;                   // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();      // 0x009A4EC0
extern double g_ProgressScreenWidth;                   // 0x013961E8
extern double g_ProgressScreenHeight;                  // 0x013961F0
extern bool   g_ProgressDisplayEnabled;                // 0x01375445
extern bool   g_ProgressDisplaySuppressed;             // 0x013B89D3
extern bool   g_ProgressBlackScreenA;                  // 0x013B89C1
extern bool   g_ProgressBlackScreenB;                  // 0x013B89C0
class CTexture { public:                                      // 8 bytes
    void* __vftable;
    void* PData;
    void Uninitialise();                               // 0x009F9F70
};
struct TextureVector {                                 // STLport vector<CTexture>, 12 bytes
    CTexture* _M_start;
    CTexture* _M_finish;
    CTexture* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CTexture& operator[](unsigned int i) { return *(_M_start + i); }
    CTexture* erase(CTexture* first, CTexture* last);  // 0x0049B3B0
    void clear() { erase(_M_start, _M_finish); }
};
class CPlayerGui { public:
    void UninitialiseGraphics();                       // 0x00437570
};
extern CPlayerGui* g_pPlayerGui;                       // 0x013B8790

struct CProgressDisplay {
    virtual void v00();
    virtual void v01();
    virtual void StopProgress(bool showStoppingText, bool clearScreen);
    virtual void v03();
    virtual void v04();
    virtual void SetBlackScreenFrame(bool on);         // slot 5 (+0x14)
    TextureVector ProgressTextures;                    // +0x04
    TextureVector GuildSealTextures;                   // +0x10
    TextureVector LoadingScreenTextures;               // +0x1c
    unsigned char _pad28[0x78 - 0x28];
    bool          DebugDisplay;                        // +0x78
    bool          Initialised;                         // +0x79
    bool          FullScreen;                          // +0x7a
    bool          BlackScreen;                         // +0x7b
    void RenderProgress(const CCharString& textTag, float progress, bool showText, bool force);   // 0x00499880
};

void CProgressDisplay::StopProgress(bool showStoppingText, bool clearScreen)
{
    if (!g_ProgressDisplayEnabled || g_ProgressDisplaySuppressed)
        return;
    if (Initialised) {
        if (clearScreen) {
            CDisplayManager* display = GFGetSystemManager()->DisplayManager;
            if (display) {
                CRenderManager* renderer = display->PRenderManager;
                if (renderer) {
                    bool wasRendering = display->Rendering;
                    if (!wasRendering)
                        display->BeginRender();
                    C2DBoxF box(0.0f, 0.0f, (float)g_ProgressScreenWidth, (float)g_ProgressScreenHeight);
                    renderer->Draw2DBox(box, CRGBColour(0, 0, 0, 0xff), 0);
                    renderer->Render2DDrawList(true);
                    display->EndRender();
                    display->SwapScreens();
                    if (wasRendering)
                        display->BeginRender();
                }
            }
        } else if (!DebugDisplay && showStoppingText) {
            bool oldBlackScreen = BlackScreen;
            if (g_ProgressBlackScreenA || g_ProgressBlackScreenB) {
                BlackScreen = true;
                SetBlackScreenFrame(false);
            }
            RenderProgress("Stopping progress display", -1.0f, false, false);
            BlackScreen = oldBlackScreen;
        }
    }
    unsigned int i;
    for (i = 0; i < ProgressTextures.size(); ++i)
        ProgressTextures[i].Uninitialise();
    ProgressTextures.clear();
    for (i = 0; i < GuildSealTextures.size(); ++i)
        GuildSealTextures[i].Uninitialise();
    GuildSealTextures.clear();
    for (i = 0; i < LoadingScreenTextures.size(); ++i)
        LoadingScreenTextures[i].Uninitialise();
    LoadingScreenTextures.clear();
    if (g_pPlayerGui)
        g_pPlayerGui->UninitialiseGraphics();
    Initialised = false;
}