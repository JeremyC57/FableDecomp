// CWorld::UpdateEnvironmentCameraPos(const C3DVector& pos) -- bilinear blend of the environment themes of the four 4-unit map cells around the camera position: each cell's three atmos themes are weighted by its blend byte and the corner weight, and the camera theme stack is set when any weight is meaningful
// Leaf: ?UpdateEnvironmentCameraPos@CWorld@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049E370, 542 bytes, `ret 4` (FableWin.pdb ?UpdateEnvironmentCameraPos@CWorld@@IAEXABVC3DVector@@@Z).
// Callees: CWorld::PeekWorldMap (slot 13), CWorldMap::GetPGameMapCellAt 0x0063D8A0, _ftol 0x00BFEA70,
//   CEnvironment::GetGlobalEnvironmentThemeIndexFromLocal 0x006B2C60, CGameMapCell::GetAtmosBlend 0x006B74E0,
//   CEnvironment theme-stack setter 0x006B3E60 (ret 0x14: two zero words, theme indices, weights, count).
// Layout: as the landed sibling CWorld::GetEnvironmentFishWeightMult 0x0049F2D0 (PEnvironment +0x18, RegionTheme +0x10).
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CGameMapCell {
    unsigned char AtmosThemes[3];                  // +0x00
    unsigned char AtmosBlends[2];                  // +0x03
    unsigned char GetAtmosBlend(long index) const; // 0x006B74E0
};
struct CWorldMap {
    const CGameMapCell* GetPGameMapCellAt(const C3DVector& pos) const;   // 0x0063D8A0
};
struct CEnvironment {
    void* __vftable;
    void* DefinitionManager;                       // +0x04
    unsigned char _pad08[8];
    long  RegionTheme;                             // +0x10
    long GetGlobalEnvironmentThemeIndexFromLocal(long localIndex);   // 0x006B2C60
    void SetCameraThemes(long a, long b, const long* themes, const float* weights, long count);   // 0x006B3E60
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual const CWorldMap* PeekWorldMap() const;   // slot 13 (+0x34)
    unsigned char _pad04[0x14];
    CEnvironment* PEnvironment;                    // +0x18 (scoped_ptr)
    void UpdateEnvironmentCameraPos(const C3DVector& pos);
};
void CWorld::UpdateEnvironmentCameraPos(const C3DVector& pos)
{
    const CWorldMap* map = PeekWorldMap();
    const CGameMapCell* cells[4];
    cells[0] = map->GetPGameMapCellAt(C3DVector(pos.X, pos.Y, 0.0f));
    cells[1] = map->GetPGameMapCellAt(C3DVector(pos.X + 4.0f, pos.Y, 0.0f));
    cells[2] = map->GetPGameMapCellAt(C3DVector(pos.X, pos.Y + 4.0f, 0.0f));
    cells[3] = map->GetPGameMapCellAt(C3DVector(pos.X + 4.0f, pos.Y + 4.0f, 0.0f));
    float fx = (pos.X - (float)(long)(pos.X * 0.25f) * 4.0f) * 0.25f;
    float fy = (pos.Y - (float)(long)(pos.Y * 0.25f) * 4.0f) * 0.25f;
    long themes[12];
    float weights[12];
    long count = 0;
    float total = 0.0f;
    for (long corner = 0; corner < 4; ++corner) {
        float weight = 0.0f;
        if (corner == 0)
            weight = (1.0f - fy) * (1.0f - fx);
        else if (corner == 1)
            weight = (1.0f - fy) * fx;
        else if (corner == 2)
            weight = (1.0f - fx) * fy;
        else if (corner == 3)
            weight = fy * fx;
        const CGameMapCell* cell = cells[corner];
        if (cell) {
            for (long i = 0; i != 3; ++i) {
                long theme = cell->AtmosThemes[i];
                if (theme == 0)
                    themes[count] = PEnvironment->RegionTheme;
                else
                    themes[count] = PEnvironment->GetGlobalEnvironmentThemeIndexFromLocal(theme);
                float w = (float)cell->GetAtmosBlend(i) * weight * (1.0f / 255.0f);
                weights[count++] = w;
                total += w;
            }
        }
    }
    if (total > 0.00390625f)
        PEnvironment->SetCameraThemes(0, 0, themes, weights, count);
}