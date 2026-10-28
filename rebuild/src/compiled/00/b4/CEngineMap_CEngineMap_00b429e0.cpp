// CEngineMap::CEngineMap(const CIDrawMap*, long, long, long, long, long, CCharString, bool) -- constructor: empty name, file entry and file-block manager (whose block map allocates its header node), then the game map, world position (as floats and as 16-bit grid coordinates), map size, UID and static-map name; not yet in the visible-map list
// Leaf: ??0CEngineMap
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B429E0, 172 bytes (+ int3 padding), `ret 20h`. Mangled
//   ??0CEngineMap@@QAE@PBVCIDrawMap@@JJJJJVCCharString@@_N@Z (the trailing bool is not read here); built by `new` 0x68 bytes at
//   0x00B42D87.
// Callees: CCharString::CCharString() 0x0099E4B0, malloc 0x00BFEA0E (the block map's 0x24-byte header node),
//   CCharString::operator=(const CCharString&) 0x0099EFB0, ~CCharString 0x0099EAE0 (the by-value name, callee-destroyed).
// Layout (retail; Ego_r.pdb agrees up to FileBlockManager, whose map is 12 bytes in retail): LandscapeMap +0,
//   LocalDetailCacheMap +4, GameMap +8, MapWorldPosition +0xc, MapUID +0x18, StaticMapName +0x1c, StaticMapOpenState +0x20,
//   CameraMapBounds +0x24, AsyncFileEntry +0x3c, FileBlockManager +0x44 {AsyncBankEntry +0x44, Blocks +0x4c (header node,
//   count +0x50, comparator)}, VisibleMapListIterator +0x58, MapWidth +0x5c, MapHeight +0x5e, WorldPosX +0x60, WorldPosY
//   +0x62, IsInVisibleMapList +0x64.
// Shape: member objects first (the name's default ctor, the counted pointers' zeroing ctors, the STLport tree's header
//   allocation: alloc proxy zeroed, malloc, count 0, colour/parent/left/right), then body assignments with one zero register
//   in the order that reproduces retail's schedule: game map, position, UID, grid position, size, the two engine pointers,
//   the name, the visible flag.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CCharString { public:
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};

class C3DVector { public: float X, Y, Z; };
class C3DBoundingBox { public: C3DVector Min, Max; };

template <class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) { }
};

struct SFileBlockNode {                                // _Rb_tree_node<pair<const long, CFileBlock>> (0x24 bytes)
    unsigned char   _M_color;
    SFileBlockNode* _M_parent;
    SFileBlockNode* _M_left;
    SFileBlockNode* _M_right;
    unsigned char   _M_value[0x14];
};
struct CFileBlockTreeBase {
    SFileBlockNode* _M_header;
    CFileBlockTreeBase() : _M_header(0)
    {
        _M_header = (SFileBlockNode*)malloc(sizeof(SFileBlockNode));
    }
};
struct CFileBlockMap : CFileBlockTreeBase {            // map<long, CFileBlock> (retail 12 bytes)
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    CFileBlockMap() : _M_node_count(0)
    {
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};

class CBankFileAsyncEntry;
class CEngineStaticMapFileBlockManager { public:              // retail 0x14 bytes
    CCountedPointer<CBankFileAsyncEntry> AsyncBankEntry;   // +0x00
    CFileBlockMap                        Blocks;           // +0x08
};

class CEngineLandscapeMap;
class CLocalDetailCacheMap;
class CIDrawMap;

class CEngineMap { public:
    CEngineLandscapeMap*                 LandscapeMap;          // +0x00
    CLocalDetailCacheMap*                LocalDetailCacheMap;   // +0x04
    const CIDrawMap*                     GameMap;               // +0x08
    C3DVector                            MapWorldPosition;      // +0x0c
    long                                 MapUID;                // +0x18
    CCharString                          StaticMapName;         // +0x1c
    long                                 StaticMapOpenState;    // +0x20
    C3DBoundingBox                       CameraMapBounds;       // +0x24
    CCountedPointer<CBankFileAsyncEntry> AsyncFileEntry;        // +0x3c
    CEngineStaticMapFileBlockManager     FileBlockManager;      // +0x44
    void*                                VisibleMapListIterator;   // +0x58
    unsigned short                       MapWidth;              // +0x5c
    unsigned short                       MapHeight;             // +0x5e
    unsigned short                       WorldPosX;             // +0x60
    unsigned short                       WorldPosY;             // +0x62
    bool                                 IsInVisibleMapList;    // +0x64
    CEngineMap(const CIDrawMap* gameMap, long worldX, long worldY, long width, long height, long mapUID, CCharString name, bool flag);
};

CEngineMap::CEngineMap(const CIDrawMap* gameMap, long worldX, long worldY, long width, long height, long mapUID, CCharString name, bool flag)
{
    GameMap = gameMap;
    MapWorldPosition.X = (float)worldX;
    MapWorldPosition.Y = (float)worldY;
    MapWorldPosition.Z = 0.0f;
    MapUID = mapUID;
    WorldPosX = (unsigned short)worldX;
    WorldPosY = (unsigned short)worldY;
    MapWidth = (unsigned short)width;
    MapHeight = (unsigned short)height;
    LandscapeMap = 0;
    LocalDetailCacheMap = 0;
    StaticMapName = name;
    IsInVisibleMapList = false;
}