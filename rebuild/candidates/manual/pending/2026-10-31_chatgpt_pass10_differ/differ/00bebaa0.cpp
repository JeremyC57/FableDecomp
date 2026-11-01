// 00bebaa0 CEngineInternalPrimitiveMeshBase::CalcObjectMatrix
// Leaf: ?CalcObjectMatrix@CEngineInternalPrimitiveMeshBase@@SIXAAVCMatrix3x4@@ABVC3DVector@@ABVCRightHandedSet@@M@Z
// Flags:
// Layout: original matrix48 E11..E43; protected set Up0 Forward12; vector XYZ12. No shared headers supplied.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // the engine's memberwise copy constructor
    const C3DVector Cross(const C3DVector&b)const{const C3DVector&c=b;float x=Y*c.Z;x-=Z*c.Y;float y=Z*c.X;y-=X*c.Z;float z=X*b.Y;z-=Y*b.X;return C3DVector(x,y,z);}
    const C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
};
class CRightHandedSet { protected:
    C3DVector Up;                                      // +0x00
    C3DVector Forward;                                 // +0x0c
public:
    const C3DVector&GetForward()const{return Forward;}const C3DVector&GetUp()const{return Up;}
    C3DVector GetRight() const { return Forward.Cross(Up); }   // as landed 0x0049CE00
};
class CMatrix3x4 { public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    void SetTranslateOnly(const C3DVector& t) { E41 = t.X; E42 = t.Y; E43 = t.Z; }
};
class CEngineInternalPrimitiveMeshBase { public:
    static void __fastcall CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& pos, const CRightHandedSet& rhset, float scale);
};

void __fastcall CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& pos, const CRightHandedSet& rhset, float scale)
{
    C3DVector forward = rhset.GetForward();
    C3DVector right = rhset.GetRight();
    C3DVector up = rhset.GetUp();
    right *= scale;
    forward *= scale;
    up *= scale;
    matrix.E11 = -right.X;
    matrix.E12 = -right.Y;
    matrix.E13 = -right.Z;
    matrix.E21 = -forward.X;
    matrix.E22 = -forward.Y;
    matrix.E23 = -forward.Z;
    matrix.E31 = up.X;
    matrix.E32 = up.Y;
    matrix.E33 = up.Z;
    matrix.SetTranslateOnly(pos);
}
