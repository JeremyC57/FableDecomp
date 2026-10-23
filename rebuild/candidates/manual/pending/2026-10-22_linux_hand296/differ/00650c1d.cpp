// Grade: DIFFER(639v635) -- x87 / slot allocation in the icon loop: retail keeps drawPos.X in memory with drawPos.Y
//   promoted to st(0) (slot [ebp-0x24] left empty), the icon height in memory ([ebp-0x10]) and the width in st(0) across
//   the branch merge; ours keeps both scaled sizes in registers and drawPos as a memory pair, and retail frame is
//   0xC0 (an extra 8-byte hole at [ebp-0x6C]) against ours 0xB0. Branch order (small icons fall through), the "5"
//   label block (font read before the CWideString, text destroyed straight after Draw2DText) and the per-iteration
//   uv zero vectors are reproduced.
//   Tried: C2DVector size/operator arithmetic (682, biased frame), scalar w/h (616), flipped branch (615), vector
//   bottom-right sum (630), font local (631), scoped label block (639), cursor-first operator+ (639).
// NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounterList::DrawAt(const C2DVector& pos, CEnginePrimitivePrepareInfo* info) -- refresh the list, then draw its value as tally icons along a row: one enlarged icon (1.2x, nudged back by a tenth) labelled "5" per group of five, then a 0.75x icon per remaining unit; returns the row height (graphic height * scale + 2), or 0 with no graphic
// Leaf: DrawAt
// Retail 0x00650C1D, 635 bytes, size-optimised TU (#pragma optimize("s"): ebp frame, `leave`, unaligned entry), `ret 8`.
// Callees: PeekGuiDef 0x00438570 (__fastcall, result unused), vector<CEnginePrimitiveHandle>::Resize(long) 0x00443F90, the
// quest-info graphic bank (g_pPlayerGui 0x013B8790 +0x1c8 draw-quest-info +0x14; vtable slots 21/22 width/height),
// GFACoordToVCoord 0x009E21D0 (__fastcall), CWideString(const wchar_t*) 0x0099B6B0 (L"5" 0x0125A6B0), CPlayerGui::Draw2DText
// 0x00438010, ~CWideString 0x0099B510, _ftol 0x00BFEA70, CEnginePrimitive2DSprite 0x0041BEB0 (box overload), renderer
// AddPrimitive slot 23.
// Pool: 2^32 0x0122DCB4, 0.75f 0x01238174, 8.0f 0x0125916C, 4.0f 0x0123088C, 1.2f 0x0125A690, 0.1f 0x0122EE58,
// 255.0f 0x01230014, 2.0f 0x0122DCDC, 0.0f 0x0122DEDC.
// Layout: retail CQuestInfoElement is the PDB less 4 (12-byte handle vector): Scale +0x0c, PrimitiveHandles +0x10;
// CQuestInfoElementCounterList Graphic +0x1c, Value +0x20, MaxValue +0x24.
#pragma optimize("s", on)
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    C2DVector& operator-=(const C2DVector& o) { X -= o.X; Y -= o.Y; return *this; }
};
struct C2DSpriteBox {
    float Left, Top, Right, Bottom;
    C2DSpriteBox(const C2DVector& topLeft, const C2DVector& bottomRight)
        : Left(topLeft.X), Top(topLeft.Y), Right(bottomRight.X), Bottom(bottomRight.Y) {}
};
void __fastcall GFACoordToVCoord(const C2DVector* in, C2DVector* out);   // 0x009E21D0
class CWideString {
public:
    void* m_pStringData;
    ~CWideString();                                 // 0x0099B510
    CWideString(const unsigned short* text);        // 0x0099B6B0
};
struct CPlayerGuiDef;
extern CPlayerGuiDef* __fastcall PeekGuiDef();      // 0x00438570
class CFontBank {
public:
    void* __vftable;
    long  Height;                                   // +0x04
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
enum EEngine2DLayer { ENGINE_2D_LAYER_QUEST_INFO_TEXT = 0x49, ENGINE_2D_LAYER_QUEST_INFO = 0x4a };
struct CEnginePrimitiveHandle { unsigned long Data[2]; };
class CEnginePrimitive2DSprite {
public:
    unsigned char _body[0x54];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DSpriteBox& box, unsigned long layer, unsigned long bank,
                             unsigned long frame, unsigned long blend, unsigned long interpolation, float angleStart,
                             float angleEnd, unsigned char alpha, unsigned char red, unsigned char green,
                             unsigned char blue, unsigned long param, const C2DVector& uvStart, const C2DVector& uvEnd);   // 0x0041BEB0
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
    float                     FrameTime;            // +0x00
    unsigned char             _pad04[0x30];
    CEnginePrimitiveRenderer* Renderer;             // +0x34
};
class CGraphicBank {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20();
    virtual unsigned long GetGraphicWidth(long graphic, long frame);    // slot 21
    virtual unsigned long GetGraphicHeight(long graphic, long frame);   // slot 22
};
struct CDrawQuestInfoBase {
    unsigned char _pad[0x14];
    CGraphicBank* Bank;                             // +0x14
};
struct CPlayerGui {
    unsigned char       _pad000[0x1c8];
    CDrawQuestInfoBase* DrawQuestInfo;              // +0x1c8
    unsigned char       _pad1cc[0x2a0 - 0x1cc];
    float               Alpha;                      // +0x2a0
    void Draw2DText(const C2DVector& pos, const CWideString& text, CFontBank* font, const CRGBColour& colour,
                    float scale, EEngine2DLayer layer, bool flag);   // 0x00438010
};
extern CPlayerGui* g_pPlayerGui;                    // 0x013B8790
struct CMainGameComponent {
    unsigned char _pad[0x1614c];
    CFontBank*    DefaultFont;                      // +0x1614c
};
extern CMainGameComponent* g_pMainGameComponent;    // 0x013B86A0

