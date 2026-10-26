// Behaviour test for CEngineClothSimulationMeshBase::ConvertVectorToLocal @ 0x00B80AD0
#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CMatrix3x4 { public: float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43; };
struct CEngineClothSimulationMeshBase {
    CMatrix3x4 InverseTransform;
    C3DVector ConvertVectorToLocal(const C3DVector& v) const
    {
        float x = v.Y * InverseTransform.E21; x += v.Z * InverseTransform.E31; x += v.X * InverseTransform.E11;
        float y = v.Y * InverseTransform.E22; y += v.X * InverseTransform.E12; y += v.Z * InverseTransform.E32;
        float z = v.Y * InverseTransform.E23; z += v.X * InverseTransform.E13; z += v.Z * InverseTransform.E33;
        C3DVector local; local.X = x; local.Y = y; local.Z = z;
        return local;
    }
};
int main()
{
    CEngineClothSimulationMeshBase m;
    // rows: E1x = (0,1,0), E2x = (-1,0,0), E3x = (0,0,2): v * M
    m.InverseTransform.E11 = 0; m.InverseTransform.E12 = 1; m.InverseTransform.E13 = 0;
    m.InverseTransform.E21 = -1; m.InverseTransform.E22 = 0; m.InverseTransform.E23 = 0;
    m.InverseTransform.E31 = 0; m.InverseTransform.E32 = 0; m.InverseTransform.E33 = 2;
    m.InverseTransform.E41 = 5; m.InverseTransform.E42 = 5; m.InverseTransform.E43 = 5;
    C3DVector v; v.X = 3; v.Y = 4; v.Z = 5;
    C3DVector r = m.ConvertVectorToLocal(v);
    // X = 3*0 + 4*-1 + 5*0 = -4 ; Y = 3*1 + 0 + 0 = 3 ; Z = 5*2 = 10 (translation row ignored)
    if (r.X != -4.0f || r.Y != 3.0f || r.Z != 10.0f) { std::printf("00b80ad0_TEST FAIL\n"); return 1; }
    std::printf("00b80ad0_TEST PASS\n");
    return 0;
}