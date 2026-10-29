// NPlayerGui::CDrawPerceivers::Draw(CEnginePrimitivePrepareInfo*) -- draw the perceivers eye: when the GUI shows perceivers with a non-zero alpha, ease the frame interpolation toward the perceived state (rate frame time / (fade time * 0.5), clamped at the target) while throbbing the scale up to the definition maximum, or relax the throb back to 1 once settled; draw the allegiance back sprite (neutral, friendly for 1, enemy for 2) and the eye frame picked by interpolation * 0.5 across the frame list, then, when things perceive the hero, their count (capped at 99) centred under the eye in the default font
// Leaf: Draw
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064499C, length 838 bytes (biased ebp frame `lea ebp,[esp-0x74]`, `ret 4`).
// Callees: CGameUI 0x00437860 (shows perceivers) / 0x00437880 (perceiver alpha) / Draw2DText 0x00438010, PeekGuiDef 0x00438570,
//   GFAnchorToTopRight 0x00643CA1, _ftol 0x00BFEA70, faded alpha 0x00643D5D, CEnginePrimitive2DSprite 0x00493AC2, renderer slot 23,
//   GFIntToWideString 0x00405F00 (__fastcall), CFontBank::GetStringWidth 0x0048DA62, GFACoordToVCoord 0x009E21D0, CWideString dtor
//   0x0099B510. Constants 0.5f 0x0122F59C, 1.0f 0x0122DED8, 2^32 0x0122DCB4.
// Layout: retail CDrawPerceivers is the PDB minus 4 (State +0x28, Allegiance +0x2c, FrameInterp +0x30, ThrobScale +0x34, ThingCount
//   +0x38); draw base Resources +0x10, Drawing +0x18, handle vector +0x1c; gui def +0x138..+0x16c (names descriptive). Modelled locally.
// Shape: `#pragma optimize("s", on)` alone gives the biased frame (adding `y off` gives `mov ebp,esp`); the count goes through an
//   STLport-style min returning a const reference; the text position is an inline C2DVector operator+; the default font is read through
//   an inline GetFont() accessor at both uses; the Draw2DText position is an explicit C2DVector(x, y) temporary.