namespace NPlayerGui {
struct CRetailHandleVector {
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    void Resize(long count);                        // 0x00443F90
};
struct CDrawQuestInfo {
    struct CQuestInfoElement {
        virtual ~CQuestInfoElement();
        virtual void v01();
        virtual void Update();                      // slot 2
        long                ID;                     // +0x04
        long                ScriptID;               // +0x08
        float               Scale;                  // +0x0c
        CRetailHandleVector PrimitiveHandles;       // +0x10
    };
    struct CQuestInfoElementCounterList : CQuestInfoElement {
        long Graphic;                               // +0x1c
        long Value;                                 // +0x20
        long MaxValue;                              // +0x24
        float DrawAt(const C2DVector& pos, CEnginePrimitivePrepareInfo* info);
    };
};

float CDrawQuestInfo::CQuestInfoElementCounterList::DrawAt(const C2DVector& pos, CEnginePrimitivePrepareInfo* info)
{
    Update();
    if (Graphic > 0) {
        CPlayerGuiDef* def = PeekGuiDef();
        long fives = Value / 5;
        long icons = fives + Value % 5;
        PrimitiveHandles.Resize(icons);
        CGraphicBank* bank = g_pPlayerGui->DrawQuestInfo->Bank;
        float width = (float)bank->GetGraphicWidth(Graphic, 0);
        float height = (float)bank->GetGraphicHeight(Graphic, 0);
        C2DVector cursor = pos;
        for (long i = 0; i < icons; ++i) {
            C2DVector uvStart(0.0f, 0.0f);
            C2DVector uvEnd(0.0f, 0.0f);
            C2DVector drawPos = cursor;
            float w, h;
            if (i >= fives) {
                w = width * 0.75f;
                h = height * 0.75f;
                cursor.X += w * Scale;
            } else {
                {
                    C2DVector textPos = C2DVector(Scale * 8.0f, Scale * 4.0f) + cursor;
                    C2DVector screen;
                    GFACoordToVCoord(&textPos, &screen);
                    CFontBank* font = g_pMainGameComponent->DefaultFont;
                    CWideString text(L"5");
                    g_pPlayerGui->Draw2DText(screen, text, font, CRGBColour(0xff, 0xff, 0xff, 0xff), Scale,
                                             ENGINE_2D_LAYER_QUEST_INFO_TEXT, false);
                }
                cursor.X += width * Scale;
                w = width * 1.2f;
                h = height * 1.2f;
                drawPos.X -= w * 0.1f;
                drawPos.Y -= h * 0.1f;
            }
            CEnginePrimitive2DSprite sprite(0, C2DSpriteBox(drawPos, drawPos + C2DVector(w * Scale, h * Scale)),
                                            ENGINE_2D_LAYER_QUEST_INFO, Graphic, 0, 2, 0, 0.0f, 0.0f,
                                            (unsigned char)(g_pPlayerGui->Alpha * 255.0f), 0xff, 0xff, 0xff, 2, uvStart, uvEnd);
            info->Renderer->AddPrimitive(&PrimitiveHandles.m_pFirst[i], sprite, 0xc0);
        }
        return height * Scale + 2.0f;
    }
    return 0.0f;
}
}
