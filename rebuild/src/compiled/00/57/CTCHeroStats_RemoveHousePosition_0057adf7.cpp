// CTCHeroStats::RemoveHousePosition(const CRegion*, const C3DVector&) -- forgets the remembered house of this region that lies within one unit (2D) of the position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057ADF7, 92 bytes. Single caller 0x006C18F6.
// Callee: vector<pair<const CRegion*, C3DVector>>::erase(iterator) @ 0x0057FD32 (`this` = the vector, kept in ecx
//         through the loop; the returned iterator is dropped).
// Globals: 1.0f @ 0x0122DED8.
//
// Shape: the vector addressed once (`mov eax,ecx ; lea ecx,[eax+160h]`), first and last loaded before the empty test
// (last cached in esi: nothing in the loop writes memory), the position argument fetched only once the vector is
// non-empty. The 16-byte records are matched on the region pointer, then on the squared XY distance: both
// differences go into named float locals first (`fld x ; fsub ; fld y ; fsub`), squared and summed with the X square
// first (`fld st(1) ; fmul st(2)` twice, `faddp`: `d = dx*dx ; d += dy*dy` -- the one-expression `dx*dx + dy*dy`
// squares the top of stack, dy, first), compared `< 1.0f` (`test ah,5 ; jnp`), and both locals popped. A hit
// erases the record and returns.
// Layout: retail HousePositions is +0x160 (PDB +0x174: the map, three vectors and nothing else ahead are 4 bytes
// smaller each in retail -- 0x14 lower here); record = pair<const CRegion*, C3DVector> (0x10 bytes).
#pragma optimize("s", on)

struct CRegion;

struct C3DVector {
    float x, y, z;
};

struct CHousePosition {                        // std::pair<const CRegion*, C3DVector>
    const CRegion* first;
    C3DVector      second;
};

struct CHousePositionVector {                  // retail std::vector: first, last, end
    CHousePosition* m_pFirst;
    CHousePosition* m_pLast;
    CHousePosition* m_pEnd;
    CHousePosition* begin() { return m_pFirst; }
    CHousePosition* end() { return m_pLast; }
    CHousePosition* erase(CHousePosition* where);   // 0x0057FD32
};

struct CTCHeroStats_Retail {
    unsigned char        _pad_0x00[0x160];
    CHousePositionVector HousePositions;       // +0x160 (PDB +0x174)

    void RemoveHousePosition(const CRegion* region, const C3DVector& position);
};

void CTCHeroStats_Retail::RemoveHousePosition(const CRegion* region, const C3DVector& position)
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