// CEngineLandscapeMap::CEngineLandscapeMap(CEngineMap* map, long creationFlags) -- landscape map over an engine map: the texture palette over the engine's graphic bank, the patch grid size from the map's dimensions in 16-unit patches, everything else cleared (the map-file entry is explicitly released empty)
// Leaf: ??0CEngineLandscapeMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDD2F0, length 169 bytes (ret 8). Vtable 0x012A7D6C (RTTI CEngineLandscapeMap).
// Callees: CEngineTexturePalette::CEngineTexturePalette(CGraphicDataBank*, bool, unsigned long) 0x00BF90A0 (landed), the
// count block's destroy function (through its +4 pointer) and operator delete 0x00BFE9BC for the inline counted-pointer release.
// Globals: the engine 0x01436E98 (graphic bank +0x954), the render settings 0x01436E20 (+0x78).
// Layout (retail = PDB; the palette is the 0x18-byte retail CEngineTexturePalette): PatchTree +4, Patches +8,
// PatchGridWidth +0xC, PatchGridHeight +0x10, Map +0x14, MapFileEntry +0x18 (object, count block +0x1C), FileBlockManager +0x20,
// MapFileHeader +0x24 (PatchesFilePointer +0x2C, PatchTreeFileBlockPos +0x30, PatchTreeFileBlockSize +0x34),
// LoadingPatches +0x38, TexturePalette +0x40, CreationFlags +0x58, MemoryFreeingLocked +0x5C, IsDynamic +0x5D,
// PatchTreeBaseLoading +0x5E.
// Shape: one zero register (ebx) for every clear; the width is an unsigned shift of the map's field, the height a signed divide
// (`cdq ; and edx,0Fh ; add ; sar`) of the Y of a local {width, height} pair (the aggregate hides the zero-extension's range
// from VC7.1, so it cannot fold the divide to `shr`); the counted-pointer clear re-reads the block after the decrement and stores the count word before
// the object word.
struct CGraphicDataBank;
struct CLandscapeBackgroundTreeNode;
struct CEngineLandscapePatch;
struct CEngineStaticMapFileBlockManager;

struct CEngineMap {
    unsigned char  _pad0[0x5c];
    unsigned short Width;                       // +0x5c
    unsigned short Height;                      // +0x5e
};

struct CVector2i { long X, Y; CVector2i() { } CVector2i(long x, long y) : X(x), Y(y) { } };
static inline long MapUnitsToPatches(long units) { return units / 16; }

struct CEngineGlobal_01436e98 {
    unsigned char     _pad0[0x954];
    CGraphicDataBank* GraphicBank;              // +0x954
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98
struct SRenderSettings_01436e20 {
    unsigned char _pad0[0x78];
    unsigned long TexturePaletteDefault;        // +0x78
};
extern SRenderSettings_01436e20* g_pRenderSettings_01436e20;   // 0x01436E20

struct SCountBlock {
    long  m_references;                         // +0
    void (__fastcall* m_pfnDestroy)(void*);     // +4
    void* m_pObject;                            // +8
};
struct CCountedPointer {
    void*        m_pObject;                     // +0
    SCountBlock* m_pCount;                      // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    void Clear()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pCount = 0;
        m_pObject = 0;
    }
};

struct CMapFileHeader {
    long TexturePaletteFilePointer;             // +0x00
    long TexturePaletteSize;                    // +0x04
    long PatchesFilePointer;                    // +0x08
    long PatchTreeFileBlockPos;                 // +0x0c
    long PatchTreeFileBlockSize;                // +0x10
};

class CEngineTexturePalette {                   // retail 0x18 bytes
public:
    unsigned char _body[0x18];
    CEngineTexturePalette(CGraphicDataBank* bank, bool retailVersion, unsigned long defaultIndex);   // 0x00BF90A0
};

class CEngineLandscapeMap {
public:
    virtual ~CEngineLandscapeMap();
    CLandscapeBackgroundTreeNode*     PatchTree;           // +0x04
    CEngineLandscapePatch*            Patches;             // +0x08
    long                              PatchGridWidth;      // +0x0c
    long                              PatchGridHeight;     // +0x10
    CEngineMap*                       Map;                 // +0x14
    CCountedPointer                   MapFileEntry;        // +0x18
    CEngineStaticMapFileBlockManager* FileBlockManager;    // +0x20
    CMapFileHeader                    MapFileHeader;       // +0x24
    CCountedPointer                   LoadingPatches;      // +0x38
    CEngineTexturePalette             TexturePalette;      // +0x40
    long                              CreationFlags;       // +0x58
    bool                              MemoryFreeingLocked; // +0x5c
    bool                              IsDynamic;           // +0x5d
    bool                              PatchTreeBaseLoading;// +0x5e
    CEngineLandscapeMap(CEngineMap* map, long creationFlags);
};

CEngineLandscapeMap::CEngineLandscapeMap(CEngineMap* map, long creationFlags)
    : TexturePalette(g_pEngine_01436e98->GraphicBank, false, g_pRenderSettings_01436e20->TexturePaletteDefault),
      CreationFlags(creationFlags)
{
    Map = map;
    CVector2i size(map->Width, map->Height);
    PatchGridWidth = map->Width >> 4;
    PatchGridHeight = MapUnitsToPatches(size.Y);
    Patches = 0;
    PatchTree = 0;
    PatchTreeBaseLoading = false;
    FileBlockManager = 0;
    MapFileHeader.PatchesFilePointer = 0;
    MapFileHeader.PatchTreeFileBlockPos = 0;
    MapFileHeader.PatchTreeFileBlockSize = 0;
    MapFileEntry.Clear();
    IsDynamic = false;
    MemoryFreeingLocked = false;
}