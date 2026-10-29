// CWorldMap::SetPlayerPos(long, const C3DVector&, ELoadType) -- place a player: with region loading on and the destination map not loaded, unload the player's previous region (if different) and load the new region (or just the level when it has none); record the position and cache the maps around it
// Retail: Fable.exe @ 0x005063E0, 196 bytes, `ret 0Ch`. PDB ?SetPlayerPos@CWorldMap@@QAEXJABVC3DVector@@W4ELoadType@1@@Z
// Callees: CWorldMap virtual slots 16 GetMapNumberAt(const C3DVector&) const (+0x40, 0x004FC0C0), 10 IsMapLoaded(long)
// const (+0x28, 0x004FC050); GetRegionNumberMapIsIn(long) const 0x004FC190, UnloadRegion(long, bool) 0x004FEEC0,
// LoadRegion(long, ELoadType, bool) 0x00500540, LoadLevel(long, ELoadType) 0x00502620, CacheMaps(ELoadType) 0x00502850.
// Global bool 0x013756F6 (region loading enabled; name descriptive, same global as StartMovePlayerPos 0x00502500).
// Retail offsets: CWorldMap PlayerPositionInfo (vector<CPlayerPositionInfo>, 13-byte packed elements {bool Set;
// C3DVector Pos;}) +0x78 (PDB +0x8C).
// Shape: STLport operator[] is `*(begin() + n)`; the previous position's Set test and Pos address share one index
// computation; the two final stores index the vector separately (its start is re-read after the byte store).
class C3DVector { public: float X, Y, Z; };
class C2DCoordI;

#pragma pack(push, 1)
struct CPlayerPositionInfo {                                 // 13 bytes (PDB)
    bool      Set;                                           // +0x00
    C3DVector Pos;                                           // +0x01
};
#pragma pack(pop)
template <class T> struct CVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

enum ELoadType { LOAD_TYPE_DEFAULT = 0 };
extern bool g_bRegionLoadingEnabled;                         // 0x013756F6

class CWorldMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual bool IsMapLoaded(long map) const;                          // slot 10 (+0x28) 0x004FC050
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAt(const C2DCoordI& pos) const;           // slot 17 (+0x44) 0x004FC080
    virtual long GetMapNumberAt(const C3DVector& pos) const;           // slot 16 (+0x40) 0x004FC0C0 (reverse order)
    unsigned char               _pad04[0x74];
    CVector<CPlayerPositionInfo> PlayerPositionInfo;                   // +0x78 (PDB +0x8C)

    long GetRegionNumberMapIsIn(long map) const;                       protected: // 0x004FC190
    void UnloadRegion(long region, bool b); public:                            // 0x004FEEC0
    void LoadRegion(long region, ELoadType loadType, bool b);          // 0x00500540
    void LoadLevel(long map, ELoadType loadType);                      // 0x00502620
    void CacheMaps(ELoadType loadType);                                // 0x00502850
    void SetPlayerPos(long player, const C3DVector& pos, ELoadType loadType);
};

void CWorldMap::SetPlayerPos(long player, const C3DVector& pos, ELoadType loadType)
{
    if (g_bRegionLoadingEnabled)
    {
        long newMap = GetMapNumberAt(pos);
        if (!IsMapLoaded(newMap))
        {
            long newRegion = GetRegionNumberMapIsIn(newMap);
            if (PlayerPositionInfo[player].Set)
            {
                long oldRegion = GetRegionNumberMapIsIn(GetMapNumberAt(PlayerPositionInfo[player].Pos));
                if (oldRegion > 0 && oldRegion != newRegion)
                    UnloadRegion(oldRegion, true);
            }
            if (newRegion > 0)
                LoadRegion(newRegion, loadType, true);
            else if (newMap > 0)
                LoadLevel(newMap, loadType);
        }
    }
    PlayerPositionInfo[player].Set = true;
    PlayerPositionInfo[player].Pos = pos;
    CacheMaps(loadType);
}