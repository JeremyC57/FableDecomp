
struct C2DPointI { int x; int y; };

struct C2DBoxI {
    int minX; // +0x0
    int minY; // +0x4
    int maxX; // +0x8
    int maxY; // +0xc
    int ContainsPoint(const C2DPointI* coordI) const;
};

int C2DBoxI::ContainsPoint(const C2DPointI* coordI) const
{
    int x = coordI->x;
    if (x >= minX && x < maxX)
    {
        int y = coordI->y;
        if (y >= minY && y < maxY)
            return 1;
    }
    return 0;
}