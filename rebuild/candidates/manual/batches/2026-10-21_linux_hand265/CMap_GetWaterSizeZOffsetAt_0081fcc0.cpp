// CMap::GetWaterSizeZOffsetAt(const C2DCoordI& pos) -- the water height at a map cell: 0 unless HasWaterAt, else the blend-weighted sum of the water heights of the (up to three) engine themes painted there (blends +7/+8, the third weight 255 minus both, all scaled by 1/255)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081FCC0, length 512 bytes (`ret 4`).
// Callees: CMap::HasWaterAt @ 0x0081FB00, CDefinitionManager::GetDef<CEngineThemeDef> @ 0x0063F3A0 (manifest label
//   GetDef<CThingPhysicalSwitchDef>, a byte-twin), the def's virtual slot 1 (self-destroy) on the last reference. Floats: 0x0122DEDC 0.0f,
//   0x01231724 1/255.
// Layout: as the sibling HasWaterAt 0x0081FB00 (CMap DefinitionManager +0x1c, palette +0x1a6c, height map +0x1a7c); CEngineThemeDef
//   WaterHeight is retail +0x6c (PDB +0x70).
// Shape: the coordinates copied into locals, nested scopes with one raw def pointer per layer released by hand on the way out. GetDef is a same-TU stand-in that only writes its out slot.
struct C2DCoordI {
    long X, Y;
};
struct CHeightMapCell {                            // PDB, 12 bytes
    float         Height;                          // +0x00
    unsigned char EngineThemePaletteIndex[3];      // +0x04
    unsigned char EngineBlend[2];                  // +0x07
    unsigned char Flags;                           // +0x09
    unsigned char _pad0a[2];
};
template <class T> struct CRetailVector {          // STLport vector: begin / end / end of storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CHeightMap {                                // retail layout
    unsigned char                  _pad00[0x10];
    CRetailVector<CHeightMapCell>  Cells;          // +0x10
    unsigned char                  _pad1c[0x1e8];
    long                           SizeX;          // +0x204
    long                           SizeY;          // +0x208
    __forceinline CHeightMapCell& GetCellIncludingExtraCells(long x, long y) { return Cells[(SizeY - y) * (SizeX + 1) + x]; }
};
struct CEngineThemePaletteEntry {                  // 8 bytes
    long DefIndex;
    long Count;
};
struct CDefPointeeBase {
    virtual void v00();
    virtual void DestroySelf();
    long RefCount;
};
struct CEngineThemeDef : CDefPointeeBase {
    unsigned char _pad08[0x6c - 8];
    float         WaterHeight;                     // +0x6c (retail)
    long          WaterType;                       // +0x70 (retail)
};
struct CDefinitionManager {
    bool GetDef(long index, CEngineThemeDef** out);   // 0x0063F3A0
};
void* FindDef_CDefinitionManager_GetDef_0();       // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CEngineThemeDef** out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CMap {
    unsigned char                            _pad00[0x1c];
    CDefinitionManager*                      DefinitionManager;   // +0x1c
    unsigned char                            _pad20[0x1a6c - 0x20];
    CRetailVector<CEngineThemePaletteEntry>  EngineThemePalette;  // +0x1a6c
    unsigned char                            _pad1a78[4];
    CHeightMap*                              HeightMap;           // +0x1a7c
    bool HasWaterAt(const C2DCoordI& pos);       // 0x0081FB00
    float GetWaterSizeZOffsetAt(const C2DCoordI& pos);
};

float CMap::GetWaterSizeZOffsetAt(const C2DCoordI& pos)
{
    if (!HasWaterAt(pos))
        return 0.0f;
    long x = pos.X;
    long y = pos.Y;
    float height = 0.0f;
    long index0 = EngineThemePalette[HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[0]].DefIndex;
    if (index0 > 0) {
        CEngineThemeDef* def0 = 0;
        DefinitionManager->GetDef(index0, &def0);
        unsigned char blend0 = HeightMap->GetCellIncludingExtraCells(x, y).EngineBlend[0];
        height = blend0 * def0->WaterHeight * (1.0f / 255.0f);
        long index1 = EngineThemePalette[HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[1]].DefIndex;
        if (index1 > 0) {
            CEngineThemeDef* def1 = 0;
            DefinitionManager->GetDef(index1, &def1);
            unsigned char blend1 = HeightMap->GetCellIncludingExtraCells(x, y).EngineBlend[1];
            height += blend1 * def1->WaterHeight * (1.0f / 255.0f);
            long index2 = EngineThemePalette[HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[2]].DefIndex;
            if (index2 > 0) {
                CEngineThemeDef* def2 = 0;
                DefinitionManager->GetDef(index2, &def2);
                unsigned char blend2 = (unsigned char)(255 - blend1 - blend0);
                height += blend2 * def2->WaterHeight * (1.0f / 255.0f);
                if (--def2->RefCount == 0)
                    def2->DestroySelf();
            }
            if (--def1->RefCount == 0)
                def1->DestroySelf();
        }
        if (--def0->RefCount == 0)
            def0->DestroySelf();
    }
    return height;
}
