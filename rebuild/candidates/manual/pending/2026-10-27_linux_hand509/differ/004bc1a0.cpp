// NGraphicAppearance::DrawRHSet(const CEnginePrimitivePrepareInfo& info, const C3DVector& pos, const CRightHandedSet& rhSet, float width, long alpha) -- debug-draw a right-handed set at pos: Up (blue), Forward ^ Up (red) and Forward (white) as three engine line primitives
// Grade: DIFFER(397v406) -- retail gives each of the three CEnginePrimitiveLine temporaries its own 0x40 frame slot (sub esp,0D0h: slots +0x68/+0x2C/+0xA4); VC7.1 here shares one slot (sub esp,58h), so every esp offset and the colour-store schedule shift. 4 grades: named locals, temporaries (this), temporaries + user dtor on the base.
// Leaf: ?DrawRHSet@NGraphicAppearance@@YIXABVCEnginePrimitivePrepareInfo@@ABVC3DVector@@ABVCRightHandedSet@@MJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004BC1A0, 406 bytes, `__fastcall` (info ecx, pos edx), `ret 0Ch`.
// Identity (proven): the PDB name set holds `?DrawRHSet@NGraphicAppearance@@YIXABVCEnginePrimitivePrepareInfo@@ABVC3DVector@@
//   ABVCRightHandedSet@@MJ@Z`, the only fastcall free function of that signature; this row sits among the
//   NGraphicAppearance::CTCGraphicAppearance functions, its two callers (0x004BE980, 0x004BEDED) pass the prepare info in
//   ecx, a position in edx, a CRightHandedSet, the width 0.05f and alpha 0xFF; it builds CEnginePrimitiveLine
//   (0x004C3090) objects and hands them to info.Engine (+0x34) slot 20 (SetPrimitive).
// Callees: CEnginePrimitiveLine::CEnginePrimitiveLine(...) 0x004C3090, CIEngine::SetPrimitive (vtable slot 20, +0x50).
// Layout: CRightHandedSet {Up +0, Forward +0xC} (Ego_r.pdb, landed 0x004BBE10); CRGBColour bytes B,G,R,A (landed 0x004C3090).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator^(const C3DVector& v) const { return C3DVector(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X); }
};
class CRightHandedSet { public:
    C3DVector Up;                            // +0x00
    C3DVector Forward;                       // +0x0C
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); }
};
class CEnginePrimitive { public: };
class CEnginePrimitiveLine : public CEnginePrimitive { public:
    unsigned char _body[0x3c];
    CEnginePrimitiveLine(unsigned __int64 id, const C3DVector& start, const C3DVector& end, const CRGBColour& colour,
                         float width, long blendMode, unsigned long flags, bool depthTest, long interpolation,
                         long blendOp);                                     // 0x004C3090
};
class CIEngine { public:
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16)
    VF(17) VF(18) VF(19)
#undef VF
    virtual void SetPrimitive(const CEnginePrimitive& primitive);   // slot 20 (+0x50)
};
class CEnginePrimitivePrepareInfo { public:
    unsigned char _pad00[0x34];
    CIEngine* Engine;                                   // +0x34
};

namespace NGraphicAppearance {
void __fastcall DrawRHSet(const CEnginePrimitivePrepareInfo& info, const C3DVector& pos, const CRightHandedSet& rhSet,
                          float width, long alpha)
{
    info.Engine->SetPrimitive(CEnginePrimitiveLine(0, pos, pos + rhSet.Up, CRGBColour(100, 100, 255, (unsigned char)alpha),
                                                   width, 0, 0, true, 0, 0));
    info.Engine->SetPrimitive(CEnginePrimitiveLine(0, pos, pos + (rhSet.Forward ^ rhSet.Up),
                                                   CRGBColour(255, 100, 100, (unsigned char)alpha), width, 0, 0, true, 0, 0));
    info.Engine->SetPrimitive(CEnginePrimitiveLine(0, pos, pos + rhSet.Forward, CRGBColour(255, 255, 255, (unsigned char)alpha),
                                                   width, 0, 0, true, 0, 0));
}
}
