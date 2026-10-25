// NPlayerGui::CDrawQuestInfo::CQuestInfoElementTick::DrawAt(const C2DVector& pos, const CEnginePrimitivePrepareInfo& info) -- draw a tick line at pos and return its height: a button graphic (sprite, plus its controller caption beside it) or else the text, then the tick box after it when ticked; 0 when there is neither
// Grade: DIFFER(987v987) -- 1 grade; same length, two schedule spots: the caption's X sum loads offset.X before pos.X (retail pos.X first; 12 operand/alias/operator forms identical), and the tick sprite's four zero UV stores land after the box stores (retail before); 120+ compile-only variants scored
// Leaf: ?DrawAt@CQuestInfoElementTick@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00650E98, 987 bytes, `ret 8`, slot 0 of the CQuestInfoElementTick vtable 0x0125A518
//   (Ego_r.pdb ?DrawAt@CQuestInfoElementTick@CDrawQuestInfo@NPlayerGui@@UAEMABVC2DVector@@ABVCEnginePrimitivePrepareInfo@@@Z).
//   Size-optimised NPlayerGui TU (`lea ebp,[esp-70h]` frame, `leave`, unaligned entry).
// Callees: Clear (virtual slot 2), the handle vector Resize 0x00443F90, the GUI graphic bank's width / height (virtual
//   slots 21 / 22, unsigned), C2DBoxF(const C2DVector&, const C2DExtentsF&) 0x0041A7E0 (out of line at the first site,
//   expanded at the second), CEnginePrimitive2DSprite 0x0041BEB0, the engine's SetPersistentPrimitive (slot 23), the
//   subtitle renderer's GetTextFromEmbeddedTagGraphic 0x0048DF77 / GetTextOffsetFromEmbeddedTagGraphic 0x0048DC3C,
//   CWideString != 0x0099B150 (with L"" @ 0x0122D70C) and ~CWideString 0x0099B510, GFACoordToVCoord 0x009E21D0
//   (__fastcall), CGameUI::Draw2DText 0x00438010, the font's height (slot 2) and GetStringWidth 0x0048DA62, PeekGuiDef
//   0x00438570, the float-to-int helper 0x00BFEA70.
// Constants: 255.0f @ 0x01230014, 4.0f @ 0x0123088C, 0.75f @ 0x01238174, 3.0f @ 0x0122DED4, 2.0f @ 0x0122DCDC,
//   0.0f @ 0x0122DEDC, 4294967296.0f @ 0x0122DCB4.
// Layout: CQuestInfoElement = Ego_r.pdb (ID +4, ScriptID +8, Scale +0xc, the 12-byte retail PrimitiveHandles +0x10);
//   CQuestInfoElementTick Graphic +0x1c, Text +0x20, Ticked +0x24 (the landed constructor 0x00656C77). Game UI 0x013B8790:
//   subtitle renderer +0x18, graphic data +0x1c8 (bank +0x14), alpha +0x2a0; the game 0x013B86A0 font +0x1614C; the
//   player GUI def's tick graphic +0x590.
// Shape: C2DBoxF has a member-wise operator= (the first box is built in a named temporary and copied with dword moves);
//   both bank sizes are read into float locals before the height is set; the GUI def is a named local; the caption font
//   is a named local; the tick position is copied then advanced.
#pragma optimize("s", on)
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct C2DExtentsF {
    float X, Y;
    C2DExtentsF(float x, float y) : X(x), Y(y) { }
};
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF() { }
    C2DBoxF(const C2DVector& topLeft, const C2DExtentsF& extents)
        : TLX(topLeft.X), TLY(topLeft.Y), BRX(topLeft.X + extents.X), BRY(extents.Y + topLeft.Y) { }
    C2DBoxF& operator=(const C2DBoxF& o) { TLX = o.TLX; TLY = o.TLY; BRX = o.BRX; BRY = o.BRY; return *this; }
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CWideString {
public:
    unsigned short* m_pData;
    ~CWideString();                                            // 0x0099B510
    bool operator!=(const unsigned short* text) const;                // 0x0099B150
};
struct CEnginePrimitive2DSprite {
    unsigned char _data[0x54];
    CEnginePrimitive2DSprite(unsigned __int64 unused, const C2DBoxF& box, unsigned long layer, unsigned long bank,
        unsigned long frame, unsigned long blend, unsigned long interpolation, float angleStart, float angleEnd,
        unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3, unsigned long flags,
        const C2DVector& uvStart, const C2DVector& uvEnd);      // 0x0041BEB0
};
struct CEnginePrimitiveHandle { long m_0; void* m_pData; };
struct CPrimitiveHandles {
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
    void Resize(unsigned long n);                              // 0x00443F90
};
struct CIEngine {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive2DSprite& primitive,
                                        unsigned long flags);  // slot 23 (+0x5c)
};
struct CEnginePrimitivePrepareInfo { unsigned char _pad[0x34]; CIEngine* Engine; };
struct CGraphicBank {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20();
    virtual unsigned long GetWidth(long graphic, long frame);  // slot 21 (+0x54)
    virtual unsigned long GetHeight(long graphic, long frame); // slot 22 (+0x58)
};
struct CGraphicData { unsigned char _pad[0x14]; CGraphicBank* Bank; };
class CFontBank {
public:
    virtual void v00(); virtual void v01();
    virtual float GetHeight() const;                           // slot 2 (+0x08)
    float GetStringWidth(const CWideString& text);             // 0x0048DA62
};
struct CSubtitleRenderer {
    CWideString GetTextFromEmbeddedTagGraphic(long graphic) const;            // 0x0048DF77
    const C2DVector& GetTextOffsetFromEmbeddedTagGraphic(long graphic) const; // 0x0048DC3C
};
enum EEngine2DLayer { ENGINE_2D_LAYER_TEXT = 0x43, ENGINE_2D_LAYER_QUEST = 0x4a };
struct CGameUI {
    unsigned char     _pad00[0x18];
    CSubtitleRenderer Subtitles;                               // +0x18
    unsigned char     _pad19[0x1c8 - 0x19];
    CGraphicData*     GraphicData;                             // +0x1c8
    unsigned char     _pad1cc[0x2a0 - 0x1cc];
    float             Alpha;                                   // +0x2a0
    void Draw2DText(const C2DVector& pos, const CWideString& text, const CFontBank& font, const CRGBColour& colour,
                    float scale, EEngine2DLayer layer, bool flag);           // 0x00438010
};
extern CGameUI* g_pGameUI;                                     // 0x013B8790
struct CGame { unsigned char _pad[0x1614c]; CFontBank* SmallFont; };         // +0x1614c
extern CGame* g_pGame;                                         // 0x013B86A0
struct CPlayerGuiDef { unsigned char _pad[0x590]; long TickGraphic; };       // +0x590 (retail)
extern const CPlayerGuiDef* __fastcall PeekGuiDef();          // 0x00438570
void __fastcall GFACoordToVCoord(const C2DVector& in, C2DVector& out);      // 0x009E21D0

