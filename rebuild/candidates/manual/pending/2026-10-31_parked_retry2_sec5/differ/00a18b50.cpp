// CPhysicsMesh::HasCollidedWithTriangleAt(const C3DTriangle2& triangle, float radius, const C3DVector& from, const C3DVector& direction, float length, C3DVector& position, C3DVector& normal) const -- swept-sphere test against one mesh triangle facing the move: step the sphere from `from` along direction*length in length/radius+1 steps and report the first hit position and the triangle normal
// Leaf: ?HasCollidedWithTriangleAt@CPhysicsMesh@@ABE_NABVC3DTriangle2@@MABVC3DVector@@1MAAV3@2@Z
// Grade: DIFFER(783v783) -- leaf now binds (the method is private: ?..@@ABE..); residue unchanged: frame-slot order
//   (retail step at 0x4c above edge1/edge2/to), the step/to x87 juggling, testPos += step operand order and the mov
//   edi,edi loop pad; step-first declarations, to = from + step and step + testPos all worse (sec5 retry, 4 grades)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A18B50, 783 bytes, thiscall `ret 0x1c`; unreferenced (dead code kept).
// Identity: FableWin.pdb name set (after GetPointIntersectionFromTo; point overload at 0x00A18910 `ret 0x18`), 7 args,
//   this+0x3c = TotalPoints. Callee 0x00A4B230 NBBBMaths::GetSquaredDistanceBetweenSphereTriangle (Ego_r donor 0xb9f590).
#include <math.h>
#pragma intrinsic(sqrt)

extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
    C3DVector Cross(const C3DVector& v) const
    {
        return C3DVector(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X);
    }
    float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
    __forceinline C3DVector& Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
        return *this;
    }
};

namespace NBBBMaths {
float __fastcall GetSquaredDistanceBetweenSphereTriangle(const C3DVector& centre, float radius, const C3DVector& a,
                                                         const C3DVector& b, const C3DVector& c);   // 0x00A4B230
}

class C3DTriangle2 { public:
    unsigned short VertexIndex[3];
};

template <class T> class CArray { public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T& operator[](unsigned int i) const { return _M_start[i]; }
};

class CPhysicsMesh { public:
    unsigned char           _pad00[0x30];
    CArray<C3DTriangle2>    TotalFaceIndices;   // +0x30 (PDB +0x38)
    CArray<C3DVector>       TotalPoints;        // +0x3c (PDB +0x48)
private:
    bool HasCollidedWithTriangleAt(const C3DTriangle2& triangle, float radius, const C3DVector& from, const C3DVector& direction,
                                   float length, C3DVector& position, C3DVector& normal) const;
};

bool CPhysicsMesh::HasCollidedWithTriangleAt(const C3DTriangle2& triangle, float radius, const C3DVector& from, const C3DVector& direction,
                                             float length, C3DVector& position, C3DVector& normal) const
{
    const C3DVector& v0 = TotalPoints[triangle.VertexIndex[0]];
    const C3DVector& v1 = TotalPoints[triangle.VertexIndex[1]];
    const C3DVector& v2 = TotalPoints[triangle.VertexIndex[2]];
    C3DVector edge1 = v1 - v0;
    C3DVector edge2 = v2 - v0;
    C3DVector faceNormal = edge1.Cross(edge2);
    if (faceNormal.Dot(direction) > 0.0f) {
        C3DVector to = from + direction * length;
        C3DVector step = direction * length;
        long noSteps = (long)(length / radius) + 1;
        C3DVector testPos = (noSteps == 1) ? to : from;
        step *= 1.0f / (float)noSteps;
        for (long i = 0; i < noSteps; ++i) {
            float distanceSquared = NBBBMaths::GetSquaredDistanceBetweenSphereTriangle(testPos, radius, v0, edge1, edge2);
            if (distanceSquared < radius * radius) {
                normal = faceNormal;
                normal.Normalise();
                position = from + direction * (float)sqrt(distanceSquared);
                return true;
            }
            if (i == noSteps - 1)
                testPos = to;
            else
                testPos += step;
        }
    }
    return false;
}
