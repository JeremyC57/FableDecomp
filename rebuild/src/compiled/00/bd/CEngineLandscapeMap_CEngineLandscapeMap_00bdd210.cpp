// CEngineLandscapeMap::~CEngineLandscapeMap() -- destructor: release a still-pending patch-tree file block, delete the patch array and the patch tree, then the texture palette, the loading-patches and map-file counted pointers and the CMovableResource base
// Leaf: ??1CEngineLandscapeMap
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDD210, 218 bytes (+ int3 padding).
// Callees: CEngineStaticMapFileBlockManager::ReleaseRequest(long) 0x00BE0400 (on FileBlockManager, the PatchTreeFileBlockPos
//   request), operator delete[] 0x00BFEB1C (an empty patch array's block), the patches' and the tree's virtual dtors (slot 0
//   with 3 / 1), ~CCharString 0x0099EAE0 (each palette entry's name), free 0x00BFEA14, operator delete 0x00BFE9BC (the counted
//   pointers' info blocks).
// Vtables: 0x012A7D6C CEngineLandscapeMap (stored first), 0x0129B860 CMovableResource (the base, stored last by its inline dtor).
// Layout: as the ctor 0x00BDD2F0 (retail): PatchTree +4, Patches +8, Map +0x14, MapFileEntry +0x18, FileBlockManager +0x20,
//   MapFileHeader +0x24 (PatchTreeFileBlockPos +0x30), LoadingPatches +0x38, TexturePalette +0x40 (Palette vector +0x44 of
//   8-byte {Index, CCharString Name} entries), PatchTreeBaseLoading +0x5e.
// Shape: `delete[] Patches` on a class with a virtual dtor (the count word decides between `??_E` with 3 and a bare
//   operator delete[]); `delete PatchTree` (virtual dtor, slot 0 with 1); the palette entries die through an inline loop with
//   the end cached (by-value _Destroy); each counted pointer's dtor releases the info block then clears Data and Info; one
//   zero register (ebx) throughout.
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
void __cdecl operator delete(void* block);             // 0x00BFE9BC
void __cdecl operator delete[](void* block);           // 0x00BFEB1C

struct CCharString {
    void* PStringData;
    ~CCharString();                                    // 0x0099EAE0
};

struct CCPPointerInfo {
    long  References;                                  // +0x00
    void (__fastcall* Destroy)(void* object);          // +0x04
    void* Object;                                      // +0x08
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};

struct CPaletteEntry {                                 // CEngineTexturePalette::CPaletteEntry
    unsigned long Index;                               // +0x00
    CCharString   Name;                                // +0x04
};
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
struct CPaletteVector {                                // vector<CPaletteEntry> (retail 12 bytes)
    CPaletteEntry* _M_start;
    CPaletteEntry* _M_finish;
    CPaletteEntry* _M_end_of_storage;
    ~CPaletteVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};

struct CGraphicDataBank;
struct CEngineTexturePalette {                         // retail 0x18 bytes
    CGraphicDataBank* Bank;                            // +0x00
    CPaletteVector    Palette;                         // +0x04
    unsigned long     DefaultIndex;                    // +0x10
    bool              RetailVersion;                   // +0x14
};

struct CMapFileHeader {
    long TexturePaletteFilePointer;
    long TexturePaletteSize;
    long PatchesFilePointer;
    long PatchTreeFileBlockPos;
    long PatchTreeFileBlockSize;
};

struct CEngineStaticMapFileBlockManager {
    void ReleaseRequest(long request);                 // 0x00BE0400
};
struct CLandscapeBackgroundTreeNode {
    virtual ~CLandscapeBackgroundTreeNode();
};
struct CEngineLandscapePatch {
    virtual ~CEngineLandscapePatch();
};
struct CBankFileAsyncEntry;
struct CBankFileAsyncData;
struct CEngineMap;

struct CMovableResource {                              // vtable 0x0129B860
    virtual ~CMovableResource() { }
};

struct CEngineLandscapeMap : CMovableResource {        // vtable 0x012A7D6C
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
    ~CEngineLandscapeMap();
};

CEngineLandscapeMap::~CEngineLandscapeMap()
{
    if (PatchTreeBaseLoading) {
        FileBlockManager->ReleaseRequest(MapFileHeader.PatchTreeFileBlockPos);
        PatchTreeBaseLoading = false;
    }
    delete[] Patches;
    delete PatchTree;
}