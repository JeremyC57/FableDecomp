// behaviour test for 0x004FB2F0
#include <stdio.h>
class C2DCoordI { public: long X, Y; };
class CWorldMap {
public:
    virtual void _s00() {}
    virtual void _s01() {}
    virtual void _s02() {}
    virtual void _s03() {}
    virtual void _s04() {}
    virtual void _s05() {}
    virtual void _s06() {}
    virtual void _s07() {}
    virtual void _s08() {}
    virtual void _s09() {}
    virtual void _s10() {}
    virtual void _s11() {}
    virtual void _s12() {}
    virtual void _s13() {}
    virtual void _s14() {}
    virtual void _s15() {}
    virtual void _s16() {}
    virtual long GetMapNumberAt(const C2DCoordI& pos) const { return pos.X > 0 ? 3 : 1; }
    virtual void _s18() {}
    virtual void _s19() {}
    virtual void _s20() {}
    virtual void _s21() {}
    virtual void _s22() {}
    virtual void _s23() {}
    virtual bool IsMapChangeable(long mapNumber) const { return mapNumber == 3; }
    virtual bool IsPosChangeable(const C2DCoordI& pos) const;         // slot 25 (+0x64)
};
bool CWorldMap::IsPosChangeable(const C2DCoordI& pos) const
{
    return IsMapChangeable(GetMapNumberAt(pos));
}
int main()
{
    CWorldMap m; C2DCoordI a = { 5, 0 }, b = { -5, 0 };
    int ok = m.IsPosChangeable(a) && !m.IsPosChangeable(b);
    printf(ok ? "004fb2f0_TEST PASS\n" : "004fb2f0_TEST FAIL\n");
    return ok ? 0 : 1;
}