// Behaviour test for 005ec7c0: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTavernGame::RenderText(const CEnginePrimitivePrepareInfo&, float x, float y, CWideString text, const CFontBank& font, CRGBColour colour) -- queues one 2D text primitive at a screen-fraction position: preloads the glyphs, adds a primitive handle and hands the engine the text primitive on layer 0x45
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#define __int64 long long
#endif
#include <stddef.h>

class CWideString { public:
    void* m_data;
    CWideString(const CWideString& other);      // 0x0099B720
    ~CWideString();                             // 0x0099B510
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class C2DExtentsI { public:
    long X, Y;
    C2DExtentsI() {}
};
class CDisplayManager { public:
    const C2DExtentsI GetRenderTargetDimensions();   // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayDevice;      // 0x013B8390

class CFontBank { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual bool Preload(const CWideString& text, bool wait) const;   // slot 4
};

class CEnginePrimitiveThemeChannelDesc { public:
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) {}
};
class CEnginePrimitive { public:
    long Type;                                  // +0x00 EEnginePrimitiveType
    long InterpolationSet;                      // +0x04 EEngineInterpolationSet
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;   // +0x08
    CEnginePrimitive(long type, long interpolationSet) : Type(type), InterpolationSet(interpolationSet) {}
};
class CEnginePrimitive2DText : public CEnginePrimitive { public:
    C3DVector    Pos;                           // +0x0c
    long         Layer;                         // +0x18 EEngine2DLayer
    float        Scale;                         // +0x1c
    CRGBColour   Colour;                        // +0x20
    CWideString  Text;                          // +0x24
    const CFontBank* PFont;                     // +0x28
    CEnginePrimitive2DText(unsigned __int64, const C2DVector& pos, long layer, float scale, const CWideString& text,
                           const CFontBank& font, const CRGBColour& colour, long interpolationSet)
        : CEnginePrimitive(0x27, interpolationSet), Pos(pos.X, pos.Y, 0.0f), Layer(layer), Scale(scale), Colour(colour),
          Text(text), PFont(&font)
    {
    }
};

class CEnginePrimitiveHandle { public:                 // 8 bytes
    void* _vtable;
    void* Data;
};
struct CHandleVector {                          // STLport vector<CEnginePrimitiveHandle> (12 bytes)
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
    CEnginePrimitiveHandle* end() { return _M_finish; }
    CEnginePrimitiveHandle& back() { return *(end() - 1); }
    void AddDefaultHandle_005ed940();           // 0x005ED940
};

class CIEngine { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DText* primitive, unsigned long flags);   // slot 23
};
class CEnginePrimitivePrepareInfo { public:
    unsigned char _pad[0x34];                   // CInterpolationInfo Interpolate
    CIEngine*     Engine;                       // +0x34
};

class CTCTavernGame { public:
    void*         __vfptr;                      // +0x00 (CTCBase)
    void*         ParentThing;                  // +0x04
    unsigned char _pad08[0x44 - 0x08];
    CHandleVector Handles;                      // +0x44
    void RenderText(const CEnginePrimitivePrepareInfo& info, float x, float y, CWideString text, const CFontBank& font,
                    CRGBColour colour);
};

void CTCTavernGame::RenderText(const CEnginePrimitivePrepareInfo& info, float x, float y, CWideString text,
                               const CFontBank& font, CRGBColour colour)
{
    font.Preload(text, false);
    Handles.AddDefaultHandle_005ed940();
    C2DExtentsI size = g_pDisplayDevice->GetRenderTargetDimensions();
    CEnginePrimitive2DText primitive(0, C2DVector(size.X * x, size.Y * y), 0x45, 1.0f, text, font, colour, 0);
    info.Engine->AddPrimitive(&Handles.back(), &primitive, 0xc0);
}

// ---- stubs and driver ----
static int g_wcopies, g_wdtors;
CWideString::CWideString(const CWideString& other) { m_data = other.m_data; ++g_wcopies; }
CWideString::~CWideString() { ++g_wdtors; }
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions() { C2DExtentsI e; e.X = 640; e.Y = 480; return e; }
static CDisplayManager g_display;
CDisplayManager* g_pDisplayDevice = &g_display;
void CFontBank::v00() {} void CFontBank::v01() {} void CFontBank::v02() {} void CFontBank::v03() {}
static const void* g_preloaded; static bool g_wait = true;
bool CFontBank::Preload(const CWideString& text, bool wait) const { g_preloaded = text.m_data; g_wait = wait; return true; }
static CEnginePrimitiveHandle g_handles[4];
void CHandleVector::AddDefaultHandle_005ed940() { _M_finish->_vtable = 0; _M_finish->Data = 0; ++_M_finish; }
#define V(n) void CIEngine::v##n() {}
V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19) V(20) V(21) V(22)
static CEnginePrimitiveHandle* g_gotHandle; static unsigned long g_flags; static int g_adds;
static long g_type, g_interp, g_layer; static float g_x, g_y, g_z, g_scale; static unsigned char g_b, g_g, g_r, g_a, g_cd0, g_cd1, g_cd2;
static const void* g_text; static const CFontBank* g_font;
void CIEngine::AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DText* p, unsigned long flags)
{
    ++g_adds; g_gotHandle = handle; g_flags = flags;
    g_type = p->Type; g_interp = p->InterpolationSet; g_cd0 = p->ThemeChannelDesc.ChannelIndex0; g_cd1 = p->ThemeChannelDesc.ChannelIndex1;
    g_cd2 = p->ThemeChannelDesc.ChannelBlendFactor; g_x = p->Pos.X; g_y = p->Pos.Y; g_z = p->Pos.Z; g_layer = p->Layer; g_scale = p->Scale;
    g_b = p->Colour.B; g_g = p->Colour.G; g_r = p->Colour.R; g_a = p->Colour.A; g_text = p->Text.m_data; g_font = p->PFont;
}

int main()
{
    static CTCTavernGame game;
    game.Handles._M_start = game.Handles._M_finish = g_handles; game.Handles._M_end_of_storage = g_handles + 4;
    static CIEngine engine;
    static CEnginePrimitivePrepareInfo info;
    info.Engine = &engine;
    static CFontBank font;
    static char chars[] = "hello";
    CWideString* text = (CWideString*)new char[sizeof(CWideString)];
    text->m_data = chars;
    CRGBColour* colour = (CRGBColour*)new char[sizeof(CRGBColour)];
    colour->B = 1; colour->G = 2; colour->R = 3; colour->A = 4;
    float x = 0.25f, y = 0.5f;
    game.RenderText(info, x, y, *text, font, *colour);
    float ex = 640 * x, ey = 480 * y;
    if (g_adds != 1 || g_gotHandle != &g_handles[0] || game.Handles._M_finish != g_handles + 1 || g_flags != 0xc0
        || g_preloaded != chars || g_wait != false)
        { std::printf("005ec7c0_TEST FAIL (calls)\n"); return 1; }
    if (g_type != 0x27 || g_interp != 0 || g_cd0 || g_cd1 || g_cd2 || g_x != ex || g_y != ey || g_z != 0.0f || g_layer != 0x45
        || g_scale != 1.0f || g_b != 1 || g_g != 2 || g_r != 3 || g_a != 4 || g_text != chars || g_font != &font)
        { std::printf("005ec7c0_TEST FAIL (primitive)\n"); return 1; }
    std::printf("005ec7c0_TEST PASS\n");
    return 0;
}