// NPlayerGui::CDrawExperience::Draw(CEnginePrimitivePrepareInfo*) -- draw the experience-gain readout: the def's icon for the multi-stat bar's current type at the experience position (layer 0x44), the current change as faded-colour text at the text offset, and a backing box sized from the font string width plus padding and the back graphic height, capped by the back-end graphic (layer 0x47)
// Grade: DIFFER(824v843) -- calls and x87 sequence follow retail, but VC calls the CRGBColour copy ctor out of line (a __forceinline copy ctor gives 837v843 with more register drift), the frame is 0x14c v 0x154 and retail builds the faded colour through a byte temp (B to -0xc, then A, R, B, G). Tried: forceinline / implicit copy ctor, y-off+s-on vs s-only frames, colour field-wise construction, end-sprite x order variants
// Leaf: Draw
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
};
struct C2DCoordF {
    float X, Y;
    C2DCoordF(const C2DVector& v) : X(v.X), Y(v.Y) {}
    C2DCoordF(float x, float y) : X(x), Y(y) {}
};
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF(const C2DVector& tl, const C2DVector& br) : TLX(tl.X), TLY(tl.Y), BRX(br.X), BRY(br.Y) {}
};
void __fastcall GFACoordToVCoord(const C2DVector* in, C2DVector* out);  // 0x009E21D0
class CWideString {
public:
    void* m_pStringData;
    ~CWideString();                                 // 0x0099B510
};
CWideString __fastcall IntToWideString(long value);  // 0x00405F00
class CFontBank {
public:
    float GetStringWidth(const CWideString& text);  // 0x0048DA62
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& o) : B(o.B), G(o.G), R(o.R), A(o.A) {}
};
enum EEngine2DLayer { ENGINE_2D_LAYER_45 = 0x45 };
struct CEnginePrimitiveHandle { unsigned long Data[2]; };
class CEnginePrimitive2DSprite {
public:
    unsigned char _body[0x54];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DCoordF& pos, long layer, unsigned long graphic, unsigned long b, long blend,
                             long interpolation, float scaleX, float scaleY, unsigned char alpha, unsigned char red, unsigned char green,
                             unsigned char blue, unsigned long e, const C2DVector& uv0, const C2DVector& uv1);   // 0x00493AC2
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DBoxF& box, long layer, unsigned long graphic, unsigned long b, long blend,
                             long interpolation, float scaleX, float scaleY, unsigned char alpha, unsigned char red, unsigned char green,
                             unsigned char blue, unsigned long e, const C2DVector& uv0, const C2DVector& uv1);   // 0x0041BEB0
};
class CEnginePrimitiveRenderer {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DSprite& primitive, long flags);   // slot 23
};
class CEnginePrimitivePrepareInfo {
public:
    unsigned char             _pad00[0x34];
    CEnginePrimitiveRenderer* Renderer;             // +0x34
};
class CGraphicDataBank {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual long GetGraphicHeight(long graphic, long frame);   // slot 22 (+0x58)
};
struct CPlayerGuiDef {
    unsigned char _pad000[0x3d4];
    C2DVector     ExperiencePos;                    // +0x3d4
    long          ExperienceGraphic;                // +0x3dc
    long          ExperienceGraphic3;               // +0x3e0
    long          ExperienceGraphic1;               // +0x3e4
    long          ExperienceGraphic0;               // +0x3e8
    long          ExperienceGraphic2;               // +0x3ec
    C2DVector     ExperienceTextOffset;             // +0x3f0
    CRGBColour    ExperienceTextColour;             // +0x3f8
    C2DVector     ExperienceBackOffset;             // +0x3fc
    float         ExperienceBackPadding;            // +0x404
    long          ExperienceBackGraphic;            // +0x408
    long          ExperienceBackEndGraphic;         // +0x40c
};
extern CPlayerGuiDef* __fastcall PeekGuiDef();      // 0x00438570
struct CMultiStatBar {
    unsigned char _pad00[0x2c];
    float         Alpha;                            // +0x2c
    long GetCurrentType();                          // 0x00646D90
    long GetCurrentChange();                        // 0x00646DA7
};
struct CGameUI {
    unsigned char  _pad000[0x2a0];
    float          GlobalAlpha;                     // +0x2a0
    unsigned char  _pad2a4[0x2f4 - 0x2a4];
    CMultiStatBar* ExperienceBar;                   // +0x2f4
    void Draw2DText(const C2DVector& pos, const CWideString& text, CFontBank* font, const CRGBColour& colour,
                    float scale, EEngine2DLayer layer, bool flag);   // 0x00438010
};
extern CGameUI* g_pGameUI;                          // 0x013B8790
struct CGuiResources {
    unsigned char _pad[0x1614c];
    CFontBank*    DefaultFont;                      // +0x1614c
};
struct CHandleVector { CEnginePrimitiveHandle *Start, *Finish, *End; };

