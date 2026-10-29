// CWorldMap::StartMovePlayerPos(long, const C3DVector&, ELoadType) -- a player is about to move to a new position: with region loading on, swap the loaded region when the move crosses regions; otherwise unload everything except the player's maps and force-load the destination map; then record the new position
// Retail: Fable.exe @ 0x00502500, 172 bytes, `ret 0Ch`. PDB ?StartMovePlayerPos@CWorldMap@@QAEXJABVC3DVector@@W4ELoadType@1@@Z
// Callees: CWorldMap virtual slots 16 GetMapNumberAt(const C3DVector&) const (+0x40, 0x004FC0C0), 10 IsMapLoaded(long)
// const (+0x28, 0x004FC050), 8 ForceLoad(long) (+0x20, 0x005063D0); GetRegionNumberMapIsIn(long) const 0x004FC190,
// UnloadRegion(long, bool) 0x004FEEC0, LoadRegion(long, ELoadType, bool) 0x00500540, UnloadAllMapsExceptPlayerPos()
// 0x004FF390. Global bool 0x013756F6 (region loading enabled; name descriptive).
// Retail offsets: CWorldMap PlayerPositionInfo (vector<CPlayerPositionInfo>, 13-byte packed elements {bool Set;
// C3DVector Pos;}) +0x78 (PDB +0x8C).
// Shape: STLport operator[] is `*(begin() + n)` (the plain `_M_start[n]` swaps eax/ecx in the first index); the old
// player map's region is computed before the new one; the two final stores index the vector separately
// (its start is re-read after the byte store).
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
    virtual void ForceLoad(long map);                                  // slot 8 (+0x20) 0x005063D0
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
    void UnloadAllMapsExceptPlayerPos();                               // 0x004FF390
    void StartMovePlayerPos(long player, const C3DVector& pos, ELoadType loadType);
};

void CWorldMap::StartMovePlayerPos(long player, const C3DVector& pos, ELoadType loadType)
{
    long newMap = GetMapNumberAt(pos);
    if (g_bRegionLoadingEnabled)
    {
        long oldRegion = GetRegionNumberMapIsIn(GetMapNumberAt(PlayerPositionInfo[player].Pos));
        long newRegion = GetRegionNumberMapIsIn(newMap);
        if (oldRegion != newRegion)
        {
            UnloadRegion(oldRegion, true);
            LoadRegion(newRegion, loadType, true);
        }
    }
    else
    {
        UnloadAllMapsExceptPlayerPos();
        if (!IsMapLoaded(newMap))
            ForceLoad(newMap);
    }
    PlayerPositionInfo[player].Set = true;
    PlayerPositionInfo[player].Pos = pos;
}