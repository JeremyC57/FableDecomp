// Grade: DIFFER(438v454) -- x87 scheduling/frame: retail keeps the top box's right/bottom on the x87 stack until after the uv temporaries are stored and writes the box last (TL from one `fld -8.0` + fst/fstp), and stages the bottom box's BR.Y through a temp word; ours stores the named box first. A temporary box (variant e) loses the biased `lea ebp,[esp-74h]` frame (0xd0 vs 0xdc).
// NPlayerGui::CDrawLetterBoxBlackBars::Draw(const CEnginePrimitivePrepareInfo& info) -- mark drawn, compute the letterbox bar height from the view's
// width/height and the virtual screen size, then submit two black sprites (top band and bottom band, 8 units of overscan) through the renderer.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00644677, length 454 bytes, `ret 4`.
// Callees: view getters 0x004349F0 / 0x00434A00 (float, thiscall; manifest labels propagated), GFAYToVY 0x009E1E40 / GFAXToVX 0x009E1E10
// (__stdcall), CEnginePrimitive2DSprite(16 args) 0x0041BEB0, renderer slot 23 (+0x5C) on info +0x34. Screen doubles 0x013961E8/0x013961F0,
// 8.0f 0x0125916C, -8.0f 0x0125A298, 0.5f 0x0122F59C.
// Layout: this +0x10 owner (+0x28 view), +0x18 drawn flag, +0x1c primitive handle vector start (descriptive); sprite model as the landed
// CTCFireheartMinigame 0x007E7C90.
// Leaf: Draw
#pragma optimize("s", on)
struct CEnginePrimitiveHandle { unsigned long Data[2]; };
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
};
struct C2DBoxF {
    C2DVector TopLeft, BottomRight;
    C2DBoxF(float left, float top, float right, float bottom) : TopLeft(left, top), BottomRight(right, bottom) {}
};
class CEnginePrimitive2DSprite {
public:
    unsigned char _body[0x54];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DBoxF& box, long layer, unsigned long a, unsigned long b, long blend,
                             long interpolation, float c, float d, unsigned char red, unsigned char green, unsigned char blue,
                             unsigned char alpha, unsigned long e, const C2DVector& uv0, const C2DVector& uv1);   // 0x0041BEB0
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
    CEnginePrimitiveRenderer* Renderer;             // +0x34
};
float __stdcall GFAYToVY(float y);                 // 0x009E1E40
float __stdcall GFAXToVX(float x);                 // 0x009E1E10
extern double g_screenHeight;                      // 0x013961F0
extern double g_screenWidth;                       // 0x013961E8

struct CLetterBoxView {
    float GetWidth() const;                        // 0x004349F0
    float GetHeight() const;                       // 0x00434A00
};
struct CLetterBoxOwner {
    unsigned char _pad00[0x28];
    CLetterBoxView* View;                          // +0x28
};

namespace NPlayerGui {
struct CDrawLetterBoxBlackBars {
    unsigned char _pad00[0x10];
    CLetterBoxOwner* Owner;                        // +0x10
    unsigned char _pad14[4];
    bool Drawn;                                    // +0x18
    unsigned char _pad19[3];
    CEnginePrimitiveHandle* Handles;               // +0x1c vector start
    void Draw(const CEnginePrimitivePrepareInfo& info);
};

void CDrawLetterBoxBlackBars::Draw(const CEnginePrimitivePrepareInfo& info)
{
    Drawn = true;
    CLetterBoxView* view = Owner->View;
    float width = view->GetWidth();
    float height = view->GetHeight();
    float vy = GFAYToVY((float)g_screenHeight);
    float barHeight = (vy - ((GFAXToVX((float)g_screenWidth) / width - vy) * height + vy)) * 0.5f;
    C2DBoxF topBox(-8.0f, -8.0f, GFAXToVX((float)g_screenWidth) + 8.0f - 8.0f, barHeight - 8.0f);
    CEnginePrimitive2DSprite top(0, topBox, 0x50, 0, 0, 2, 0, 0.0f, 0.0f, 0xff, 0, 0, 0, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Renderer->AddPrimitive(&Handles[0], top, 0x40);
    C2DVector size(GFAXToVX((float)g_screenWidth) + 8.0f, barHeight + 8.0f);
    float bottomY = GFAYToVY((float)g_screenHeight) - barHeight;
    C2DBoxF bottomBox(-8.0f, bottomY, size.X - 8.0f, bottomY + size.Y);
    CEnginePrimitive2DSprite bottom(0, bottomBox, 0x50, 0, 0, 2, 0, 0.0f, 0.0f, 0xff, 0, 0, 0, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Renderer->AddPrimitive(&Handles[1], bottom, 0x40);
}
}
