// Grade: DIFFER(163v169) -- retail divides the zero-extended MapHeight by 16 with the signed `cdq ; and edx,0Fh ; add ; sar` sequence while the lane compiler proves it non-negative and emits `shr`; the map parameter is loaded into ecx (ours eax) and the info block pushed from eax (ours ecx)
// CEngineLandscapeMap::CEngineLandscapeMap(CEngineMap*, long) -- constructor: empty file/loading counted pointers, the texture palette (engine texture bank, non-retail, the render settings' default index), the creation flags and owning map, the patch grid size in 16-cell patches, and every patch/tree/header/flag field cleared
// Leaf: ??0CEngineLandscapeMap
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDD2F0, 169 bytes, `ret 8`.
// Callees: CEngineTexturePalette::CEngineTexturePalette(CGraphicDataBank*, bool, unsigned long) 0x00BF90A0 (manifest label
//   "CParticleMorphs::Initialize" is propagated; the body zeroes the palette vector, stores the bool at +0x14 and the default
//   index at +0x10), operator delete 0x00BFE9BC. Globals: the engine data 0x01436E98 (texture bank at +0x954), the render
//   settings 0x01436E20 (+0x78, the palette's default index).
// Vtable: 0x012A7D6C CEngineLandscapeMap.
// Layout (retail; Ego_r.pdb agrees up to the palette, whose CArray is 12 bytes in retail): PatchTree +4, Patches +8,
//   PatchGridWidth +0xc, PatchGridHeight +0x10, Map +0x14, MapFileEntry +0x18 {Data, Info}, FileBlockManager +0x20,
//   MapFileHeader +0x24 (TexturePaletteFilePointer, TexturePaletteSize, PatchesFilePointer +0x2c, PatchTreeFileBlockPos +0x30,
//   PatchTreeFileBlockSize +0x34), LoadingPatches +0x38, TexturePalette +0x40, CreationFlags +0x58, MemoryFreeingLocked +0x5c,
//   IsDynamic +0x5d, PatchTreeBaseLoading +0x5e. CEngineMap (retail, 4 below the PDB): MapWidth +0x5c, MapHeight +0x5e.
// Shape: the counted pointers and the palette are member objects built after the vtable store; every scalar is a body
//   assignment (stored after the member ctors, in source order, one zero register); the grid width is `MapWidth >> 4`
//   (`shr`), the height `MapHeight / 16` (signed `cdq ; and ; add ; sar`); MapFileEntry is reset through an inline Reset()
//   (release the info block, then Info = 0, Data = 0).
void __cdecl operator delete(void* block);             // 0x00BFE9BC

struct CCPPointerInfo {
    long  References;                                  // +0x00
    void (__fastcall* Destroy)(void* object);          // +0x04
    void* Object;                                      // +0x08
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    void Reset()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
        Info = 0;
        Data = 0;
    }
};

struct CGraphicDataBank;
struct CEngineTexturePalette {                         // retail 0x18 bytes
    CGraphicDataBank* Bank;                            // +0x00
    void*             Palette[3];                      // +0x04
    unsigned long     DefaultIndex;                    // +0x10
    bool              RetailVersion;                   // +0x14
    CEngineTexturePalette(CGraphicDataBank* bank, bool retailVersion, unsigned long defaultIndex);   // 0x00BF90A0
};

struct CEngineData {                                   // the engine data singleton
    unsigned char     _pad[0x954];
    CGraphicDataBank* TextureBank;                     // +0x954
};
extern CEngineData* g_pEngineData;                     // 0x01436E98
struct CRenderSettings {
    unsigned char _pad[0x78];
    unsigned long DefaultTexturePaletteIndex;          // +0x78
};
extern CRenderSettings* g_pRenderSettings;             // 0x01436E20

struct CEngineMap {
    unsigned char  _pad00[0x5c];
    unsigned short MapWidth;                           // +0x5c (retail)
    unsigned short MapHeight;                          // +0x5e
};

struct CMapFileHeader {
    long TexturePaletteFilePointer;
    long TexturePaletteSize;
    long PatchesFilePointer;
    long PatchTreeFileBlockPos;
    long PatchTreeFileBlockSize;
};

struct CBankFileAsyncEntry;
struct CBankFileAsyncData;
struct CLandscapeBackgroundTreeNode;
struct CEngineLandscapePatch;
struct CEngineStaticMapFileBlockManager;

struct CEngineLandscapeMap {                           // vtable 0x012A7D6C
    CLandscapeBackgroundTreeNode*         PatchTree;   // +0x04
    CEngineLandscapePatch*                Patches;     // +0x08
    long                                  PatchGridWidth;    // +0x0c
    long                                  PatchGridHeight;   // +0x10
    CEngineMap*                           Map;         // +0x14
    CCountedPointer<CBankFileAsyncEntry>  MapFileEntry;   // +0x18
    CEngineStaticMapFileBlockManager*     FileBlockManager;   // +0x20
    CMapFileHeader                        MapFileHeader;  // +0x24
    CCountedPointer<CBankFileAsyncData>   LoadingPatches; // +0x38
    CEngineTexturePalette                 TexturePalette; // +0x40
    long                                  CreationFlags;  // +0x58
    bool                                  MemoryFreeingLocked;   // +0x5c
    bool                                  IsDynamic;      // +0x5d
    bool                                  PatchTreeBaseLoading;  // +0x5e
    virtual void v00();
    CEngineLandscapeMap(CEngineMap* map, long creationFlags);
};

CEngineLandscapeMap::CEngineLandscapeMap(CEngineMap* map, long creationFlags)
    : TexturePalette(g_pEngineData->TextureBank, false, g_pRenderSettings->DefaultTexturePaletteIndex)
{
    CreationFlags = creationFlags;
    Map = map;
    PatchGridWidth = Map->MapWidth >> 4;
    PatchGridHeight = Map->MapHeight / 16;
    Patches = 0;
    PatchTree = 0;
    PatchTreeBaseLoading = false;
    FileBlockManager = 0;
    MapFileHeader.PatchesFilePointer = 0;
    MapFileHeader.PatchTreeFileBlockPos = 0;
    MapFileHeader.PatchTreeFileBlockSize = 0;
    MapFileEntry.Reset();
    IsDynamic = false;
    MemoryFreeingLocked = false;
}
