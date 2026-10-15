// CTCHeroStats::AddHousePosition(const CRegion*, const C3DVector&) -- remembers a house position for a region
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057CB4B, 51 bytes. Single caller 0x006C1871.
// Callee: vector<pair<const CRegion*, C3DVector>>::push_back(const value_type&) @ 0x00581132.
//
// Shape: a frame-pointer size TU (#pragma optimize("y", off) + optimize("s", on): `push ebp ; mov ebp,esp ; sub esp,10h
// ... leave`). The 16-byte record is a pair temporary built in the frame from the region and a copy of the position
// through eax/edx (the y store scheduled after the push) -- C3DVector's memberwise user copy constructor; the implicit
// copy is a `movsd` block through esi/edi. Its address is pushed and the vector addressed with `add ecx,160h` (`this`
// reused as the vector pointer).
// Layout: retail HousePositions +0x160 (PDB +0x174); see RemoveHousePosition (0x0057ADF7).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CRegion;

struct C3DVector {
    float x, y, z;
    C3DVector(const C3DVector& other) : x(other.x), y(other.y), z(other.z) {}
};

struct CHousePosition {                        // std::pair<const CRegion*, C3DVector>
    const CRegion* first;
    C3DVector      second;
    CHousePosition(const CRegion* const& region, const C3DVector& position) : first(region), second(position) {}
};

struct CHousePositionVector {                  // retail std::vector: first, last, end
    CHousePosition* m_pFirst;
    CHousePosition* m_pLast;
    CHousePosition* m_pEnd;
    void push_back(const CHousePosition& value);   // 0x00581132
};

struct CTCHeroStats_Retail {
    unsigned char        _pad_0x00[0x160];
    CHousePositionVector HousePositions;       // +0x160 (PDB +0x174)

    void AddHousePosition(const CRegion* region, const C3DVector& position);
};

void CTCHeroStats_Retail::AddHousePosition(const CRegion* region, const C3DVector& position)
{
    HousePositions.push_back(CHousePosition(region, position));
}