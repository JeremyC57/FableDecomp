// CEngineMap::~CEngineMap() -- engine map destructor
// Leaf: ??1CEngineMap@@QAE@XZ
#include "engine/CCountedPointer.h"
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                             // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
class C3DBoundingBox { public: C3DVector Min, Max; };
class CEngineMap;
class CEngineLandscapeMap;
class CLocalDetailCacheMap;
class CIDrawMap;
class CBankFileAsyncEntry;
class CEngineLandscapeRenderer { public:
    void DeleteMap(CEngineMap* map);            // 0x00B67180
};
extern CEngineLandscapeRenderer* g_pLandscapeRenderer;          // 0x01436EA8
class CEngineLocalDetailGenerator { public:
    void DeleteMap(CEngineMap* map);            // 0x00B59840
};
extern CEngineLocalDetailGenerator* g_pLocalDetailGenerator;    // 0x01436E88
class CEngineStaticMapFileBlockManager { public:
    unsigned char _body[0x14];                  // AsyncBankEntry (CCountedPointer) +0x00, Blocks (STLport map) +0x08
    void Reset();                               // 0x00BE05D0
    ~CEngineStaticMapFileBlockManager();        // 0x00B443B0
};
enum STATIC_MAP_OPEN_STATE { STATIC_MAP_CLOSED_DUMMY };
class CEngineMap { public:
    CEngineLandscapeMap*  LandscapeMap;         // +0x00
    CLocalDetailCacheMap* LocalDetailCacheMap;  // +0x04
    CIDrawMap*            GameMap;              // +0x08
    C3DVector             MapWorldPosition;     // +0x0c
    long                  MapUID;               // +0x18
    CCharString           StaticMapName;        // +0x1c
    STATIC_MAP_OPEN_STATE StaticMapOpenState;   // +0x20
    C3DBoundingBox        CameraMapBounds;      // +0x24
    CCountedPointer<CBankFileAsyncEntry> AsyncFileEntry;   // +0x3c
    CEngineStaticMapFileBlockManager FileBlockManager;     // +0x44
    unsigned char         _tail58[0x10];        // +0x58
    void CloseStaticMap();                      // 0x00B3EF40
    ~CEngineMap();
};
CEngineMap::~CEngineMap()
{
    CloseStaticMap();
    if (LandscapeMap)
        g_pLandscapeRenderer->DeleteMap(this);
    if (LocalDetailCacheMap)
        g_pLocalDetailGenerator->DeleteMap(this);
    FileBlockManager.Reset();
}
