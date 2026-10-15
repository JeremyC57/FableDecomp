#include <cstdio>
// Behaviour of CTCHeroStats::RemoveHousePosition (0x0057ADF7): erases the first record of the region lying less than one
// unit away in XY (Z ignored); other regions and far records stay.
struct CRegion { int id; };
struct C3DVector { float x, y, z; };
struct CHousePosition { const CRegion* first; C3DVector second; };
static CHousePosition* g_erased = 0;
struct CHousePositionVector {
    CHousePosition* m_pFirst; CHousePosition* m_pLast; CHousePosition* m_pEnd;
    CHousePosition* begin() { return m_pFirst; }
    CHousePosition* end() { return m_pLast; }
    CHousePosition* erase(CHousePosition* where)
    {
        g_erased = where;
        for (CHousePosition* p = where; p + 1 != m_pLast; ++p) *p = *(p + 1);
        --m_pLast;
        return where;
    }
};
struct CTCHeroStats_Retail {
    CHousePositionVector HousePositions;
    void RemoveHousePosition(const CRegion* region, const C3DVector& position)
    {
        for (CHousePosition* it = HousePositions.begin(); it != HousePositions.end(); ++it) {
            if (it->first == region) {
                float dx = it->second.x - position.x;
                float dy = it->second.y - position.y;
                float distanceSquared = dx * dx;
                distanceSquared += dy * dy;
                if (distanceSquared < 1.0f) {
                    HousePositions.erase(it);
                    return;
                }
            }
        }
    }
};
static CHousePosition Make(const CRegion* r, float x, float y, float z)
{
    CHousePosition h; h.first = r; h.second.x = x; h.second.y = y; h.second.z = z; return h;
}
int main()
{
    CRegion oakvale, bowerstone;
    CHousePosition houses[4];
    houses[0] = Make(&bowerstone, 0.0f, 0.0f, 0.0f);
    houses[1] = Make(&oakvale, 10.0f, 10.0f, 0.0f);
    houses[2] = Make(&oakvale, 0.5f, 0.5f, 50.0f);
    houses[3] = Make(&oakvale, 0.25f, 0.0f, 0.0f);
    CTCHeroStats_Retail s;
    s.HousePositions.m_pFirst = houses; s.HousePositions.m_pLast = houses + 4; s.HousePositions.m_pEnd = houses + 4;
    C3DVector at; at.x = 0.0f; at.y = 0.0f; at.z = 0.0f;
    s.RemoveHousePosition(&oakvale, at);
    if (g_erased != houses + 2 || s.HousePositions.m_pLast != houses + 3 || houses[2].second.x != 0.25f) { std::printf("0057adf7_TEST FAIL\n"); return 1; }
    C3DVector far; far.x = 5.0f; far.y = 5.0f; far.z = 0.0f; g_erased = 0;
    s.RemoveHousePosition(&oakvale, far);
    if (g_erased != 0 || s.HousePositions.m_pLast != houses + 3) { std::printf("0057adf7_TEST FAIL\n"); return 1; }
    std::printf("0057adf7_TEST PASS\n");
    return 0;
}