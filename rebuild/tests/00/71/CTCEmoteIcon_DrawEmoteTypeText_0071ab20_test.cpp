// Behaviour test for 0071ab20: src/0071ab20.cpp copied verbatim; the camera, the thing, its appearance, the game
// subsystem and the renderer stubbed so the 24-unit range test, the focal-point gate and the text primitive
// (uid, raised focal position, 1.4x scale, emote text, font, white, 3D scaling) are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCEmoteIcon::DrawEmoteTypeText(const CEnginePrimitivePrepareInfo& info) -- within 24 units of the camera, draw the emote text in white half a unit above the thing's static focal point, scaled 1.4x the emote scale
// Retry 10b (combined getter sweep, FIXES B-15): the graphic appearance and the emote font are read through inline getters (`ParentThing->GetGraphicAppearance()`, `GetGameSubsystem()->GetEmoteFont()`); plain member reads give the parked register difference.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071AB20, 244 bytes (+ int3 padding), `ret 4`.
// Callees: CTCBase::GetCamera @ 0x00686A80 (`[g_pMainGameComponent]+0x24`, the world's current camera; manifest
//   "CViewBase::GetDisplayEngine" is propagated), CThing::GetPosition @ 0x004C73D0, the camera's virtual slot 8 (+0x20,
//   position by value through a hidden pointer), CTCGraphicAppearance::GetStaticFocalPosIfLoaded(C3DVector*) @
//   0x004C11F0, CTCBase::GetGameSubsystem @ 0x00686D60 (its +0x16144 font), CEnginePrimitiveText(unsigned __int64,
//   const C3DVector&, float, const CWideString&, const CFontBank&, const CRGBColour&, bool, EEngineInterpolationSet) @
//   0x004C3120, the prepare info's primitive renderer (+0x34) virtual slot 20 (+0x50, add the primitive),
//   ~CWideString @ 0x0099B510 (the text primitive's only non-trivial member).
// Globals: 576.0f @ 0x01265AB0 (24 squared), 1.4f @ 0x01265AAC, the emote text height 0.5f @ 0x01381044 (an extern
//   float variable, named descriptively).
// Layout = Ego_r.pdb CTCEmoteIcon (no retail shift): EmoteText +0x10, EmoteScale +0x18; CThing 64-bit UID +0x18, the
//   appearance TC +0x64.
// Shape: camera and position are named locals fetched in that order (the landed CThingAICreature::
//   WorldUpdateBulletTime 0x0082FD90); `(camPos - pos).GetLengthSquared() < 576` as one expression (the Z*Z term first);
//   the focal point reuses the camera temporary's frame words; the font is a named local fetched first; the text
//   primitive is a named local built from the white CRGBColour temporary and handed to the renderer.
extern float g_EmoteTextHeight;                 // 0x01381044 (0.5f)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLengthSquared() const { float s = Z * Z; s += Y * Y; s += X * X; return s; }
};
class CRGBColour { public:                             // D3DCOLOR byte order
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); }
};
class CWideString { public:
    void* PStringData;
    ~CWideString();                             // 0x0099B510
};
class CFontBank;
class CEnginePrimitiveText { public:                   // 0x2c bytes
    unsigned char _base[0x20];
    CWideString   Text;                         // +0x20
    CFontBank*    PFont;                        // +0x24
    bool          ScaleIn3D;                    // +0x28
    CEnginePrimitiveText(unsigned __int64 handle, const C3DVector& pos, float scale, const CWideString& text,
                         const CFontBank& font, const CRGBColour& colour, bool scaleIn3D, long interpolationSet);   // 0x004C3120
};
class CEnginePrimitiveRenderer { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void AddPrimitive(const CEnginePrimitiveText& primitive);   // slot 20 (+0x50)
};
class CEnginePrimitivePrepareInfo { public:
    unsigned char _pad00[0x34];
    CEnginePrimitiveRenderer* PRenderer;        // +0x34
};
class CCamera { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual C3DVector GetPosition() const;      // slot 8 (+0x20)
};
struct CTCGraphicAppearance {
    bool GetStaticFocalPosIfLoaded(C3DVector* pos);   // 0x004C11F0
};
class CThing { public:
    unsigned char _pad00[0x18];
    unsigned __int64 UID;                       // +0x18
    unsigned char _pad20[0x64 - 0x20];
    CTCGraphicAppearance* PGraphicAppearance;   // +0x64
    CTCGraphicAppearance* GetGraphicAppearance() const { return PGraphicAppearance; }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
class CMainGameComponent { public:
    unsigned char _pad00[0x16144];
    CFontBank* EmoteFont;                       // +0x16144
    CFontBank* GetEmoteFont() const { return EmoteFont; }
};
class CTCEmoteIcon { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    long        ActiveTimer;                    // +0x0c
    CWideString EmoteText;                      // +0x10
    long        EmoteIcon;                      // +0x14
    float       EmoteScale;                     // +0x18
    const CCamera* GetCamera() const;           // 0x00686A80 (CTCBase)
    CMainGameComponent* GetGameSubsystem();     // 0x00686D60 (CTCBase)
    void DrawEmoteTypeText(const CEnginePrimitivePrepareInfo& info);
};

void CTCEmoteIcon::DrawEmoteTypeText(const CEnginePrimitivePrepareInfo& info)
{
    const CCamera* camera = GetCamera();
    const C3DVector& pos = ParentThing->GetPosition();
    if ((camera->GetPosition() - pos).GetLengthSquared() < 576.0f) {
        C3DVector focal;
        if (ParentThing->GetGraphicAppearance()->GetStaticFocalPosIfLoaded(&focal)) {
            focal.Z += g_EmoteTextHeight;
            CFontBank* font = GetGameSubsystem()->GetEmoteFont();
            CEnginePrimitiveText text(ParentThing->UID, focal, EmoteScale * 1.4f, EmoteText, *font,
                                      CRGBColour(255, 255, 255, 255), true, 0);
            info.PRenderer->AddPrimitive(text);
        }
    }
}
// ---- end source ----
void CEnginePrimitiveRenderer::v00() {} void CEnginePrimitiveRenderer::v01() {} void CEnginePrimitiveRenderer::v02() {} void CEnginePrimitiveRenderer::v03() {} void CEnginePrimitiveRenderer::v04() {} void CEnginePrimitiveRenderer::v05() {} void CEnginePrimitiveRenderer::v06() {} void CEnginePrimitiveRenderer::v07() {} void CEnginePrimitiveRenderer::v08() {} void CEnginePrimitiveRenderer::v09() {} void CEnginePrimitiveRenderer::v10() {} void CEnginePrimitiveRenderer::v11() {} void CEnginePrimitiveRenderer::v12() {} void CEnginePrimitiveRenderer::v13() {} void CEnginePrimitiveRenderer::v14() {} void CEnginePrimitiveRenderer::v15() {} void CEnginePrimitiveRenderer::v16() {} void CEnginePrimitiveRenderer::v17() {} void CEnginePrimitiveRenderer::v18() {} void CEnginePrimitiveRenderer::v19() {}
void CCamera::v00() {} void CCamera::v01() {} void CCamera::v02() {} void CCamera::v03() {} void CCamera::v04() {} void CCamera::v05() {} void CCamera::v06() {} void CCamera::v07() {}
void CTCEmoteIcon::v00() {}
float g_EmoteTextHeight = 0.5f;
CWideString::~CWideString() {}
static unsigned __int64 g_uid = 0; static C3DVector g_textPos; static float g_scale = 0; static void* g_text = 0; static const CFontBank* g_font = 0;
static unsigned char g_r = 0, g_a = 0; static bool g_3d = false; static long g_interp = -1;
CEnginePrimitiveText::CEnginePrimitiveText(unsigned __int64 handle, const C3DVector& pos, float scale, const CWideString& text,
                                           const CFontBank& font, const CRGBColour& colour, bool scaleIn3D, long interpolationSet)
{ g_uid = handle; g_textPos = pos; g_scale = scale; g_text = text.PStringData; g_font = &font; g_r = colour.R; g_a = colour.A; g_3d = scaleIn3D; g_interp = interpolationSet; Text.PStringData = 0; }
static int g_adds = 0;
void CEnginePrimitiveRenderer::AddPrimitive(const CEnginePrimitiveText&) { ++g_adds; }
static C3DVector g_camPos(0, 0, 0), g_thingPos(10, 10, 0), g_focal(1, 2, 3); static bool g_loaded = true;
C3DVector CCamera::GetPosition() const { return g_camPos; }
bool CTCGraphicAppearance::GetStaticFocalPosIfLoaded(C3DVector* pos) { if (g_loaded) *pos = g_focal; return g_loaded; }
const C3DVector& CThing::GetPosition() const { return g_thingPos; }
static CCamera g_camera; const CCamera* CTCEmoteIcon::GetCamera() const { return &g_camera; }
static unsigned char g_mgcRaw[sizeof(CMainGameComponent)]; CMainGameComponent* CTCEmoteIcon::GetGameSubsystem() { return (CMainGameComponent*)g_mgcRaw; }
int main()
{
    static int fontObj; ((CMainGameComponent*)g_mgcRaw)->EmoteFont = (CFontBank*)&fontObj;
    static CTCGraphicAppearance app; static unsigned char thingRaw[sizeof(CThing)]; memset(thingRaw, 0, sizeof(thingRaw));
    CThing* thing = (CThing*)thingRaw; thing->PGraphicAppearance = &app; thing->UID = 0x1122334455ULL;
    static CEnginePrimitiveRenderer renderer; CEnginePrimitivePrepareInfo info; memset(&info, 0, sizeof(info)); info.PRenderer = &renderer;
    static unsigned char iconRaw[sizeof(CTCEmoteIcon)]; memset(iconRaw, 0, sizeof(iconRaw));
    CTCEmoteIcon* icon = (CTCEmoteIcon*)iconRaw; icon->ParentThing = thing; icon->EmoteScale = 2.0f; static char hi[] = "hi"; icon->EmoteText.PStringData = hi;
    icon->DrawEmoteTypeText(info);                                   // 200 < 576
    CHECK(g_adds == 1 && g_uid == 0x1122334455ULL && g_textPos.X == 1 && g_textPos.Y == 2 && g_textPos.Z == 3.5f);
    CHECK(g_scale > 2.79f && g_scale < 2.81f && g_text == (void*)hi && g_font == (const CFontBank*)&fontObj && g_r == 255 && g_a == 255 && g_3d && g_interp == 0);
    g_loaded = false; icon->DrawEmoteTypeText(info); CHECK(g_adds == 1);
    g_loaded = true; g_thingPos = C3DVector(20, 20, 0); icon->DrawEmoteTypeText(info); CHECK(g_adds == 1);   // 800 >= 576
    if (fails == 0) printf("0071ab20_TEST PASS\n"); else printf("0071ab20_TEST FAIL\n");
    return fails;
}