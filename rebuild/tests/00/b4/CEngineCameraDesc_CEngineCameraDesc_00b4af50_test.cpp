// Behaviour test for CEngineCameraDesc::CEngineCameraDesc(const CEngineCameraDesc&) @ 0x00B4AF50
#include <cstdio>
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF() {}
    C2DBoxF(const C2DBoxF& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) {}
};
struct CMatrix3x4 { float m_elements[12]; };
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct CEngineCameraDesc {
    C2DBoxF Window; CMatrix3x4 RotationMatrix; C3DVector WorldPosition;
    float HorizontalFOV, VerticalFOV; bool Use2DFOV;
    float NearZPlane, FarZPlane, ZBufferStart, ZBufferEnd; bool StateValid;
    CEngineCameraDesc() {}
    CEngineCameraDesc(const CEngineCameraDesc& other)
        : Window(other.Window), RotationMatrix(other.RotationMatrix), WorldPosition(other.WorldPosition),
          HorizontalFOV(other.HorizontalFOV), VerticalFOV(other.VerticalFOV), Use2DFOV(other.Use2DFOV),
          NearZPlane(other.NearZPlane), FarZPlane(other.FarZPlane), ZBufferStart(other.ZBufferStart),
          ZBufferEnd(other.ZBufferEnd), StateValid(other.StateValid) {}
};
int main()
{
    CEngineCameraDesc a;
    int i;
    a.Window.TLX = 1; a.Window.TLY = 2; a.Window.BRX = 3; a.Window.BRY = 4;
    for (i = 0; i < 12; ++i) a.RotationMatrix.m_elements[i] = (float)(i + 10);
    a.WorldPosition.X = 5; a.WorldPosition.Y = 6; a.WorldPosition.Z = 7;
    a.HorizontalFOV = 0.5f; a.VerticalFOV = 0.25f; a.Use2DFOV = true;
    a.NearZPlane = 0.1f; a.FarZPlane = 100.0f; a.ZBufferStart = 0.0f; a.ZBufferEnd = 1.0f; a.StateValid = true;
    CEngineCameraDesc b(a);
    for (i = 0; i < 12; ++i) if (b.RotationMatrix.m_elements[i] != (float)(i + 10)) { std::printf("00b4af50_TEST FAIL\n"); return 1; }
    if (b.Window.BRX != 3 || b.Window.BRY != 4 || b.WorldPosition.Z != 7 || b.HorizontalFOV != 0.5f || b.VerticalFOV != 0.25f ||
        !b.Use2DFOV || b.NearZPlane != 0.1f || b.FarZPlane != 100.0f || b.ZBufferEnd != 1.0f || !b.StateValid) {
        std::printf("00b4af50_TEST FAIL\n"); return 1;
    }
    std::printf("00b4af50_TEST PASS\n");
    return 0;
}