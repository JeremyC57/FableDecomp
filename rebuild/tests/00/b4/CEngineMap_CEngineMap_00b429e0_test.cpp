#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// Behaviour test: the model and logic below are a copy of the src/ file; the callees are test doubles.
#include <new>
#include <cstdlib>
#define malloc TestMalloc   // route the model's malloc() into the test double
// CEngineMap::CEngineMap(const CIDrawMap*, long, long, long, long, long, CCharString, bool) -- constructor: empty name, file entry and file-block manager (whose block map allocates its header node), then the game map, world position (as floats and as 16-bit grid coordinates), map size, UID and static-map name; not yet in the visible-map list
// Leaf: ??0CEngineMap
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B429E0, 170 bytes (+ int3 padding), `ret 20h`. Mangled
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

struct CCharString {
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};

struct C3DVector { float X, Y, Z; };
struct C3DBoundingBox { C3DVector Min, Max; };

template <class T> struct CCountedPointer {
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

struct CBankFileAsyncEntry;
struct CEngineStaticMapFileBlockManager {              // retail 0x14 bytes
    CCountedPointer<CBankFileAsyncEntry> AsyncBankEntry;   // +0x00
    CFileBlockMap                        Blocks;           // +0x08
};

struct CEngineLandscapeMap;
struct CLocalDetailCacheMap;
struct CIDrawMap;

struct CEngineMap {
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

// ---- test doubles and driver ----
static unsigned char g_heap[256]; static int g_mallocs = 0; static unsigned int g_size = 0;
extern "C" void* __cdecl TestMalloc(unsigned int size) { ++g_mallocs; g_size = size; return g_heap; }
static int g_assigns = 0, g_dtors = 0;
CCharString::CCharString() : PStringData(0) { }
CCharString::~CCharString() { ++g_dtors; }
CCharString& CCharString::operator=(const CCharString& other) { PStringData = other.PStringData; ++g_assigns; return *this; }
int main()
{
    static double raw[32];
    std::memset(raw, 0xcd, sizeof(raw));
    CCharString name; name.PStringData = (void*)0x1234;
    CIDrawMap* game = reinterpret_cast<CIDrawMap*>(&g_assigns);
    CEngineMap* m = new (raw) CEngineMap(game, 3, -2, 64, 32, 77, name, true);
    SFileBlockNode* h = m->FileBlockManager.Blocks._M_header;
    bool ok = m->LandscapeMap == 0 && m->LocalDetailCacheMap == 0 && m->GameMap == game
        && m->MapWorldPosition.X == 3.0f && m->MapWorldPosition.Y == -2.0f && m->MapWorldPosition.Z == 0.0f
        && m->MapUID == 77 && m->StaticMapName.PStringData == (void*)0x1234 && g_assigns == 1
        && m->MapWidth == 64 && m->MapHeight == 32 && m->WorldPosX == 3 && m->WorldPosY == (unsigned short)-2
        && m->IsInVisibleMapList == false && m->AsyncFileEntry.Data == 0 && m->AsyncFileEntry.Info == 0
        && m->FileBlockManager.AsyncBankEntry.Data == 0 && m->FileBlockManager.AsyncBankEntry.Info == 0
        && g_mallocs == 1 && g_size == sizeof(SFileBlockNode) && h == (SFileBlockNode*)g_heap && h->_M_color == 0 && h->_M_parent == 0
        && h->_M_left == h && h->_M_right == h && m->FileBlockManager.Blocks._M_node_count == 0;
    if (!ok) { std::printf("00b429e0_TEST FAIL\n"); return 1; }
    std::printf("00b429e0_TEST PASS\n");
    return 0;
}