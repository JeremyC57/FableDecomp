// Grade: DIFFER(507v508) -- frame slots only: retail homes BOTH GFAnchorToTopRight results in one named word pair [ebp-0x1c] (each sprite gets a converted copy) with the three sprites in distinct slots; ours: a named pair per call (frame +8) or, scoped, shared sprites, or (this file) a temporary first result read through its pointer.
// NPlayerGui::CDrawWorldTime::Draw(const CEnginePrimitivePrepareInfo& info) -- when the world-time clock is visible with a non-zero alpha: mark drawn
// and submit three top-right-anchored sprites: the back (layer 0x46), the hand rotated by the world time + 0.75 (layer 0x44) and the centre cap
// at scale 0.25 (layer 0x43), each with the faded alpha.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064CA15, length 508 bytes, `ret 4`.
// Callees: game UI (+4) 0x00437860 visible / 0x00437880 alpha (manifest labels propagated), PeekGuiDef 0x00438570, GFAnchorToTopRight 0x00643CA1
// (__fastcall, C2DVector by value), CDrawBase faded alpha 0x00643D5D, CEnginePrimitive2DSprite(C2DVector variant) 0x00493AC2, renderer slot 23.
// 0.75f 0x01238174, 0.25f 0x0123078C. Layout (descriptive): owner +0xc (+0x1c source, rotation +8), drawn +0x18, handles +0x1c; GUI def back
// pos +0x304 / graphic +0x30c, hand pos +0x2f4 / graphic +0x2fc, centre graphic +0x300.
// Tried: named pos per sprite (514), one reassigned pos (temp + copy, 518), scoped named pos (sprites share, 502).
// Leaf: Draw
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CEnginePrimitiveHandle { unsigned long Data[2]; };
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C2DCoordF {
    float X, Y;
    C2DCoordF(const C2DVector& v) : X(v.X), Y(v.Y) {}
};
C2DVector __fastcall GFAnchorToTopRight(const C2DVector& pos);   // 0x00643CA1
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
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DSprite& primitive, long flags);   // slot 23 (+0x5C)
};
class CEnginePrimitivePrepareInfo {
public:
    unsigned char _pad00[0x34];
    CEnginePrimitiveRenderer* Renderer;             // +0x34
};
struct CPlayerGuiDef {
    unsigned char _pad000[0x2f4];
    C2DVector WorldTimeHandPos;                     // +0x2f4
    unsigned long WorldTimeHandGraphic;             // +0x2fc
    unsigned long WorldTimeCentreGraphic;           // +0x300
    C2DVector WorldTimeBackPos;                     // +0x304
    unsigned long WorldTimeBackGraphic;             // +0x30c
};
const CPlayerGuiDef* __fastcall PeekGuiDef();       // 0x00438570
struct CGameUI {
    bool IsWorldTimeVisible();                      // 0x00437860
    unsigned char GetWorldTimeAlpha();              // 0x00437880
};
struct CWorldTimeSource { unsigned char _pad[8]; float Rotation; };   // +8
struct CWorldTimeOwner { unsigned char _pad[0x1c]; CWorldTimeSource* Source; };   // +0x1c

namespace NPlayerGui {
struct CDrawWorldTime {
    virtual void v00();
    CGameUI* PGameUI;                               // +0x04
    unsigned char _pad08[4];
    CWorldTimeOwner* POwner;                        // +0x0c
    unsigned char _pad10[8];
    bool Drawn;                                     // +0x18
    unsigned char _pad19[3];
    CEnginePrimitiveHandle* Handles;                // +0x1c vector start
    unsigned char GetFadedAlpha(unsigned char alpha);   // 0x00643D5D
    void Draw(const CEnginePrimitivePrepareInfo& info);
};

void CDrawWorldTime::Draw(const CEnginePrimitivePrepareInfo& info)
{
    if (PGameUI->IsWorldTimeVisible()) {
        unsigned char alpha = PGameUI->GetWorldTimeAlpha();
        if (alpha > 0) {
            Drawn = true;
            const CPlayerGuiDef* def = PeekGuiDef();
            CEnginePrimitive2DSprite back(0, GFAnchorToTopRight(def->WorldTimeBackPos), 0x46, def->WorldTimeBackGraphic, 0, 2, 0, 0.0f, 0.0f, GetFadedAlpha(alpha), 0xff, 0xff, 0xff, 0,
                                          C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Renderer->AddPrimitive(&Handles[0], back, 0xc0);
            C2DVector handPos = GFAnchorToTopRight(def->WorldTimeHandPos);
            float rotation = POwner->Source->Rotation + 0.75f;
            CEnginePrimitive2DSprite hand(0, handPos, 0x44, def->WorldTimeHandGraphic, 0, 2, 0, rotation, rotation, GetFadedAlpha(alpha), 0xff, 0xff, 0xff, 2,
                                          C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Renderer->AddPrimitive(&Handles[1], hand, 0xc0);
            CEnginePrimitive2DSprite centre(0, handPos, 0x43, def->WorldTimeCentreGraphic, 0, 2, 0, 0.25f, 0.25f, GetFadedAlpha(alpha), 0xff, 0xff, 0xff, 2,
                                            C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Renderer->AddPrimitive(&Handles[2], centre, 0xc0);
        }
    }
}
}
