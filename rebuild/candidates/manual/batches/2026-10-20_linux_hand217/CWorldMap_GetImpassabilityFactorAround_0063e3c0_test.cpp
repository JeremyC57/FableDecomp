// Behaviour test for CWorldMap::GetImpassabilityFactorAround 0x0063E3C0: nine neighbour factors, 1 where impassable.
#include <cstdio>
#include <cstring>

struct C3DVector { float X, Y, Z; };
struct C2DCoordI { long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) { } };
static const C2DCoordI g_BlockNeighbourOffsets[9] = {
    C2DCoordI(-1, -1), C2DCoordI(0, -1), C2DCoordI(1, -1), C2DCoordI(1, 0), C2DCoordI(1, 1),
    C2DCoordI(0, 1), C2DCoordI(-1, 1), C2DCoordI(-1, 0), C2DCoordI(0, 0) };
struct CFloatVector {
    float* _M_start; float* _M_finish; float* _M_end_of_storage;
    float* begin() { return _M_start; }
    float* end() { return _M_finish; }
    float& operator[](unsigned long n) { return *(begin() + n); }
    static float* CopyDown(float* src, float* srcEnd, float* dest)
    {
        if (src == srcEnd) return dest;
        unsigned long count = (unsigned long)((char*)srcEnd - (char*)src);
        return (float*)((char*)std::memmove(dest, src, count) + count);
    }
    void erase(float* first, float* last) { _M_finish = CopyDown(last, _M_finish, first); }
    void clear() { erase(begin(), end()); }
};
static int g_peeks = 0;
struct CGameDefinitionManager { };
static CGameDefinitionManager g_defs;
struct CWorldMap {
    const CGameDefinitionManager& PeekDefinitionManager() const { ++g_peeks; return g_defs; }
    bool IsPosPassable(const C2DCoordI& pos) const { return !(pos.X == 11 || pos.Y == 19); }   // column x=11 and row y=19 blocked
    void GetImpassabilityFactorAround(const C3DVector& pos, CFloatVector& factors, bool unused) const;
};

void CWorldMap::GetImpassabilityFactorAround(const C3DVector& pos, CFloatVector& factors, bool unused) const
{
    (void)unused;
    PeekDefinitionManager();
    PeekDefinitionManager();
    factors.clear();
    C2DCoordI centre((long)pos.X, (long)pos.Y);
    for (long i = 0; i < 9; ++i)
    {
        factors[i] = 0.0f;
        if (!IsPosPassable(C2DCoordI(g_BlockNeighbourOffsets[i].X + centre.X, g_BlockNeighbourOffsets[i].Y + centre.Y)))
            factors[i] = 1.0f;
    }
}

int main()
{
    float storage[9]; for (int k = 0; k < 9; ++k) storage[k] = 5.0f;
    CFloatVector v; v._M_start = storage; v._M_finish = storage + 3; v._M_end_of_storage = storage + 9;
    CWorldMap m; C3DVector p = { 10.7f, 20.2f, 0.0f };   // centre (10, 20)
    m.GetImpassabilityFactorAround(p, v, false);
    // offsets -> cells: (9,19)B (10,19)B (11,19)B (11,20)B (11,21)B (10,21) (9,21) (9,20) (10,20)
    const float expect[9] = { 1, 1, 1, 1, 1, 0, 0, 0, 0 };
    bool ok = v._M_finish == v._M_start && g_peeks == 2;
    for (int k = 0; k < 9; ++k) if (storage[k] != expect[k]) ok = false;
    if (!ok) { std::printf("0063e3c0_TEST FAIL\n"); return 1; }
    std::printf("0063e3c0_TEST PASS\n");
    return 0;
}
