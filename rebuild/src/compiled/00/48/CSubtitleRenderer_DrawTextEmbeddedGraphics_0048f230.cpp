// CSubtitleRenderer::DrawTextEmbeddedGraphics(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& box) -- for each embedded controller graphic already reached by the text reveal, add its sprite (layer 0x45) at the box corner plus the graphic's and the GUI def's offsets, and draw the graphic's caption (if any) beside it in white at 0.75 scale on layer 0x43
// Leaf: DrawTextEmbeddedGraphics
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048F230, 474 bytes, `ret 8` (retail mangling
//   ?DrawTextEmbeddedGraphics@CSubtitleRenderer@@AAEXABVCEnginePrimitivePrepareInfo@@ABVC2DBoxF@@@Z).
// Callees: PeekGuiDef 0x00438570 (__fastcall), GFVYToAY 0x009E1E40 / GFVXToAX 0x009E1E10 (__stdcall, virtual to render-target pixels: x * width / 640; some landed files call these GFAXToVX/GFAYToVY), CEnginePrimitive2DSprite
//   ctor 0x00493AC2 (13 arguments), the renderer's AddPrimitive (slot 23, +0x5C), CSubtitleRenderer::GetTextFromEmbeddedTagGraphic
//   0x0048DF77 and GetTextOffsetFromEmbeddedTagGraphic 0x0048DC3C (both landed), CWideString::operator!=(const wchar_t*)
//   0x0099B150 (L"" 0x0122D70C) / ~CWideString 0x0099B510, GFACoordToVCoord 0x009E21D0 (__fastcall), CGameUI::Draw2DText
//   0x00438010 (g_pGameUI 0x013B8790); 0.75f 0x01238174.
// Layout: the retail CSubtitleRenderer view of the landed neighbours (TextCharactersShowing +0x24, PFont +0x80,
//   EmbeddedPrimitiveHandles +0xAC, EmbeddedGraphics +0xB8 of 16-byte CEmbeddedGraphic, AllCharactersDisplayed +0xDD);
//   CPlayerGuiDef ControllerGraphicOffset +0x694 (PDB +0x6D0, retail 0x3C lower).
// Shape: the size TU of its neighbours (`y` off + `s` on: frame pointer, leave); the sprite position is
//   `Offset + def offset + C2DVector(GFVXToAX, GFVYToAY)` (the right operand's calls run first, Y before X); the caption
//   position is a named sum passed by address.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                          // 0x0099B510
    bool operator!=(const unsigned short* text) const;             // 0x0099B150
};

struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
};
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CFontBank;

float __stdcall GFVXToAX_009e1e10(float x);                           // 0x009E1E10
float __stdcall GFVYToAY(float y);                           // 0x009E1E40
void __fastcall GFACoordToVCoord(const C2DVector* in, C2DVector* out);   // 0x009E21D0

struct CEnginePrimitiveHandle { unsigned long Data[2]; };
class CEnginePrimitive2DSprite {
public:
    unsigned char _body[0x54];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DVector& pos, long layer, unsigned long graphic, unsigned long b, long blend,
                             long interpolation, float scaleX, float scaleY, unsigned char alpha, unsigned char red, unsigned char green,
                             unsigned char blue, unsigned long e, const C2DVector& uv0, const C2DVector& uv1);   // 0x00493AC2
};
class CEnginePrimitiveRenderer {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DSprite& primitive, long flags);   // slot 23 (+0x5C)
};
class CEnginePrimitivePrepareInfo {
public:
    unsigned char _pad00[0x34];
    CEnginePrimitiveRenderer* Renderer;                      // +0x34
};
struct CPlayerGuiDef {
    unsigned char _pad000[0x694];
    C2DVector     ControllerGraphicOffset;                   // +0x694 (PDB +0x6D0)
};
const CPlayerGuiDef* __fastcall PeekGuiDef();                // 0x00438570

enum EEngine2DLayer { ENGINE_2D_LAYER_SUBTITLE = 0x43 };
struct CGameUI {
    void Draw2DText(const C2DVector& pos, const CWideString& text, CFontBank* font, const CRGBColour& colour,
                    float scale, EEngine2DLayer layer, bool flag);   // 0x00438010
};
extern CGameUI* g_pGameUI;                                   // 0x013B8790

struct CEmbeddedGraphic {                                    // CSubtitleRenderer::CEmbeddedGraphic
    long      Graphic;                                       // +0x00
    long      CharIndex;                                     // +0x04
    C2DVector Offset;                                        // +0x08
};
template <class T> struct CRetailVector {                    // VC6 vector: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    bool empty() const { return m_pFirst == m_pLast; }
    unsigned long size() const { return m_pLast - m_pFirst; }
    T& operator[](unsigned long i) { return m_pFirst[i]; }
};

class CSubtitleRenderer {
public:
    void*              __vftable;                            // +0x00
    C2DBoxF            VWindow;                              // +0x04
    C2DBoxF            VTelevisionWindow;                    // +0x14
    long               TextCharactersShowing;                // +0x24
    unsigned char      _pad28[0x80 - 0x28];
    CFontBank*         PFont;                                // +0x80
    unsigned char      _pad84[0xac - 0x84];
    CRetailVector<CEnginePrimitiveHandle> EmbeddedPrimitiveHandles;   // +0xAC
    CRetailVector<CEmbeddedGraphic>       EmbeddedGraphics;           // +0xB8
    unsigned char      _padc4[0xdd - 0xc4];
    bool               AllCharactersDisplayed;               // +0xDD
    CWideString GetTextFromEmbeddedTagGraphic(long graphic) const;            // 0x0048DF77
    const C2DVector& GetTextOffsetFromEmbeddedTagGraphic(long graphic) const;  // 0x0048DC3C
    void DrawTextEmbeddedGraphics(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& box);
};

void CSubtitleRenderer::DrawTextEmbeddedGraphics(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& box)
{
    if (EmbeddedGraphics.empty())
        return;
    const CPlayerGuiDef* def = PeekGuiDef();
    for (unsigned long i = 0; i < EmbeddedGraphics.size(); ++i) {
        if (AllCharactersDisplayed || TextCharactersShowing > EmbeddedGraphics[i].CharIndex) {
            C2DVector pos = EmbeddedGraphics[i].Offset + def->ControllerGraphicOffset + C2DVector(GFVXToAX_009e1e10(box.TLX), GFVYToAY(box.TLY));
            CEnginePrimitive2DSprite sprite(0, pos, 0x45, EmbeddedGraphics[i].Graphic, 0, 2, 0, 0.0f, 0.0f, 0xff, 0xff, 0xff, 0xff, 0,
                                            C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Renderer->AddPrimitive(&EmbeddedPrimitiveHandles[i], sprite, 0xc0);
            CWideString text = GetTextFromEmbeddedTagGraphic(EmbeddedGraphics[i].Graphic);
            if (text != L"") {
                C2DVector textPos;
                C2DVector at = pos + GetTextOffsetFromEmbeddedTagGraphic(EmbeddedGraphics[i].Graphic);
                GFACoordToVCoord(&at, &textPos);
                g_pGameUI->Draw2DText(textPos, text, PFont, CRGBColour(0xff, 0xff, 0xff, 0xff), 0.75f, ENGINE_2D_LAYER_SUBTITLE, true);
            }
        }
    }
}