// Grade: DIFFER(509v509) -- frame slot + x87 scheduling: retail returns the first GetRenderTargetDimensions result into [ebp-0xc] (the slot the clock position then takes) where ours uses [ebp-0x14]; in the icon block retail keeps the centred X on the x87 stack across the uv zero stores and writes it to [ebp-0x1c] last, ours stores it straight away.
// NPlayerGui::CCrimeStatBar::DrawStatBox(const CEnginePrimitivePrepareInfo& info, CGraphicDataBank* bank, const C2DVector& pos, long
// iconGraphic, long clockGraphic, long c, const CWideString& text, unsigned char alpha, bool d, bool e) -- draw the crime clock (a
// horizontally centred clock sprite, black/white with the given alpha, layer 0x45) and the centred crime icon (layer 0x44, alpha scaled by
// the player GUI fade) above the box, then the plain CStatBar box with no graphics.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064619F, length 509 bytes, `ret 0x28`.
// Callees: CDisplayManager::GetRenderTargetDimensions 0x009BEDC0 on 0x013B8390; bank slot 23 (+0x5C, graphic width, unsigned);
// CEnginePrimitive2DClockSprite 0x0065595C; CEnginePrimitive2DSprite 0x00493AC2; renderer slot 23 (+0x5C); _ftol 0x00BFEA70;
// CStatBar::DrawStatBox 0x00645CE9 (direct base call). g_pPlayerGui 0x013B8790 (+0x2a0 fade). 0.5f 0x0122F59C, 2^32 0x0122DCB4.
// Layout (descriptive): def +4 (box Y +0x438, clock Y offset +0x440), handles +0x30 (clock [4], icon [3]), clock scale +0x6c.
// Shape: y = def Y + pos.Y named; clock position named (ctor args evaluated right to left: Y to a temp first); clock scale read once into a
// named local (fld; fst; fstp); sprite modelled as 0x58 bytes: retail keeps a 4-byte slot at [ebp-0x24] between the sprite and y (the same sprite is 0x54 in the matched 0x00645292).
// Tried: the scale named (512 -> 509), an inline centred-X helper (512), the icon position as a sprite-argument temporary (frame +4), a
// named float X (worse).
// Leaf: DrawStatBox
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CEnginePrimitiveHandle { unsigned long Data[2]; };
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C2DExtentsI { long Width, Height; };
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CWideString { void* m_pData; };
struct CDisplayManager { C2DExtentsI GetRenderTargetDimensions() const; };   // 0x009BEDC0
extern CDisplayManager* g_pDisplayManager;            // 0x013B8390
struct CPlayerGui { unsigned char _pad[0x2a0]; float FadeAlpha; };   // +0x2a0
extern CPlayerGui* g_pPlayerGui;                      // 0x013B8790
struct CGraphicDataBank {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual unsigned long GetGraphicWidth(long graphic);   // slot 23 (+0x5C)
};
class CEnginePrimitive2DSprite {
public:
    unsigned char _body[0x58];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DVector& pos, long layer, unsigned long graphic, unsigned long b, long blend,
                             long interpolation, float scaleX, float scaleY, unsigned char alpha, unsigned char red, unsigned char green,
                             unsigned char blue, unsigned long e, const C2DVector& uv0, const C2DVector& uv1);   // 0x00493AC2
};
class CEnginePrimitive2DClockSprite {
public:
    unsigned char _body[0x64];
    CEnginePrimitive2DClockSprite(unsigned __int64 id, const C2DVector& pos, long layer, unsigned long graphic, unsigned long a,
                                  const CRGBColour& colour, unsigned long graphic2, unsigned long b, const CRGBColour& colour2, long blend,
                                  float scaleX, float scaleY, const C2DVector& uv0, const C2DVector& uv1, unsigned long c,
                                  long interpolation);   // 0x0065595C
};
class CEnginePrimitiveRenderer {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const void* primitive, long flags);   // slot 23 (+0x5C)
};
class CEnginePrimitivePrepareInfo {
public:
    unsigned char _pad00[0x34];
    CEnginePrimitiveRenderer* Renderer;             // +0x34
};
struct CStatBarDef {
    unsigned char _pad000[0x438];
    float StatBoxY;                                 // +0x438
    unsigned char _pad43c[4];
    float ClockOffsetY;                             // +0x440
};
struct CStatBar {
    virtual void v00();
    CStatBarDef* PDef;                              // +0x04
    unsigned char _pad08[0x30 - 8];
    CEnginePrimitiveHandle* Handles;                // +0x30 vector start
    unsigned char _pad34[0x6c - 0x34];
    float ClockScale;                               // +0x6c
    void DrawStatBox(const CEnginePrimitivePrepareInfo& info, CGraphicDataBank* bank, const C2DVector& pos, long iconGraphic,
                     long clockGraphic, long c, const CWideString& text, unsigned char alpha, bool d, bool e);   // 0x00645CE9
};
struct CCrimeStatBar : CStatBar {
    void DrawStatBox(const CEnginePrimitivePrepareInfo& info, CGraphicDataBank* bank, const C2DVector& pos, long iconGraphic,
                     long clockGraphic, long c, const CWideString& text, unsigned char alpha, bool d, bool e);
};

void CCrimeStatBar::DrawStatBox(const CEnginePrimitivePrepareInfo& info, CGraphicDataBank* bank, const C2DVector& pos, long iconGraphic,
                                long clockGraphic, long c, const CWideString& text, unsigned char alpha, bool d, bool e)
{
    float y = PDef->StatBoxY + pos.Y;
    if (clockGraphic > 0) {
        C2DVector clockPos((float)(g_pDisplayManager->GetRenderTargetDimensions().Width - bank->GetGraphicWidth(clockGraphic)) * 0.5f,
                           y + PDef->ClockOffsetY);
        float scale = ClockScale;
        CEnginePrimitive2DClockSprite clock(0, clockPos, 0x45, clockGraphic, 0, CRGBColour(0, 0, 0, alpha), clockGraphic, 0,
                                            CRGBColour(0xff, 0xff, 0xff, alpha), 2, scale, scale, C2DVector(0.0f, 0.0f),
                                            C2DVector(0.0f, 0.0f), 2, 0);
        info.Renderer->AddPrimitive(&Handles[4], &clock, 0xc0);
    }
    if (iconGraphic > 0) {
        C2DVector iconPos((float)(g_pDisplayManager->GetRenderTargetDimensions().Width - bank->GetGraphicWidth(iconGraphic)) * 0.5f, y);
        CEnginePrimitive2DSprite icon(0, iconPos, 0x44, iconGraphic, 0, 2, 0, 0.0f, 0.0f, (unsigned char)(alpha * g_pPlayerGui->FadeAlpha),
                                      0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Renderer->AddPrimitive(&Handles[3], &icon, 0xc0);
    }
    CStatBar::DrawStatBox(info, bank, pos, 0, 0, 0, text, alpha, d, e);
}
