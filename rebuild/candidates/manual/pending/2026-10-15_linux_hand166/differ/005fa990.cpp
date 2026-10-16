// CTCInventoryMap::GetGuildSealRecallRegion() const -- the region number holding the world's guild-seal recall position, or 0 while no recall position is set
// Grade: DIFFER(112v111) -- x87 load order only: retail loads the copied position Z, Y, X (`fld [edi+15Ch]` before `add edi,154h`, X last from [edi]), ours X, Y, Z with identical squaring/compare code (tried: memberwise copy ctor, 3-float ctor from the members, reversed term order) -- lane gap: x87 sum-of-squares order
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FA990, length 111 bytes.
// Callees: CTCBase::GetWorld() const @ 0x00686A80 (manifest "CViewBase::GetDisplayEngine", propagated), CWorld virtual slot 13
// (+0x34, PeekWorldMap), CWorldMap virtual slot 16 (+0x40, the map number at a position), CWorldMap::GetRegionNumberMapIsIn(long)
// @ 0x004FC190. Float: the 1e-4 epsilon @ 0x0129BA3C (squared at run time).
// Shape: the position is copied by value onto the FPU (z, y, x loads) for the inline squared-magnitude test; the map lookups
// take the world member by reference (`push edi`, edi = world + 0x154).
// Layout: CWorld guild-seal recall position at +0x154 (retail offset, local model).
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float GetMagnitudeSquared() const { return X * X + Y * Y + Z * Z; }
};
extern const float g_fPositionEpsilon;          // 0x0129BA3C (1e-4)

struct CWorldMap {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual long GetMapNumberAtPosition(const C3DVector& position) const;   // slot 16 (+0x40)
    long GetRegionNumberMapIsIn(long map) const;                          // 0x004FC190
};
struct CWorld {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual const CWorldMap* PeekWorldMap() const;   // slot 13 (+0x34)
    unsigned char _pad_0x04[0x150];
    C3DVector GuildSealRecallPosition;               // +0x154
};

struct CTCInventoryMap {
    const CWorld* GetWorld() const;                  // 0x00686A80 (CTCBase)
    long GetGuildSealRecallRegion() const;
};

long CTCInventoryMap::GetGuildSealRecallRegion() const
{
    const C3DVector& position = GetWorld()->GuildSealRecallPosition;
    C3DVector recall(position.X, position.Y, position.Z);
    if (recall.GetMagnitudeSquared() > g_fPositionEpsilon * g_fPositionEpsilon) {
        const CWorldMap* worldMap = GetWorld()->PeekWorldMap();
        return worldMap->GetRegionNumberMapIsIn(worldMap->GetMapNumberAtPosition(position));
    }
    return 0;
}
