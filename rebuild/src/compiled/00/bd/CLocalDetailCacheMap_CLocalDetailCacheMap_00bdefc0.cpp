// CLocalDetailCacheMap::CLocalDetailCacheMap(CEngineMap*) -- construct a map's local-detail cache: unlocked, no file-block manager, a root quad-tree element covering the whole map, an empty update list and palette
// Leaf: ??0CLocalDetailCacheMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDEFC0, length 66 bytes.
// Callees: CLocalDetailCacheMap::CQuadTreeElement::CQuadTreeElement(CLocalDetailCacheMap*, long, long, long, long)
// @ 0x00BDDF30. Ego_r.pdb ??0CLocalDetailCacheMap@@QAE@PAVCEngineMap@@@Z; caller
// CEngineLocalDetailGenerator::CreateMap 0x00B59800.
// Identity: the manifest label CProjectileWeapon::CProjectileWeapon is propagated.
// Layout: LockedFromMemoryRelocation +0, FileBlockManager +4, Map +8, QuadTree +0xc (0x4c bytes),
// AreaUpdateList +0x58 and CollectionTypePalette +0x64 (retail 12-byte containers; the engine header
// CLocalDetailCacheMap.h has the PDB widths and opaque members, so the class is modelled locally); CEngineMap
// MapWidth/MapHeight are retail +0x5c/+0x5e (PDB +0x60: the debug list iterator is wider).
// Shape: Map is a BODY assignment (stored after every member constructor) while the quad-tree arguments use
// the parameter; the zero stores after the quad-tree call are the two containers' inline constructors.
class CEngineMap { public:
    unsigned char  _pad00[0x5c];
    unsigned short MapWidth;                                // +0x5c (PDB +0x60: debug list iterator)
    unsigned short MapHeight;                               // +0x5e
};

class CEngineStaticMapFileBlockManager;
class C2DBoxI;
class CLocalDetailObjectCollectionType;

template <class T> struct CRetailVector {                   // retail std::vector / CArray: three pointers
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    CRetailVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CObjectTypeCollectionPalette {
    CRetailVector<const CLocalDetailObjectCollectionType*> Entries;
};

class CLocalDetailCacheMap { public:
    class CQuadTreeElement { public:                               // 0x4c bytes
        unsigned char _body[0x4c];
        CQuadTreeElement(CLocalDetailCacheMap* map, long x, long y, long width, long height);   // 0x00BDDF30
    };
    bool                              LockedFromMemoryRelocation; // +0x00
    CEngineStaticMapFileBlockManager* FileBlockManager;      // +0x04
    CEngineMap*                       Map;                   // +0x08
    CQuadTreeElement                  QuadTree;              // +0x0c
    CRetailVector<C2DBoxI>            AreaUpdateList;        // +0x58
    CObjectTypeCollectionPalette      CollectionTypePalette; // +0x64
    CLocalDetailCacheMap(CEngineMap* map);
};

CLocalDetailCacheMap::CLocalDetailCacheMap(CEngineMap* map)
    : LockedFromMemoryRelocation(false), FileBlockManager(0),
      QuadTree(this, 0, 0, map->MapWidth, map->MapHeight)
{
    Map = map;
}