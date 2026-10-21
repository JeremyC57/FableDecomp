// CShape::EditorDrawHighlightPoints(CEnginePrimitivePrepareInfo* info, CWorldMap* worldMap, C3DVector* cursor, CRGBColour* colour) -- editor view of a shape: a ground-snapped post per point (a blue marker when the cursor is near it) and a cross on every selected point
// Leaf: EditorDrawHighlightPoints
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007617A0, length 467 bytes (`ret 10h`, no EH frame).
// Callees: CWorldMap::GetGroundSizeZAt 0x0063A300, two __fastcall editor draw helpers 0x00875FB0 (manifest
// "EditorDrawHighlightPoints_Impl": info, position, float colour triple, radius) and 0x008764E0 (manifest
// "RenderParticleEmitter_4particles": info, position, size), the engine's slot 20 (+0x50, one-off primitive).
// Floats: 0.25f 0x0126A1FC, 0.15f 0x0126A1F4, 0.0625f 0x0122DC98.
// Layout: retail CShape sits below Ego_r.pdb (STLport vectors are 12 bytes): Points +0x08, SelectedPoints +0x14
// (vector of C3DVector iterators); CEnginePrimitivePrepareInfo::Engine +0x34; CEnginePrimitiveLine (0x3c bytes,
// Ego_r.pdb: Type 6, InterpolationSet, 3-bool ThemeChannelDesc, Start +0x0c, End +0x18, Colour +0x24, Width,
// BankIndex, BlendMode, BlendOp, UseZBuffer +0x38) built inline from the out-of-line ctor's 0x004C3090 parameters.
// Shape: C3DVector needs a user copy ctor (X and Z reach the second vector from the FPU, Y as an integer copy);
// CRGBColour's copy ctor assigns R, G, B, A in that order (byte loads +2, +1, +3, +0); `dy*dy + dx*dx` order.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(const CRGBColour& c) { R = c.R; G = c.G; B = c.B; A = c.A; }
};
struct CEnginePrimitiveThemeChannelDesc {
    bool Channel0, Channel1, Channel2;
    CEnginePrimitiveThemeChannelDesc() : Channel0(false), Channel1(false), Channel2(false) {}
};
struct CEnginePrimitive {
    long                             Type;              // +0x00
    long                             InterpolationSet;  // +0x04
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;  // +0x08
    CEnginePrimitive(long type, long interpolation) : Type(type), InterpolationSet(interpolation) {}
};
struct CEnginePrimitiveLine : CEnginePrimitive {
    C3DVector     Start;                                // +0x0c
    C3DVector     End;                                  // +0x18
    CRGBColour    Colour;                               // +0x24
    float         Width;                                // +0x28
    unsigned long BankIndex;                            // +0x2c
    long          BlendMode;                            // +0x30
    long          BlendOp;                              // +0x34
    bool          UseZBuffer;                           // +0x38
    CEnginePrimitiveLine(unsigned __int64 id, const C3DVector& start, const C3DVector& end, const CRGBColour& colour,
                         float width, long blendMode, unsigned long flags, bool depthTest, long interpolation, long blendOp)
        : CEnginePrimitive(6, interpolation), Start(start), End(end), Colour(colour), Width(width), BankIndex(flags),
          BlendMode(blendMode), BlendOp(blendOp), UseZBuffer(depthTest) {}
};
struct CIEngine {
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16)
    VF(17) VF(18) VF(19)
#undef VF
    virtual void SetPrimitive(const CEnginePrimitive& primitive);   // slot 20 (+0x50)
};
struct CEnginePrimitivePrepareInfo {
    unsigned char _pad00[0x34];
    CIEngine* Engine;                                   // +0x34
};
struct CWorldMap {
    float GetGroundSizeZAt(const C3DVector* position);  // 0x0063A300
};
void __fastcall EditorDrawSphere(CEnginePrimitivePrepareInfo* info, const C3DVector& position, const C3DVector& colour, float radius);   // 0x00875FB0
void __fastcall EditorDrawCross(CEnginePrimitivePrepareInfo* info, const C3DVector& position, float size);   // 0x008764E0
struct CPointVector { C3DVector* _M_start; C3DVector* _M_finish; C3DVector* _M_end_of_storage; };
struct CPointIteratorVector { C3DVector** _M_start; C3DVector** _M_finish; C3DVector** _M_end_of_storage; };
struct CShape {
    virtual void v00();
    void*                Owner;                         // +0x04
    CPointVector         Points;                        // +0x08
    CPointIteratorVector SelectedPoints;                // +0x14
    void EditorDrawHighlightPoints(CEnginePrimitivePrepareInfo* info, CWorldMap* worldMap, C3DVector* cursor, CRGBColour* colour);
};

void CShape::EditorDrawHighlightPoints(CEnginePrimitivePrepareInfo* info, CWorldMap* worldMap, C3DVector* cursor, CRGBColour* colour)
{
    for (C3DVector* it = Points._M_start; it != Points._M_finish; ++it) {
        C3DVector top = *it;
        C3DVector bottom(top.X, top.Y + 0.25f, top.Z);
        top.Z = worldMap->GetGroundSizeZAt(&top) + 0.15f;
        bottom.Z = worldMap->GetGroundSizeZAt(&bottom) + 0.15f;
        float dx = top.X - cursor->X;
        float dy = top.Y - cursor->Y;
        if (dy * dy + dx * dx < 0.0625f)
            EditorDrawSphere(info, top, C3DVector(0.05f, 0.05f, 0.95f), 0.25f);
        info->Engine->SetPrimitive(CEnginePrimitiveLine(0, top, bottom, *colour, 0.05f, 0, 0, true, 0, 0));
    }
    for (C3DVector** sel = SelectedPoints._M_start; sel != SelectedPoints._M_finish; ++sel) {
        C3DVector point = **sel;
        point.Z = worldMap->GetGroundSizeZAt(&point) + 0.15f;
        EditorDrawCross(info, point, 0.25f);
    }
}
