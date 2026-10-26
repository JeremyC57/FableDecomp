// NPlayerGui::CDrawQuickAccessIcons::DrawIcon(const CEnginePrimitivePrepareInfo& info, const C2DVector& pos, float fade, long index,
// long backGraphic, long iconGraphic, bool selected) -- draw one quick-access slot: the back sprite (layer 0x45, half alpha when it is
// the empty-slot graphic) into handle 7, then, when the fade is above 0.0001, the icon (layer 0x44, alpha * fade, or 255/0 by selection
// when the GUI forces opacity) into the selected/normal handle vector at the index.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00645292, length 477 bytes, `ret 0x1c`.
// Callees: PeekGuiDef 0x00438570 (__fastcall); _ftol 0x00BFEA70; CDrawBase faded alpha 0x00643D5D (manifest "_Cons_val"); CEnginePrimitive2DSprite
// (C2DVector variant) 0x00493AC2; renderer slot 23 (+0x5C) AddPrimitive. 255.0f 0x01230014, 0.5f 0x0122F59C, 0.0001f 0x0129BA3C.
// Layout (descriptive): game UI +4 (force-opaque byte +0x250, alpha +0x2a0), handle vectors +0x1c (normal; back uses [7]) and +0x64
// (selected); GUI def empty-slot back graphic +0x6e0.
// Shape: `y` off + `s` on (frame pointer, leave); the handle vector chosen by a reference ternary up front; the back alpha a named copy
// reassigned in place; the icon alpha a float reassigned by a nested ternary; sprite object 0x54 bytes (0x58 grew the frame).
// Leaf: DrawIcon
#pragma optimize("y", off)
#pragma optimize("s", on)
class CEnginePrimitiveHandle { public: unsigned long Data[2]; };
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C2DCoordF {
    float X, Y;
    C2DCoordF(const C2DVector& v) : X(v.X), Y(v.Y) {}
};
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
class CPlayerGuiDef { public:
    unsigned char _pad000[0x6e0];
    long QuickAccessEmptyBackGraphic;               // +0x6e0
};
const CPlayerGuiDef* __fastcall PeekGuiDef();       // 0x00438570
struct CGameUI {
    unsigned char _pad000[0x250];
    bool ForceOpaque;                               // +0x250
    unsigned char _pad251[0x2a0 - 0x251];
    float Alpha;                                    // +0x2a0
};
struct CHandleVector {
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
    CEnginePrimitiveHandle& operator[](long i) { return _M_start[i]; }
};

namespace NPlayerGui {
class CDrawQuickAccessIcons { public:
    virtual void v00();
    CGameUI* PGameUI;                               // +0x04
    unsigned char _pad08[0x1c - 8];
    CHandleVector Handles;                          // +0x1c
    unsigned char _pad28[0x64 - 0x28];
    CHandleVector IconHandles;                      // +0x64
    unsigned char GetFadedAlpha(unsigned char alpha);   // 0x00643D5D
    void DrawIcon(const CEnginePrimitivePrepareInfo& info, const C2DVector& pos, float fade, long index, long backGraphic,
                  long iconGraphic, bool selected);
};

void CDrawQuickAccessIcons::DrawIcon(const CEnginePrimitivePrepareInfo& info, const C2DVector& pos, float fade, long index,
                                     long backGraphic, long iconGraphic, bool selected)
{
    const CPlayerGuiDef* def = PeekGuiDef();
    CHandleVector& handles = selected ? IconHandles : Handles;
    unsigned char alpha = (unsigned char)(PGameUI->Alpha * 255.0f);
    if (PGameUI->ForceOpaque && alpha < 0xff)
        alpha = 0xff;
    if (backGraphic > 0) {
        unsigned char backAlpha = alpha;
        if (backGraphic == def->QuickAccessEmptyBackGraphic)
            backAlpha = (unsigned char)(backAlpha * 0.5f);
        CEnginePrimitive2DSprite back(0, pos, 0x45, backGraphic, 0, 2, 0, 0.0f, 0.0f, GetFadedAlpha(backAlpha), 0xff, 0xff, 0xff, 0,
                                      C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Renderer->AddPrimitive(&Handles[7], back, 0xc0);
    }
    if (iconGraphic > 0 && fade > 0.0001f) {
        float iconAlpha = alpha * fade;
        if (PGameUI->ForceOpaque && alpha < 0xff)
            iconAlpha = selected ? 255.0f : 0.0f;
        CEnginePrimitive2DSprite icon(0, pos, 0x44, iconGraphic, 0, 2, 0, 0.0f, 0.0f, GetFadedAlpha((unsigned char)iconAlpha), 0xff, 0xff,
                                      0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Renderer->AddPrimitive(&handles[index], icon, 0xc0);
    }
}
}