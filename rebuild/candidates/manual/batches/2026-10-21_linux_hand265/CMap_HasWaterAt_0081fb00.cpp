// CMap::HasWaterAt(const C2DCoordI& pos) -- true if any of the (up to three) engine themes painted on the map cell is a water theme (WaterType not none/8); each theme def stays referenced until the function returns
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081FB00, length 448 bytes (`ret 4`).
// Callees: CDefinitionManager::GetDef<CEngineThemeDef> @ 0x0063F3A0 (manifest label GetDef<CThingPhysicalSwitchDef>, a byte-twin), the
//   def's virtual slot 1 (self-destroy) on the last reference.
// Layout: CMap DefinitionManager +0x1c, theme palette vector +0x1a6c (8-byte entries, def index first), height map +0x1a7c (cells
//   vector +0x10, SizeX +0x204, SizeY +0x208; 12-byte cells with the three palette indices at +4) -- as in the landed
//   CMap::GetThemeMinCameraSizeZOffsetAt 0x0081FA90 and CMap::GetEngineThemeAt 0x0081F090. CEngineThemeDef::WaterType is retail
//   +0x70 (PDB +0x74, one dword lower in retail like MinCameraHeight).
// Shape: nested scopes, one raw def pointer per theme layer released by hand (`--RefCount == 0` -> DestroySelf) on every exit;
//   GetDef is a same-TU stand-in that only writes its out slot.
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
    unsigned char _pad08[0x70 - 8];
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
    bool HasWaterAt(const C2DCoordI& pos);
};

bool CMap::HasWaterAt(const C2DCoordI& pos)
{
    long x = pos.X;
    long y = pos.Y;
    long index0 = EngineThemePalette[HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[0]].DefIndex;
    if (index0 > 0) {
        CEngineThemeDef* def0 = 0;
        DefinitionManager->GetDef(index0, &def0);
        if (def0->WaterType != 0 && def0->WaterType != 8) {
            if (--def0->RefCount == 0)
                def0->DestroySelf();
            return true;
        }
        long index1 = EngineThemePalette[HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[1]].DefIndex;
        if (index1 > 0) {
            CEngineThemeDef* def1 = 0;
            DefinitionManager->GetDef(index1, &def1);
            if (def1->WaterType != 0 && def1->WaterType != 8) {
                if (--def1->RefCount == 0)
                    def1->DestroySelf();
                if (--def0->RefCount == 0)
                    def0->DestroySelf();
                return true;
            }
            long index2 = EngineThemePalette[HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[2]].DefIndex;
            if (index2 > 0) {
                CEngineThemeDef* def2 = 0;
                DefinitionManager->GetDef(index2, &def2);
                bool water = def2->WaterType != 0 && def2->WaterType != 8;
                if (--def2->RefCount == 0)
                    def2->DestroySelf();
                if (--def1->RefCount == 0)
                    def1->DestroySelf();
                if (--def0->RefCount == 0)
                    def0->DestroySelf();
                return water;
            }
            if (--def1->RefCount == 0)
                def1->DestroySelf();
        }
        if (--def0->RefCount == 0)
            def0->DestroySelf();
    }
    return false;
}
