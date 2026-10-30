// CEngineInternalPrimitiveAnimatedMesh::CalcInterpolatedObjectTransform(CMatrix3x4&, float, bool) const -- interpolate the world position and orientation at `t`, pick the mesh scale (or 1) and build the object matrix
// Leaf: CalcInterpolatedObjectTransform
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCEF80, 187 bytes (+ int3 padding), `ret 0Ch`. Mangled
//   ?CalcInterpolatedObjectTransform@CEngineInternalPrimitiveAnimatedMesh@@UBEXAAVCMatrix3x4@@M_N@Z.
// Callees: CRightHandedSet::Interpolate(const CRightHandedSet&, float) const 0x00A23190 (returns the set by hidden pointer),
//   CalcObjectMatrix 0x00BEBAA0 (`__fastcall`: matrix in ecx, position in edx, orientation and scale on the stack).
// Layout (Ego_r.pdb, retail agrees): CEngineInternalPrimitiveMeshBase::Scale +0x58; WorldPos (C3DPositionInterpolator, vptr
//   +0x84) StartPosition +0x88 / EndPosition +0x94; Orientation (C3DOrientationInterpolator, vptr +0xa0) StartOrientation
//   +0xa4 / EndOrientation +0xbc (CRightHandedSet = Up, Forward).
// Shape: position = start + (end - start) * t through inline C3DVector operators (by-value results, no user copy ctor);
//   the scale is a named float local (homed in the dead bool slot); both helpers take references to the frame locals.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};

class CRightHandedSet { public:
    C3DVector Up;                                      // +0x00
    C3DVector Forward;                                 // +0x0c
    CRightHandedSet Interpolate(const CRightHandedSet& other, float t) const;   // 0x00A23190
};

class CMatrix3x4 { public: float E[12]; };

class CEngineInternalPrimitiveMeshBase { public:
    static void __fastcall CalcObjectMatrix(CMatrix3x4& out, const C3DVector& position, const CRightHandedSet& orientation, float scale);   // 0x00BEBAA0
};

class C3DPositionInterpolator { public:
    void*     __vftable;
    C3DVector StartPosition;                           // +0x04
    C3DVector EndPosition;                             // +0x10
};
class C3DOrientationInterpolator { public:
    void*           __vftable;
    CRightHandedSet StartOrientation;                  // +0x04
    CRightHandedSet EndOrientation;                    // +0x1c
};

class CEngineInternalPrimitiveAnimatedMesh { public:
    virtual void CalcInterpolatedObjectTransform(CMatrix3x4& out, float t, bool applyScale) const;
    unsigned char              _pad04[0x58 - 4];
    float                      Scale;                  // +0x58
    unsigned char              _pad5c[0x84 - 0x5c];
    C3DPositionInterpolator    WorldPos;               // +0x84
    C3DOrientationInterpolator Orientation;            // +0xa0
};

void CEngineInternalPrimitiveAnimatedMesh::CalcInterpolatedObjectTransform(CMatrix3x4& out, float t, bool applyScale) const
{
    C3DVector position = WorldPos.StartPosition + (WorldPos.EndPosition - WorldPos.StartPosition) * t;
    CRightHandedSet orientation = Orientation.StartOrientation.Interpolate(Orientation.EndOrientation, t);
    float scale;
    if (applyScale)
        scale = Scale;
    else
        scale = 1.0f;
    CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(out, position, orientation, scale);
}