namespace NPlayerGui {
struct CDrawExperience {
    virtual void v00();
    CGameUI*          PlayerGui;                    // +0x04
    void*             PlayerManager;                // +0x08
    void*             World;                        // +0x0c
    CGuiResources*    Resources;                    // +0x10
    CGraphicDataBank* Bank;                         // +0x14
    bool              Drawing;                      // +0x18
    CHandleVector     Handles;                      // +0x1c
    unsigned char GetFadedAlpha(unsigned char alpha);   // 0x00643D5D
    void Draw(CEnginePrimitivePrepareInfo* info);
};

void CDrawExperience::Draw(CEnginePrimitivePrepareInfo* info)
{
    long type = PlayerGui->ExperienceBar->GetCurrentType();
    if (type < 0 || type > 3)
        return;
    Drawing = true;
    CPlayerGuiDef* def = PeekGuiDef();
    unsigned char alpha = (unsigned char)(PlayerGui->ExperienceBar->Alpha * 255.0f);
    const C2DVector& origin = def->ExperiencePos;
    C2DVector pos(origin.X, origin.Y);
    long graphic = def->ExperienceGraphic;
    switch (type) {
    case 0: graphic = def->ExperienceGraphic0; break;
    case 1: graphic = def->ExperienceGraphic1; break;
    case 2: graphic = def->ExperienceGraphic2; break;
    case 3: graphic = def->ExperienceGraphic3; break;
    }
    CEnginePrimitive2DSprite icon(0, C2DCoordF(pos.X, pos.Y), 0x44, graphic, 0, 2, 0, 0.0f, 0.0f, GetFadedAlpha(alpha), 0xff, 0xff, 0xff, 0,
                                  C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info->Renderer->AddPrimitive(&Handles.Start[0], icon, 0xc0);
    CFontBank* font = Resources->DefaultFont;
    CWideString text = IntToWideString(PlayerGui->ExperienceBar->GetCurrentChange());
    CRGBColour colour = def->ExperienceTextColour;
    colour.A = (unsigned char)(colour.A * PlayerGui->GlobalAlpha);
    C2DVector textPos = pos + def->ExperienceTextOffset;
    C2DVector screen;
    GFACoordToVCoord(&textPos, &screen);
    g_pGameUI->Draw2DText(screen, text, font, colour, 0.85f, ENGINE_2D_LAYER_45, false);
    float height = (float)(unsigned long)Bank->GetGraphicHeight(def->ExperienceBackGraphic, 0);
    float width = font->GetStringWidth(text) + def->ExperienceBackPadding;
    C2DVector backPos = pos + def->ExperienceBackOffset;
    C2DVector backEnd(backPos.X + width, backPos.Y + height);
    CEnginePrimitive2DSprite back(0, C2DBoxF(backPos, backEnd), 0x47, def->ExperienceBackGraphic, 0, 2, 0, 0.0f, 0.0f, GetFadedAlpha(alpha),
                                  0xff, 0xff, 0xff, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info->Renderer->AddPrimitive(&Handles.Start[1], back, 0xc0);
    CEnginePrimitive2DSprite end(0, C2DCoordF(C2DVector(backEnd.X - backPos.X + backPos.X, backPos.Y)), 0x47, def->ExperienceBackEndGraphic,
                                 0, 2, 0, 0.0f, 0.0f, GetFadedAlpha(alpha), 0xff, 0xff, 0xff, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info->Renderer->AddPrimitive(&Handles.Start[2], end, 0xc0);
}
}
