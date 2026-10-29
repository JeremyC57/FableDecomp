// NPlayerGui::CDrawMouseCursor::MoveCursor(const C2DVector& delta) -- move the cursor by the mouse delta scaled by the GUI def's cursor scales (Y inverted) and clamp it inside the screen area minus the cursor graphic's width
// Leaf: MoveCursor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00657470, length 215 bytes (+ int3 padding), `ret 4`.
// Callees: PeekGuiDef() @ 0x00438570 (returns the CPlayerGuiDef), the player GUI's graphic bank virtual slot 23 (+0x5c)
// returning the graphic's width as an unsigned long, the screen-area getter @ 0x00434B50 (thiscall, C2DBoxF by hidden
// pointer; catalog "GetDefaultVal<CEngineGraphic>" is propagated). Globals: the player GUI @ 0x013B8790 (graphic bank at
// +0x14); float constants 2^32 @ 0x0122DCB4 and 0.0f @ 0x0122DEDC.
// Layout: CDrawBase (PDB): Component +0x10 (its screen object at +0x28); CDrawMouseCursor Pos (C2DVector) at retail +0x28
// (PDB +0x2c, the base's primitive vector is 12 bytes). CPlayerGuiDef retail MouseCursorScaleX/Y +0xa98/+0xa9c,
// MouseCursorGraphic +0xaa0 (PDB +0xb34..: the def's containers are wider in the PDB).
// Shape: the cursor width is a named float local (`fild` + 2^32 fix-up of an unsigned long); `Pos.X += delta.X * scaleX`,
// `Pos.Y -= delta.Y * scaleY`; each axis bound is computed on the FPU and the clamp is `if (pos < 0) pos = 0; else if
// (bound < pos) pos = bound;` (the zero stored as an integer).
class C2DVector { public:
    float X, Y;
};
class C2DBoxF { public:
    C2DVector TopLeft;                             // +0x00
    C2DVector BottomRight;                         // +0x08
};
class CPlayerGuiDef { public:
    unsigned char _pad[0xa98];
    float MouseCursorScaleX;                       // +0xa98
    float MouseCursorScaleY;                       // +0xa9c
    long MouseCursorGraphic;                       // +0xaa0
};
struct CGraphicBank {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual unsigned long GetGraphicWidth(long graphic);   // slot 23 (+0x5c)
};
class CPlayerGui { public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();            // 0x00438570
    unsigned char _pad[0x14];
    CGraphicBank* Bank;                            // +0x14
};
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790
class CDisplayEngine { public:
public:
    C2DBoxF GetMainWindow() const;   // 0x00434B50
};
class CMainGameComponent { public:
    unsigned char _pad[0x28];
    CDisplayEngine* Screen;                               // +0x28
};

namespace NPlayerGui {
class CDrawMouseCursor { public:
    void* __vftable;                               // +0x00
    void* PlayerGui;                               // +0x04
    void* PlayerManager;                           // +0x08
    void* World;                                   // +0x0c
    CMainGameComponent* Component;                 // +0x10
    unsigned char _pad[0x14];
    C2DVector Pos;                                 // +0x28
    void MoveCursor(const C2DVector& delta);
};

void CDrawMouseCursor::MoveCursor(const C2DVector& delta)
{
    const CPlayerGuiDef* def = &CPlayerGui::PeekGuiDef();
    float width = (float)g_pPlayerGui->Bank->GetGraphicWidth(def->MouseCursorGraphic);
    Pos.X += delta.X * def->MouseCursorScaleX;
    Pos.Y -= delta.Y * def->MouseCursorScaleY;
    C2DBoxF area = Component->Screen->GetMainWindow();
    float maxX = area.BottomRight.X - area.TopLeft.X - width;
    if (Pos.X < 0.0f)
        Pos.X = 0.0f;
    else if (maxX < Pos.X)
        Pos.X = maxX;
    float maxY = area.BottomRight.Y - area.TopLeft.Y - width;
    if (Pos.Y < 0.0f)
        Pos.Y = 0.0f;
    else if (maxY < Pos.Y)
        Pos.Y = maxY;
}
}