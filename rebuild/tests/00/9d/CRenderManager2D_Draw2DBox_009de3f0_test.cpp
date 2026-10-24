// Behaviour test for CRenderManager2D::Draw2DBox (0x009DE3F0): forwards to Draw2DBoxWithZ with z = 0.
#include <cstdio>

class C2DBoxF { public: float x0, y0, x1, y1; };
class CRGBColour { public: unsigned char r, g, b, a; };

class CRenderManager2D
{
public:
    const C2DBoxF* m_box; float m_z; const CRGBColour* m_colour; unsigned int m_flags; int m_calls;
    CRenderManager2D() : m_box(0), m_z(-1.0f), m_colour(0), m_flags(0), m_calls(0) {}
    bool Draw2DBoxWithZ(const C2DBoxF& box, float z, const CRGBColour& colour, unsigned int flags)
    {
        m_box = &box; m_z = z; m_colour = &colour; m_flags = flags; ++m_calls;
        return flags == 7;
    }
    bool Draw2DBox(const C2DBoxF& box, const CRGBColour& colour, unsigned int flags)
    {
        return Draw2DBoxWithZ(box, 0.0f, colour, flags);
    }
};

int main()
{
    CRenderManager2D r;
    C2DBoxF box = { 1, 2, 3, 4 };
    CRGBColour c = { 1, 2, 3, 4 };
    bool ok = r.Draw2DBox(box, c, 7);
    if (!ok || r.m_calls != 1 || r.m_box != &box || r.m_colour != &c || r.m_z != 0.0f || r.m_flags != 7) { std::printf("009de3f0_TEST FAIL\n"); return 1; }
    if (r.Draw2DBox(box, c, 3)) { std::printf("009de3f0_TEST FAIL\n"); return 1; }
    std::printf("009de3f0_TEST PASS\n");
    return 0;
}