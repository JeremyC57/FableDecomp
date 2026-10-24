// Grade: DIFFER(478v479) -- retail keeps the target width/height in edi/ebx and a zero in ebp (`cmp eax,ebp`, `mov [esp+2Ch],ebp`, the top clamp coalesced with that zero) with left/top in their own frame words (frame 18h); VC7.1 here reloads W/H, folds the 0 bound into test and keeps the box pointer in ebx (frame 14h). Taking the Clamp arguments by const reference (this version) already homes `right` in the dead box slot as retail. Tried (debakeB2): by-ref result (508), named W/H locals (474), a zero local (478), if/else-if chains (446), clip-box members as bounds (474).
// CDisplayManager::SetViewport(const C2DBoxI& box) -- clip the box to the render target, nudge degenerate edge boxes on screen and set the D3D viewport
// Leaf: ?SetViewport@CDisplayManager@@QAEXABVC2DBoxI@@@Z
// Retail: Fable.exe @ 0x009BEF80, 479 bytes, __thiscall `ret 4`. The box is clamped to [0, RenderTargetDimensions]
// (right/bottom made exclusive with +1); an unchanged ViewportBox returns early. A zero-width box at the left/top edge
// gets width/height 1, one at the right/bottom edge is moved in by one, both setting ViewportOffscreen. The D3DVIEWPORT9
// (+0x1DC, MinZ 0 / MaxZ 1) goes to IDirect3DDevice9::SetViewport (COM slot 47, +0xBC, device +0x58); on success the
// box is stored as ViewportBox (+0x1F4) and ViewportBoxFloat (+0x204). Then the render manager (scoped_ptr +0x08,
// tested through its unspecified_bool_type) is told (0x00A0AAC0) and the shader render manager singleton (0x013BC470)
// runs OnPostViewportChanged (0x009880D0).
// Layout: Ego_r.pdb CDisplayManager names and offsets (they agree with retail up to +0x214); no engine header.
struct C2DCoordI { long X, Y; };
struct C2DBoxI {
    C2DCoordI TopLeft, BottomRight;
    C2DBoxI() { }
    C2DBoxI(long x0, long y0, long x1, long y1) { TopLeft.X = x0; TopLeft.Y = y0; BottomRight.X = x1; BottomRight.Y = y1; }
    bool operator==(const C2DBoxI& b) const
    {
        return TopLeft.X == b.TopLeft.X && TopLeft.Y == b.TopLeft.Y && BottomRight.X == b.BottomRight.X && BottomRight.Y == b.BottomRight.Y;
    }
};
struct C2DCoordF { float X, Y; };
struct C2DBoxF {
    C2DCoordF TopLeft, BottomRight;
    C2DBoxF& operator=(const C2DBoxI& b)
    {
        TopLeft.X = (float)b.TopLeft.X; TopLeft.Y = (float)b.TopLeft.Y; BottomRight.X = (float)b.BottomRight.X; BottomRight.Y = (float)b.BottomRight.Y;
        return *this;
    }
};
struct C2DExtentsI { long Width, Height; };
struct D3DVIEWPORT9_ { unsigned long X, Y, Width, Height; float MinZ, MaxZ; };
struct IDirect3DDevice9_;
struct IDirect3DDevice9Vtbl_ {
    void* _slots[47];
    long (__stdcall* SetViewport)(IDirect3DDevice9_* self, const D3DVIEWPORT9_* viewport);   // slot 47 (+0xBC)
};
struct IDirect3DDevice9_ { IDirect3DDevice9Vtbl_* lpVtbl; };
class CRenderManager {
public:
    void OnViewportChanged();                       // 0x00A0AAC0
};
template <class T> class scoped_ptr {
public:
    T* px;
    T* get() const { return px; }
    T* operator->() const { return px; }
    typedef T* (scoped_ptr::*unspecified_bool_type)() const;
    operator unspecified_bool_type() const { return px == 0 ? 0 : &scoped_ptr::get; }
};
class CShaderRenderManager {
public:
    void OnPostViewportChanged();                   // 0x009880D0
};
extern CShaderRenderManager g_ShaderRenderManager;  // 0x013BC470
template <class T> inline T Clamp(const T& value, const T& low, const T& high) { return value < low ? low : (value > high ? high : value); }
class CDisplayManager {
public:
    void*                     __vftable;            // +0x000
    void*                     SystemManager;        // +0x004
    scoped_ptr<CRenderManager> PRenderManager;      // +0x008
    unsigned char             _pad00c[0x4c];
    IDirect3DDevice9_*        D3DDevice;            // +0x058
    unsigned char             _pad05c[0x138];
    C2DExtentsI               RenderTargetDimensions;   // +0x194
    unsigned char             _pad19c[0x3c];
    bool                      ViewportOffscreen;    // +0x1d8
    D3DVIEWPORT9_             D3DViewport;          // +0x1dc
    C2DBoxI                   ViewportBox;          // +0x1f4
    C2DBoxF                   ViewportBoxFloat;     // +0x204
    void SetViewport(const C2DBoxI& box);
};
void CDisplayManager::SetViewport(const C2DBoxI& box)
{
    long left = Clamp(box.TopLeft.X, 0L, RenderTargetDimensions.Width);
    long right = Clamp(box.BottomRight.X + 1, 0L, RenderTargetDimensions.Width);
    long top = Clamp(box.TopLeft.Y, 0L, RenderTargetDimensions.Height);
    long bottom = Clamp(box.BottomRight.Y + 1, 0L, RenderTargetDimensions.Height);
    C2DBoxI viewport(left, top, right, bottom);
    if (viewport == ViewportBox)
        return;
    ViewportOffscreen = false;
    if (left == 0 && right == 0) {
        viewport.BottomRight.X = 1;
        ViewportOffscreen = true;
    } else if (left == RenderTargetDimensions.Width && right == RenderTargetDimensions.Width) {
        viewport.TopLeft.X = RenderTargetDimensions.Width - 1;
        ViewportOffscreen = true;
    }
    if (top == 0 && bottom == 0) {
        viewport.BottomRight.Y = 1;
        ViewportOffscreen = true;
    } else if (top == RenderTargetDimensions.Height && bottom == RenderTargetDimensions.Height) {
        viewport.TopLeft.Y = RenderTargetDimensions.Height - 1;
        ViewportOffscreen = true;
    }
    D3DViewport.X = viewport.TopLeft.X;
    D3DViewport.Y = viewport.TopLeft.Y;
    D3DViewport.Width = viewport.BottomRight.X - viewport.TopLeft.X;
    D3DViewport.Height = viewport.BottomRight.Y - viewport.TopLeft.Y;
    D3DViewport.MinZ = 0.0f;
    D3DViewport.MaxZ = 1.0f;
    if (D3DDevice->lpVtbl->SetViewport(D3DDevice, &D3DViewport) >= 0) {
        ViewportBox = viewport;
        ViewportBoxFloat = viewport;
    }
    if (PRenderManager)
        PRenderManager->OnViewportChanged();
    g_ShaderRenderManager.OnPostViewportChanged();
}
