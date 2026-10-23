// Grade: DIFFER(549v549) -- one swapped instruction pair in the epilogue: retail stores the returned height
//   (`fstp [ebp+8]`) before `lea ecx,[ebp-0x14]` for ~CWideString; ours schedules the lea first. Everything else
//   (frame 0xA0, slot packing, register choice, the size temporary sharing the uv slot) is byte-identical.
//   Tried: if/return, ternary, result variable (declared/assigned in branches), default-then-override (541),
//   inverted condition, inline GetDrawnHeight helper (529), #pragma optimize("y", off), QFE toolset.
// NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounter::DrawAt(const C2DVector& pos, CEnginePrimitivePrepareInfo* info) -- refresh the counter, draw its graphic (when it has one) as a sprite sized by the bank graphic times the element scale, then its value (and " / max" when there is a maximum) beside it in the default font, vertically centred on the graphic; returns the drawn height (graphic height * scale + 2, else the font height * scale)
// Leaf: DrawAt
// Retail 0x006509F8, 549 bytes, size-optimised TU (#pragma optimize("s"): ebp frame, `leave`, unaligned entry), `ret 8`.
// Callees: vector<CEnginePrimitiveHandle>::Resize(long) 0x00443F90, the quest-info graphic bank (g_pPlayerGui 0x013B8790
// +0x1c8 draw-quest-info +0x14; vtable slots 21/22 width/height), C2DBoxF(const C2DVector&, const C2DVector&) 0x0041A7E0,
// _ftol 0x00BFEA70, CEnginePrimitive2DSprite 0x0041BEB0 (box overload), renderer AddPrimitive slot 23, GFACoordToVCoord
// 0x009E21D0 (__fastcall), IntToWideString 0x00405F00 (__fastcall), operator+(const wchar_t*, const CWideString&)
// 0x0099BFF0 (__fastcall), CWideString::operator+= 0x0099B8D0, ~CWideString 0x0099B510, CPlayerGui::Draw2DText 0x00438010.
// Pool: 2^32 0x0122DCB4, 255.0f 0x01230014, 0.5f 0x0122F59C, 0.0f 0x0122DEDC, 2.0f 0x0122DCDC, L" / " 0x0125A6A8.
// Layout: retail CQuestInfoElement is the PDB less 4 (12-byte handle vector): Scale +0x0c, PrimitiveHandles +0x10;
// CQuestInfoElementCounter PCounter +0x1c, Graphic +0x20, Value +0x24, MaxValue +0x28.
#pragma optimize("s", on)
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C2DBoxF {
    float Left, Top, Right, Bottom;
    C2DBoxF(const C2DVector& topLeft, const C2DVector& size);   // 0x0041A7E0
};
struct C2DSpriteBox {
    float Left, Top, Right, Bottom;
    C2DSpriteBox(const C2DBoxF& box) : Left(box.Left), Top(box.Top), Right(box.Right), Bottom(box.Bottom) {}
};
void __fastcall GFACoordToVCoord(const C2DVector* in, C2DVector* out);   // 0x009E21D0
class CWideString {
public:
    void* m_pStringData;
    ~CWideString();                                 // 0x0099B510
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
};
CWideString __fastcall operator+(const unsigned short* left, const CWideString& right);   // 0x0099BFF0
CWideString __fastcall IntToWideString(long value);  // 0x00405F00
class CFontBank {
public:
    void* __vftable;
    long  Height;                                   // +0x04
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
enum EEngine2DLayer { ENGINE_2D_LAYER_QUEST_INFO = 0x4a };
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
    struct CCounter;
    struct CQuestInfoElementCounter : CQuestInfoElement {
        CCounter* PCounter;                         // +0x1c
        long      Graphic;                          // +0x20
        long      Value;                            // +0x24
        long      MaxValue;                         // +0x28
        float DrawAt(const C2DVector& pos, CEnginePrimitivePrepareInfo* info);
    };
};

float CDrawQuestInfo::CQuestInfoElementCounter::DrawAt(const C2DVector& pos, CEnginePrimitivePrepareInfo* info)
{
    Update();
    float width = 0.0f;
    float height = 0.0f;
    if (Graphic > 0) {
        PrimitiveHandles.Resize(1);
        CGraphicBank* bank = g_pPlayerGui->DrawQuestInfo->Bank;
        width = (float)bank->GetGraphicWidth(Graphic, 0);
        height = (float)bank->GetGraphicHeight(Graphic, 0);
        C2DBoxF box(pos, C2DVector(width * Scale, height * Scale));
        CEnginePrimitive2DSprite sprite(0, box, ENGINE_2D_LAYER_QUEST_INFO, Graphic, 0, 2, 0, 0.0f, 0.0f,
                                        (unsigned char)(g_pPlayerGui->Alpha * 255.0f), 0xff, 0xff, 0xff, 2,
                                        C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info->Renderer->AddPrimitive(PrimitiveHandles.m_pFirst, sprite, 0xc0);
    }
    CFontBank* font = g_pMainGameComponent->DefaultFont;
    C2DVector textPos(pos.X + width * Scale, pos.Y);
    textPos.Y += (height - (float)font->Height) * 0.5f;
    C2DVector screen;
    GFACoordToVCoord(&textPos, &screen);
    CWideString text = IntToWideString(Value);
    if (MaxValue > 0)
        text += L" / " + IntToWideString(MaxValue);
    g_pPlayerGui->Draw2DText(screen, text, font, CRGBColour(0xff, 0xff, 0xff, 0xff), Scale, ENGINE_2D_LAYER_QUEST_INFO, true);
    if (height > 0.0f)
        return height * Scale + 2.0f;
    return (float)font->Height * Scale;
}
}
