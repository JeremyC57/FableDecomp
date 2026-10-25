// Behaviour test for 00c46900: CGridCircle::CGridCircle -- visit the integer cells within the radius
#include <cstdio>
#include <math.h>

struct C2DVectorI {
    int X, Y;
    C2DVectorI(int x, int y) : X(x), Y(y) { }
    float GetLength() const { return (float)sqrt((float)(X * X + Y * Y)); }
};
struct CCellVisitor { virtual void Visit(const C2DVectorI& cell) = 0; virtual ~CCellVisitor() { } };
struct CCounter : public CCellVisitor {
    int m_count, m_maxX;
    CCounter() : m_count(0), m_maxX(-100) { }
    virtual void Visit(const C2DVectorI& cell) { ++m_count; if (cell.X > m_maxX) m_maxX = cell.X; }
};
struct CGridCircle {
    CGridCircle(float radius, CCellVisitor* visitor)
    {
        int extent = (int)ceil(radius * 1.1f);
        for (int x = -extent; x <= extent; ++x)
            for (int y = -extent; y <= extent; ++y) {
                C2DVectorI cell(x, y);
                if (cell.GetLength() <= radius)
                    visitor->Visit(cell);
            }
    }
};

int main()
{
    CCounter a; CGridCircle(0.0f, &a);
    CCounter b; CGridCircle(1.0f, &b);      // origin + 4 neighbours
    CCounter c; CGridCircle(2.0f, &c);      // 13 cells
    int ok = a.m_count == 1 && b.m_count == 5 && b.m_maxX == 1 && c.m_count == 13 && c.m_maxX == 2;
    if (!ok) { std::printf("00c46900_TEST FAIL\n"); return 1; }
    std::printf("00c46900_TEST PASS\n");
    return 0;
}