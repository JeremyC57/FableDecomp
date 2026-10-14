#include <cstdio>
struct C2DVector { float X, Y; };
struct CEngineCamera { unsigned char _pad[0x14]; float m_screenOriginX; float m_screenOriginY; unsigned char _pad1c[0xb0 - 0x1c]; float m_screenScaleX; float m_screenScaleY;
    void Transform(C2DVector& p) const { p.X = (p.X - m_screenOriginX) / m_screenScaleX * 2.0f - 1.0f; p.Y = -((p.Y - m_screenOriginY) / m_screenScaleY * 2.0f - 1.0f); } };
int main() { CEngineCamera c; c.m_screenOriginX = 0; c.m_screenOriginY = 0; c.m_screenScaleX = 640; c.m_screenScaleY = 480; C2DVector p = { 640, 0 }; c.Transform(p);
    if (p.X != 1.0f || p.Y != 1.0f) { std::printf("00b2fb70_TEST FAIL\n"); return 1; } C2DVector q = { 160, 480 }; c.Transform(q); if (q.X != -0.5f || q.Y != -1.0f) { std::printf("00b2fb70_TEST FAIL\n"); return 1; } std::printf("00b2fb70_TEST PASS\n"); return 0; }
