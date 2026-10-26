#include <cstdio>
#include <cmath>
class C3DVector { public: float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
class C3DBoundingSphere { public:
    C3DVector Center; float Radius;
    C3DBoundingSphere() { }
    C3DBoundingSphere(const C3DVector& c, float r) : Center(c), Radius(r) { }
};
struct CSprite {
    C3DVector m_position; float m_width, m_height, m_anchorX, m_anchorY;
    bool GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere)
    {
        float w = (float)std::fabs(m_anchorX * m_width) + m_width;
        float h = (float)std::fabs(m_anchorY * m_height) + m_height;
        sphere = C3DBoundingSphere(m_position, (float)std::sqrt(w * w + h * h));
        return true;
    }
};
int main()
{
    CSprite s; s.m_position.X = 1.0f; s.m_position.Y = 2.0f; s.m_position.Z = 3.0f;
    s.m_width = 2.0f; s.m_height = 1.0f; s.m_anchorX = -0.5f; s.m_anchorY = 1.0f;   // w = 1 + 2 = 3, h = 1 + 1 = 2 -> sqrt(13)
    C3DBoundingSphere b;
    if (!s.GetBoundingSphereWorldSpace(b) || b.Center.X != 1.0f || b.Center.Z != 3.0f || std::fabs(b.Radius - 3.6055512f) > 1e-5f) { std::printf("00bacf00_TEST FAIL\n"); return 1; }
    std::printf("00bacf00_TEST PASS\n");
    return 0;
}