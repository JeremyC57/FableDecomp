// CTCHeroStats::SetHeroKickPositionForCurrentRegion(const C3DVector&, float, bool) -- records where (and how) the hero left the current region in its mirrored criminal status
// Leaf: SetHeroKickPositionForCurrentRegion
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057D990, 109 bytes; size TU without a frame pointer here
// (`#pragma optimize("s", on)`; `ret 0Ch`).
// Callees: CTCBase::GetWorld() @ 0x00686A70 (twice), CTCBase::GetWorldMap() @ 0x00686D20,
//          CWorldMap::GetCurrentRegionNumber() @ 0x004FB150, CWorld vtable slot 12 (+0x30) GetWorldMap(),
//          CWorldMap::GetRegion(long) @ 0x004FB490 (0x58-byte CRegion records; manifest label
//          "CTCInventoryBase::GetCategoryWithIndex" is propagated), map<CCharString, MirroredCriminalStatus>::operator[]
//          @ 0x005812DF on OutstandingCrimes (+0x0C), CWorld::GetWorldFrame() @ 0x0049D870 (static, after a kept
//          GetWorld() call).
// Shape: the world pointer is a named local taken first (esi) and the region index is the virtual chain's argument;
// the key is the region's name CCharString (+0x18). The status record is filled in member order: entrance
// position (three dword copies), entrance angle, the teleporter flag, then LastTimeExited from the world frame.
// Layout: MirroredCriminalStatus (PDB, 0x44 bytes): Crimes[10] +0x00, LastTimeSeenByGuard +0x28, LastTimeExited +0x2C,
// ExitedUsingTeleporter +0x30, EntrancePos +0x34, EntranceAngle +0x40.
#pragma optimize("s", on)

struct CCharStringData;

struct CCharString {
    CCharStringData* PStringData;
};

struct C3DVector {
    float X, Y, Z;
};

struct CRegion {
    unsigned char _pad_0x00[0x18];
    CCharString   Name;                              // +0x18
};

struct CWorldMap {
    long GetCurrentRegionNumber();                   // 0x004FB150
    CRegion& GetRegion_004fb490(long index);                  // 0x004FB490
};

struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();                // slot 12 (+0x30)
    static long GetWorldFrame();                     // 0x0049D870
};

struct MirroredCriminalStatus {
    long  Crimes[10];                                // +0x00
    long  LastTimeSeenByGuard;                       // +0x28
    long  LastTimeExited;                            // +0x2C
    bool  ExitedUsingTeleporter;                     // +0x30
    float EntrancePosX;                              // +0x34
    float EntrancePosY;                              // +0x38
    float EntrancePosZ;                              // +0x3C
    float EntranceAngle;                             // +0x40
};

struct CCrimeStatusMap {                             // std::map<CCharString, MirroredCriminalStatus> (retail 0x0C bytes)
    void* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare;
    MirroredCriminalStatus& operator[](const CCharString& key);  // 0x005812DF
};

struct CTCHeroStats {
    void*           __vftable;                       // +0x00
    void*           ParentThing;                     // +0x04 CTCBase
    unsigned char   _pad_0x08[0x4];
    CCrimeStatusMap OutstandingCrimes;               // +0x0C

    CWorld* GetWorld() const;                        // 0x00686A70
    CWorldMap* GetWorldMap() const;                  // 0x00686D20
    void SetHeroKickPositionForCurrentRegion(const C3DVector& position, float angle, bool teleporter);
};

void CTCHeroStats::SetHeroKickPositionForCurrentRegion(const C3DVector& position, float angle, bool teleporter)
{
    CWorld* world = GetWorld();
    MirroredCriminalStatus& status =
        OutstandingCrimes[world->GetWorldMap()->GetRegion_004fb490(GetWorldMap()->GetCurrentRegionNumber()).Name];
    status.EntrancePosX = position.X;
    status.EntrancePosY = position.Y;
    status.EntrancePosZ = position.Z;
    status.EntranceAngle = angle;
    status.ExitedUsingTeleporter = teleporter;
    status.LastTimeExited = GetWorld()->GetWorldFrame();
}