namespace NPlayerGui {
struct CDrawQuestInfo {

struct CQuestInfoElement {
    virtual float DrawAt(const C2DVector& pos, const CEnginePrimitivePrepareInfo& info) = 0;
    virtual bool Update();
    virtual void Clear();
    long              ID;                                      // +0x04
    long              ScriptID;                                // +0x08
    float             Scale;                                   // +0x0c
    CPrimitiveHandles PrimitiveHandles;                        // +0x10
};

struct CQuestInfoElementTick : CQuestInfoElement {
    long        Graphic;                                       // +0x1c
    CWideString Text;                                          // +0x20
    bool        Ticked;                                        // +0x24
    virtual float DrawAt(const C2DVector& pos, const CEnginePrimitivePrepareInfo& info);
};

};

float CDrawQuestInfo::CQuestInfoElementTick::DrawAt(const C2DVector& pos, const CEnginePrimitivePrepareInfo& info)
{
    Clear();
    float textWidth;
    float height;
    long tickIndex;
    C2DBoxF box;
    if (Graphic > 0) {
        PrimitiveHandles.Resize(2);
        CGraphicBank* bank = g_pGameUI->GraphicData->Bank;
        tickIndex = 1;
        float width = (float)bank->GetWidth(Graphic, 0);
        float h = (float)bank->GetHeight(Graphic, 0);
        textWidth = width;
        height = h;
        C2DBoxF spriteBox(pos, C2DExtentsF(width * Scale, h * Scale));
        box = spriteBox;
        CEnginePrimitive2DSprite sprite(0, box, ENGINE_2D_LAYER_QUEST, Graphic, 0, 2, 0, 0.0f, 0.0f,
            (unsigned char)(g_pGameUI->Alpha * 255.0f), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine->SetPersistentPrimitive(PrimitiveHandles._M_start[0], sprite, 0xc0);
        CWideString caption = g_pGameUI->Subtitles.GetTextFromEmbeddedTagGraphic(Graphic);
        if (caption != L"") {
            const C2DVector& offset = g_pGameUI->Subtitles.GetTextOffsetFromEmbeddedTagGraphic(Graphic);
            C2DVector textPos;
            C2DVector sum(pos.X + offset.X, offset.Y + pos.Y);
            GFACoordToVCoord(C2DVector(sum.X - 4.0f, sum.Y - 4.0f), textPos);
            CFontBank* font = g_pGame->SmallFont;
            g_pGameUI->Draw2DText(textPos, caption, *font, CRGBColour(255, 255, 255, 255), 0.75f, ENGINE_2D_LAYER_TEXT, true);
        }
    } else if (Text != L"") {
        PrimitiveHandles.Resize(1);
        tickIndex = 0;
        C2DVector textPos;
        GFACoordToVCoord(pos, textPos);
        CFontBank* font = g_pGame->SmallFont;
        g_pGameUI->Draw2DText(textPos, Text, *font, CRGBColour(255, 255, 255, 255), 0.75f, ENGINE_2D_LAYER_TEXT, true);
        height = font->GetHeight();
        textWidth = font->GetStringWidth(Text);
    } else
        return 0.0f;
    const CPlayerGuiDef* def = PeekGuiDef();
    long tick = def->TickGraphic;
    if (tick > 0) {
        C2DVector tickPos(pos.X, pos.Y);
        tickPos.X = tickPos.X + textWidth * Scale + 3.0f;
        CGraphicBank* bank = g_pGameUI->GraphicData->Bank;
        float width = (float)bank->GetWidth(tick, 0);
        float tickHeight = (float)bank->GetHeight(tick, 0);
        if (tickHeight > height)
            height = tickHeight;
        if (Ticked) {
            box = C2DBoxF(tickPos, C2DExtentsF(width * Scale, tickHeight * Scale));
            CEnginePrimitive2DSprite sprite(0, box, ENGINE_2D_LAYER_QUEST, tick, 0, 2, 0, 0.0f, 0.0f,
                (unsigned char)(g_pGameUI->Alpha * 255.0f), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine->SetPersistentPrimitive(PrimitiveHandles._M_start[tickIndex], sprite, 0xc0);
        }
    }
    return height * Scale + 2.0f;
}

}
