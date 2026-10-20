// CWorldMap::AddLevelFromFile(const C2DCoordI&, const CCharString&) -- take the first unused map slot (index 1 up, else grow the map array by one), mark it used with the level name, place the level's world box at the position, give it a map UID and register it with the engine; returns the map index
// Leaf: AddLevelFromFile
// Retail: Fable.exe (TLC, VC7.1) @ 0x00506F30, 314 bytes (320 with padding), `ret 8`.
//   PDB ?AddLevelFromFile@CWorldMap@@QAEJABVC2DCoordI@@ABVCCharString@@@Z.
// Callees: CArray<CMapInfo>::resize 0x0051BE20 (label "GetAnimsWithNameStartingWith" is wrong, GOTCHAS chunk 173/179),
//   CCharString::operator=(const CCharString&) 0x0099EFB0, CWorldMap::GetLevelDimensions(const CCharString&) 0x00506E30
//   (C2DBoxI by hidden pointer), AllocateMapUID 0x004FCE20, CTBaseSingleton<CEngineManager>::Get 0x00436FB0 (label
//   "CGameJoystickManager" is wrong), CEngineManager::AddMap 0x006403A0, LoadLoadableGlobalThingsForLevel 0x004FBF60.
// Layout: CWorldMap Maps +0x20 (12-byte retail CArray, 0x48-byte CMapInfo: Area +0, LevelName +0x18, MapUID +0x20,
//   Used +0x24, Placed +0x25).
// Shape: a null-initialised slot pointer set inside the search loop (VC drops the test on the loop-exhausted path),
//   size() re-evaluated each iteration (`imul 38E38E39h ; sar 4` = /0x48); the box is a struct assignment, then
//   translated so its top-left sits at the position.
struct CCharString {
    void* PStringData;
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};
struct C2DCoordI { long X, Y; };
struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct CMap;
struct CMapInfo {                                      // 0x48 bytes (Ego_r.pdb = retail)
    C2DBoxI       Area;                                // +0x00
    CMap*         PMap_Data;                           // +0x10 CCountedPointer<CMap>
    void*         PMap_Info;                           // +0x14
    CCharString   LevelName;                           // +0x18
    CCharString   LevelScriptName;                     // +0x1C
    unsigned long MapUID;                              // +0x20
    bool          Used;                                // +0x24
    bool          Placed;                              // +0x25
    bool          Activated;                           // +0x26
    bool          Loading;                             // +0x27
    unsigned char _body[0x20];
};
template <class T> struct CArray {                     // retail 12-byte STLport-style vector
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int i) { return *(_M_start + i); }
    void resize(unsigned int n);                       // 0x0051BE20
};
struct CEngineManager {
    static CEngineManager& __fastcall Get();           // 0x00436FB0 CTBaseSingleton<CEngineManager>::Get
    void AddMap(long map);                             // 0x006403A0
};
class CWorldMap {
public:
    unsigned char  _pad00[0x20];
    CArray<CMapInfo> Maps;                             // +0x20
    unsigned long AllocateMapUID(long map);            // 0x004FCE20
    void LoadLoadableGlobalThingsForLevel(long map);   // 0x004FBF60
    C2DBoxI GetLevelDimensions(const CCharString& levelName);   // 0x00506E30
    long AddLevelFromFile(const C2DCoordI& position, const CCharString& levelName);
};
long CWorldMap::AddLevelFromFile(const C2DCoordI& position, const CCharString& levelName)
{
    CMapInfo* info = 0;
    unsigned long map;
    for (map = 1; map < Maps.size(); ++map) {
        if (!Maps[map].Used) {
            info = &Maps[map];
            break;
        }
    }
    if (!info) {
        Maps.resize(Maps.size() + 1);
        map = Maps.size() - 1;
        info = &Maps[map];
    }
    info->Used = true;
    info->LevelName = levelName;
    info->Placed = false;
    info->Area = GetLevelDimensions(levelName);
    info->Area.BRX += position.X - info->Area.TLX;
    info->Area.BRY += position.Y - info->Area.TLY;
    info->Area.TLX = position.X;
    info->Area.TLY = position.Y;
    info->MapUID = AllocateMapUID(map);
    CEngineManager::Get().AddMap(map);
    LoadLoadableGlobalThingsForLevel(map);
    return map;
}
