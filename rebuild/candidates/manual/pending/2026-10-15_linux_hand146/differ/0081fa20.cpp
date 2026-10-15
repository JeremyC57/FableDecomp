// CMap::GetThemeMinCameraSizeZOffsetAt(const C2DCoordI& pos) -- the dominant engine theme at a map cell; when it covers the cell fully (blend >= 255) the theme definition's float at +0x80 (PDB CEngineThemeDef Height, one dword lower in retail), else 0
// Grade: DIFFER(121v103) -- frame packing: retail homes ALL three locals (the blend out-value, the def out-pointer and the returned float) in the dead `pos` argument slot ([esp+8], no frame); ours homes only the def pointer there and opens an 8-byte frame (`sub esp,8`) for the blend and the float, and re-reads the def pointer after `dec [eax+4]` (retail keeps it in ecx)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081FA20, length 103 bytes (twin of 0x0081FA90 at the neighbouring float).
// Callees: CMap::GetDominantEngineThemeAt(const C2DCoordI&, long* blend) @ 0x0081F910 (the manifest's "CopyPos" label is wrong: it returns the
//   palette's theme def index and writes the blend), CDefinitionManager::GetDef<CEngineThemeDef> @ 0x0063F3A0 (manifest label GetDef<CThingPhysicalSwitchDef>,
//   a byte-twin), the def's virtual slot 1 (self-destroy) on the last reference. Floats: 0x01230014 = 255.0f, 0x0122DEDC = 0.0f.
// Layout: CMap DefinitionManager +0x1c (retail; PDB +0x28 less the debug list/vector words).
// Tried: an inline Release() member (load/dec/store of the count), the hand `if (--def->RefCount == 0)` (this source: `dec [eax+4]` then a
//   re-read), a copied register local for the def (worse), a CDefPointer object with an unchecked release destructor (same frame).
// next: lane gap: multi-local homing into a dead argument slot.
struct CWorld;
struct C2DCoordI {
    long X, Y;
};
struct CHeightMapCell {                            // PDB, 12 bytes
    float         Height;                          // +0x00
    unsigned char EngineThemePaletteIndex[3];      // +0x04
    unsigned char EngineBlend[2];                  // +0x07
    unsigned char Flags;                           // +0x09 Changed / Passable / ShorePoint / CameraPassable / NavPreferred bits
    unsigned char _pad0a[2];
};
template <class T> struct CRetailVector {          // STLport vector: begin / end / end of storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CHeightMapInit {                            // PDB, 12 bytes
    long           SizeX;
    long           SizeY;
    unsigned long* WorldSeed;
};
struct CMap;
struct CHeightMap {                                // retail layout (PDB CHeightMap with the 12-byte release vector)
    virtual ~CHeightMap();                         // slot 0: the scalar deleting destructor
    CWorld*                        World;          // +0x04
    CMap*                          Map;            // +0x08
    unsigned long*                 WorldSeed;      // +0x0c
    CRetailVector<CHeightMapCell>  Cells;          // +0x10
    unsigned char                  _MapUpdateAreas[0x1e8];   // +0x1c C2DBoxI[30] MapUpdateAreas, +0x1fc NoMapUpdateAreas, +0x200 Changed
    long                           SizeX;          // +0x204
    long                           SizeY;          // +0x208
    long                           SizeXIncludingExtraCells;   // +0x20c
    long                           SizeYIncludingExtraCells;   // +0x210
    CHeightMap(CWorld* world, CMap* map, const CHeightMapInit& init);   // 0x008F63C0
    __forceinline CHeightMapCell& GetCellIncludingExtraCells(long x, long y) { return Cells[(SizeY - y) * (SizeX + 1) + x]; }
};
template <class T> struct scoped_ptr {
    T* ptr;
    T* operator->() const { return ptr; }
    void reset(T* p) { T* old = ptr; ptr = p; delete old; }
};
struct CDefPointeeBase {
    virtual void v00();
    virtual void DestroySelf();
    long RefCount;
    void Release() { --RefCount; if (RefCount == 0) DestroySelf(); }
};
struct CEngineThemeDef : CDefPointeeBase {
    unsigned char _pad08[0x80 - 8];
    float Height;
    float MinCameraHeight;
};
struct CDefinitionManager {
    bool GetDef(long index, CEngineThemeDef** out);   // 0x0063F3A0
};
struct CMap {
    unsigned char       _pad00[0x1c];
    CDefinitionManager* DefinitionManager;           // +0x1c
    long GetDominantEngineThemeAt(const C2DCoordI& pos, long* blend);   // 0x0081F910
    float GetThemeMinCameraSizeZOffsetAt(const C2DCoordI& pos);
};
float CMap::GetThemeMinCameraSizeZOffsetAt(const C2DCoordI& pos)
{
    long blend;
    long themeIndex = GetDominantEngineThemeAt(pos, &blend);
    if (blend >= 255.0f) {
        CEngineThemeDef* def = 0;
        DefinitionManager->GetDef(themeIndex, &def);
        float value = def->Height;
        if (--def->RefCount == 0)
            def->DestroySelf();
        return value;
    }
    return 0.0f;
}
