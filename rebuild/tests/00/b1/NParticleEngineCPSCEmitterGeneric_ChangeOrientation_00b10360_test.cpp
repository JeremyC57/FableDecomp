#include <cstdio>

// Model of CPSCEmitterGeneric::ChangeOrientation: XZ -> (x, -z, y), YZ -> (-z, x, y), else unchanged.
struct C3DVector { float X, Y, Z; };
struct Emitter {
    unsigned int Flags;                                  // bit 26 OrientationXZ, bit 27 OrientationYZ
    C3DVector ChangeOrientation(const C3DVector& v) const
    {
        C3DVector r;
        if (Flags & 0x4000000u) { r.X = v.X; r.Y = -v.Z; r.Z = v.Y; }
        else if (Flags & 0x8000000u) { r.X = -v.Z; r.Y = v.X; r.Z = v.Y; }
        else r = v;
        return r;
    }
};

int main()
{
    C3DVector v; v.X = 1.0f; v.Y = 2.0f; v.Z = 3.0f;
    Emitter e; e.Flags = 0x4000000u | 0x8000000u | 0x3FFu;
    C3DVector a = e.ChangeOrientation(v);
    if (a.X != 1.0f || a.Y != -3.0f || a.Z != 2.0f) { std::printf("00b10360_TEST FAIL\n"); return 1; }
    e.Flags = 0x8000000u;
    C3DVector b = e.ChangeOrientation(v);
    if (b.X != -3.0f || b.Y != 1.0f || b.Z != 2.0f) { std::printf("00b10360_TEST FAIL\n"); return 1; }
    e.Flags = 0x2000000u;
    C3DVector c = e.ChangeOrientation(v);
    if (c.X != 1.0f || c.Y != 2.0f || c.Z != 3.0f) { std::printf("00b10360_TEST FAIL\n"); return 1; }
    std::printf("00b10360_TEST PASS\n");
    return 0;
}