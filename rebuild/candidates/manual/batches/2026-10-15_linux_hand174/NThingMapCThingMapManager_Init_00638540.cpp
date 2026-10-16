// NThingMap::CThingMapManager::Init(CWorld* world, long unused) -- resets the mapwho level table, binds the world, clears the game maps and sizes the 128-unit cell grid to the world map's bounds
// Leaf: ?Init@CThingMapManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00638540, length 114 bytes (`ret 8`; the second argument is never read).
// Callees: the static mapwho level-table initialiser @ 0x006379D0 (manifest "InitLayerInfo"; writes the level globals 0x013BA848/0x013BA84C),
// the manager's game-map clear @ 0x006380C0 (walks the 8-byte cells at +8; manifest "CShaderResource::CloseShaderBank" is a
// propagated label), CWorld vtable slot 12 (+0x30, GetWorldMap), CWorldMap vtable slot 13 (+0x34, the world bounds as const C2DBoxI&),
// the cell vector's resize(n) @ 0x00639810 (manifest "vector<pair<CCharString,long>>::resize" propagated).
// Layout (retail, local model): CThingMapManager PWorld +0x04, game-map cell vector (8-byte cells) +0x08, grid Width +0x30 / Height +0x34
// (PDB Dimensions C2DExtentsI at +0x3c; retail containers narrower).
// Shape: GetWorldMap()->GetBounds() is called once per extent with the inline box GetWidth()/GetHeight() (box in ecx);
// `/ 128` signed; Height is stored after the
// resize argument push (Width re-read from the member).
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    long GetWidth() const { return BRX - TLX; }
    long GetHeight() const { return BRY - TLY; }
};
struct CWorldMap {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012();
    virtual const C2DBoxI& GetBounds() const;   // slot 13 (+0x34)
};
struct CWorld {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual CWorldMap* GetWorldMap();           // slot 12 (+0x30)
};
namespace NThingMap {
struct SGameMapCell { void* PMap; long Count; };
struct CGameMapCellVector {                     // STLport vector<SGameMapCell>
    SGameMapCell* _M_start;
    SGameMapCell* _M_finish;
    SGameMapCell* _M_end_of_storage;
    void resize(unsigned int n);                // 0x00639810
};
struct CThingMapManager {
    void*              __vftable;               // +0x00
    CWorld*            PWorld;                  // +0x04
    CGameMapCellVector Cells;                   // +0x08
    unsigned char      _pad14[0x30 - 0x14];
    long               Width;                   // +0x30
    long               Height;                  // +0x34
    static void InitLayerInfo();                // 0x006379D0
    void ClearGameMaps();                       // 0x006380C0
    void Init(CWorld* world, long unused);
};

void CThingMapManager::Init(CWorld* world, long unused)
{
    InitLayerInfo();
    PWorld = world;
    ClearGameMaps();
    Width = world->GetWorldMap()->GetBounds().GetWidth() / 128;
    Height = world->GetWorldMap()->GetBounds().GetHeight() / 128;
    Cells.resize(Width * Height);
}
}
