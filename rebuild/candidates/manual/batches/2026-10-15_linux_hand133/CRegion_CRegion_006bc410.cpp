// CRegion::CRegion() -- default constructor of a world-map region: empty map lists, empty names, null region def, mini-map scale 1.0, an empty exit-text-offset map, zero offsets, hidden on the world map with creature generation and sound themes enabled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BC410, length 132 bytes (+ int3 padding).
// Callees: CCharString::CCharString() @ 0x0099E4B0 (x4, out of line), malloc @ 0x00BFEA0E (the map's 0x1c-byte head node).
// Label correction: the manifest's "CEngineInit::CEngineInit" is a propagated name; the callers (CWorldMap::CWorldMap 0x005066E0,
// CWorldMap::LoadFromFile, the CRegion vector allocator 0x0051BEC0) and the member pattern are the PDB CRegion. Layout: retail STLport
// vectors are 12 bytes (PDB 16), so the engine header CRegion.h (PDB offsets) does not apply: Maps +0x00, VisMaps +0x0c, Name +0x18,
// DisplayName +0x1c, RegionDefName +0x20, PDef +0x24, MiniMapGraphic +0x28, MiniMapScale +0x2c, MiniMapOffset +0x30,
// MiniMapRegionExitTextOffsets +0x38 (STLport map: head, count, comparator = 12 bytes), WorldMapOffset +0x44, NameGraphicOffset +0x4c
// (left uninitialised), AppearOnWorldMap +0x54, CreatureGenerationEnabled +0x55, SoundThemesEnabled +0x56.
// Shape: member constructors in declaration order (vector triples and the def pointer from the ebx zero register, the string constructors
// out of line); the STLport map constructor nulls the head, allocates it, zeroes the count, then colour/parent/left/right through the
// re-read head. The two `true` flags share `mov al,1`. The vectors need STLport's `_STLP_alloc_proxy` end-of-storage member (its own
// inline constructor): with three plain pointers the lane schedules the first string's `lea ecx,[esi+18h]` above the VisMaps zero stores.
// Leaf: ??0CRegion
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CCharString {
public:
    CCharString();                          // 0x0099E4B0
    struct CCharStringData* PStringData;
};

template <class T>
struct SRetailAllocProxy {                  // STLport _STLP_alloc_proxy<T*, T, allocator>: the end-of-storage pointer
    T* m_data;
    SRetailAllocProxy(T* data) : m_data(data) {}
};

template <class T>
struct CRetailVector {                      // STLport vector (_Vector_base): 12 bytes
    T* m_start;
    T* m_finish;
    SRetailAllocProxy<T> m_endOfStorage;
    CRetailVector() : m_start(0), m_finish(0), m_endOfStorage(0) {}
};

struct CDefPointerRegionDef {               // CDefPointer<const CRegionDef>: one pointer
    struct CRegionDef* Object;
    CDefPointerRegionDef() : Object(0) {}
};

struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};

struct CExitTextOffsetMap {                 // STLport map<CCharString, C2DVector>: head node pointer, count, empty comparator
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; unsigned char value[0x1c - 16]; };
    node*         m_header;
    unsigned long m_nodeCount;
    unsigned char m_keyCompare;
    CExitTextOffsetMap()
        : m_header(0)
    {
        m_header = (node*)malloc(sizeof(node));
        m_nodeCount = 0;
        m_header->colour = 0;
        m_header->parent = 0;
        m_header->left = m_header;
        m_header->right = m_header;
    }
};

struct CRegion {
    CRetailVector<long>  Maps;                          // +0x00
    CRetailVector<long>  VisMaps;                       // +0x0c
    CCharString          Name;                          // +0x18
    CCharString          DisplayName;                   // +0x1c
    CCharString          RegionDefName;                 // +0x20
    CDefPointerRegionDef PDef;                          // +0x24
    CCharString          MiniMapGraphic;                // +0x28
    float                MiniMapScale;                  // +0x2c
    C2DVector            MiniMapOffset;                 // +0x30
    CExitTextOffsetMap   MiniMapRegionExitTextOffsets;  // +0x38
    C2DVector            WorldMapOffset;                // +0x44
    C2DVector            NameGraphicOffset;             // +0x4c
    bool                 AppearOnWorldMap;              // +0x54
    bool                 CreatureGenerationEnabled;     // +0x55
    bool                 SoundThemesEnabled;            // +0x56
    CRegion();
};

CRegion::CRegion()
    : MiniMapScale(1.0f),
      MiniMapOffset(0, 0),
      WorldMapOffset(0, 0),
      AppearOnWorldMap(false),
      CreatureGenerationEnabled(true),
      SoundThemesEnabled(true)
{
}