#pragma optimize("s", on)
class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
};
template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }
struct C2DCoordF {
    float X, Y;
    C2DCoordF(const C2DVector& v) : X(v.X), Y(v.Y) {}
};
C2DVector __fastcall GFAnchorToTopRight(const C2DVector& in);         // 0x00643CA1
void __fastcall GFACoordToVCoord(const C2DVector* in, C2DVector* out);  // 0x009E21D0
class CWideString {
public:
    void* m_pStringData;
    ~CWideString();                                 // 0x0099B510
};
CWideString __fastcall GFIntToWideString(long value);  // 0x00405F00
class CFontBank {
public:
    float GetStringWidth(const CWideString& text);  // 0x0048DA62
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
enum EEngine2DLayer { ENGINE_2D_LAYER_TEXT = 0x43 };
class CEnginePrimitiveHandle { public: unsigned long Data[2]; };
class CEnginePrimitive2DSprite {
public:
    unsigned char _body[0x54];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DCoordF& pos, long layer, unsigned long graphic, unsigned long b, long blend,
                             long interpolation, float scaleX, float scaleY, unsigned char alpha, unsigned char red, unsigned char green,
                             unsigned char blue, unsigned long e, const C2DVector& uv0, const C2DVector& uv1);   // 0x00493AC2
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
struct LongVector { long *Start, *Finish, *End; unsigned int size() const { return Finish - Start; } };
class CPlayerGuiDef { public:
    unsigned char _pad000[0x138];
    C2DVector     PerceiverPos;                     // +0x138
    long          FriendlyBackGraphic;              // +0x140
    long          NeutralBackGraphic;               // +0x144
    long          EnemyBackGraphic;                 // +0x148
    long          _l14c;
    LongVector    PerceiverFrames;                  // +0x150
    C2DVector     CountOffset;                      // +0x15c
    float         FadeTime;                         // +0x164
    float         ThrobTime;                        // +0x168
    float         MaxThrobScale;                    // +0x16c
};
class CPlayerGui { public: static const CPlayerGuiDef& __fastcall PeekGuiDef(); };      // 0x00438570
struct CGameUI {
    bool IsShowingPerceivers();                     // 0x00437860
    unsigned char GetPerceiverAlpha();              // 0x00437880
    void Draw2DText(const C2DVector& pos, const CWideString& text, CFontBank* font, const CRGBColour& colour,
                    float scale, EEngine2DLayer layer, bool flag);   // 0x00438010
};
struct CGuiResources {
    unsigned char _pad[0x1614c];
    CFontBank*    DefaultFont;                      // +0x1614c
};
struct CHandleVector {
    CEnginePrimitiveHandle *Start, *Finish, *End;
};

namespace NPlayerGui {
class CDrawPerceivers { public:
    virtual void v00();
    CGameUI*       PlayerGui;                       // +0x04
    void*          PlayerManager;                   // +0x08
    void*          World;                           // +0x0c
    CGuiResources* Resources;                       // +0x10
    void*          Bank;                            // +0x14
    bool           Drawing;                         // +0x18
    CHandleVector  Handles;                         // +0x1c
    long           State;                           // +0x28
    long           Allegiance;                      // +0x2c
    float          FrameInterp;                     // +0x30
    float          ThrobScale;                      // +0x34
    long           ThingCount;                      // +0x38
    unsigned char GetFadedAlpha(unsigned char alpha);   // 0x00643D5D
    CFontBank* GetFont() { return Resources->DefaultFont; }
    void Draw(CEnginePrimitivePrepareInfo* info);
};

void CDrawPerceivers::Draw(CEnginePrimitivePrepareInfo* info)
{
    if (!PlayerGui->IsShowingPerceivers())
        return;
    unsigned char alpha = PlayerGui->GetPerceiverAlpha();
    if (alpha <= 0)
        return;
    Drawing = true;
    const CPlayerGuiDef* def = &CPlayerGui::PeekGuiDef();
    float target = (float)State;
    if (FrameInterp != target) {
        if (FrameInterp < target) {
            FrameInterp += info->FrameTime / (def->FadeTime * 0.5f);
            if (FrameInterp > target)
                FrameInterp = target;
        } else if (FrameInterp > target) {
            FrameInterp -= info->FrameTime / (def->FadeTime * 0.5f);
            if (FrameInterp < target)
                FrameInterp = target;
        }
        if (ThrobScale < def->MaxThrobScale) {
            ThrobScale += info->FrameTime / def->ThrobTime;
            if (ThrobScale > def->MaxThrobScale)
                ThrobScale = def->MaxThrobScale;
        }
    } else if (ThrobScale > 1.0f) {
        ThrobScale -= info->FrameTime / def->ThrobTime;
        if (ThrobScale < 1.0f)
            ThrobScale = 1.0f;
    }
    C2DVector pos = GFAnchorToTopRight(def->PerceiverPos);
    long frame = (long)(((float)def->PerceiverFrames.size() - 1.0f) * (FrameInterp * 0.5f));
    long back = def->NeutralBackGraphic;
    if (Allegiance == 1)
        back = def->FriendlyBackGraphic;
    else if (Allegiance == 2)
        back = def->EnemyBackGraphic;
    CEnginePrimitive2DSprite backSprite(0, pos, 0x45, back, 0, 2, 0, 0.0f, 0.0f, GetFadedAlpha(alpha), 0xff, 0xff, 0xff, 2,
                                        C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info->Renderer->AddPrimitive(&Handles.Start[0], backSprite, 0xc0);
    CEnginePrimitive2DSprite eyeSprite(0, pos, 0x44, def->PerceiverFrames.Start[frame], 0, 2, 0, 0.0f, 0.0f, GetFadedAlpha(alpha), 0xff,
                                       0xff, 0xff, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info->Renderer->AddPrimitive(&Handles.Start[1], eyeSprite, 0xc0);
    if (ThingCount > 0) {
        CWideString text = GFIntToWideString(Min(ThingCount, 99L));
        C2DVector textPos = pos + def->CountOffset;
        textPos.X -= GetFont()->GetStringWidth(text) * 0.5f;
        C2DVector screen;
        GFACoordToVCoord(&textPos, &screen);
        PlayerGui->Draw2DText(C2DVector(screen.X, screen.Y), text, GetFont(), CRGBColour(0xff, 0xff, 0xff, GetFadedAlpha(0xff)), 1.0f,
                              ENGINE_2D_LAYER_TEXT, true);
    }
}
}