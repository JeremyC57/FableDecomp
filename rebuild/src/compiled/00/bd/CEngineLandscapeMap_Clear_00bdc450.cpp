// CEngineLandscapeMap::Clear() -- drops everything the map holds: the patch array and background patch tree, the map file entry (counted), any async patch loading, the base-load request on the block manager, and the header's request words
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDC450, length 149 bytes.
// Callees: operator delete[] @ 0x00BFEB1C (empty patch array) or the patch vector-deleting destructor (slot 0 with 3), the tree's
// deleting destructor (slot 0 with 1), the counted block's release method then operator delete @ 0x00BFE9BC, DiscardAsyncData
// @ 0x009D68A0 (`__fastcall`, the counted pointer at +0x38), CEngineStaticMapFileBlockManager::ReleaseRequest(long) @ 0x00BE0400.
// Layout: PDB CEngineLandscapeMap (120 bytes) with the retail offsets (TexturePalette and the flags 4 bytes lower than the PDB:
// PatchTree +0x04, Patches +0x08 (array new[] of polymorphic CEngineLandscapePatch), MapFileEntry CCountedPointer +0x18/+0x1C,
// FileBlockManager +0x20, MapFileHeader +0x24 (word 3 = +0x30 is the outstanding base-load request), LoadingPatches
// CCountedPointer +0x38, IsDynamic +0x5D, PatchTreeBaseLoading +0x5E); the landed candidates/landscape_map_open_compiled.h agrees on
// +0x18..+0x40. The CCountedPointer block is {count, pointer-to-member release, object} (`mov ecx,[eax+8] ; call [eax+4]`).
// Shape: one zero register (ebx) for every store and compare; `delete[] Patches` / `delete PatchTree` are unconditional with the
// zero stores after them; the counted pointer is reset through an inline member (`Reset()`: the block re-read after each step,
// both words zeroed) -- written inline in Clear the two zero stores sink below the next block.
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C (the lane <new> lacks it)
class CReleaseTarget { };
typedef void (CReleaseTarget::*CReleaseMethod)();

struct SCountBlock {                               // CCountedPointer's shared block: count, release method, object
    long           RefCount;
    CReleaseMethod Release;
    CReleaseTarget* Object;
};

template <class T>
struct CCountedPointer {
    T*           Object;
    SCountBlock* Block;
    void Reset()
    {
        if (Block) {
            --Block->RefCount;
            if (Block->RefCount == 0) {
                (Block->Object->*Block->Release)();
                delete Block;
            }
        }
        Block = 0;
        Object = 0;
    }
};

class CBankFileAsyncEntry;
class CBankFileAsyncData;
void __fastcall DiscardAsyncData(CCountedPointer<CBankFileAsyncData>* data);   // 0x009D68A0

class CLandscapeBackgroundTreeNode {
public:
    virtual ~CLandscapeBackgroundTreeNode();
};

class CEngineLandscapePatch {
public:
    virtual ~CEngineLandscapePatch();
};

class CEngineStaticMapFileBlockManager {
public:
    void ReleaseRequest(long request);             // 0x00BE0400
    void RemoveFinished();                         // 0x00BE04F0 (manifest "BankFileList_RemoveFinished")
};

struct CMapFileHeader {                            // 20 bytes
    unsigned long Words[5];
};

class CEngineLandscapeMap {                        // PDB 120 bytes; retail offsets (TexturePalette 4 bytes narrower)
public:
    void*                                PadVtable;          // +0x00
    CLandscapeBackgroundTreeNode*        PatchTree;          // +0x04
    CEngineLandscapePatch*               Patches;            // +0x08
    long                                 PatchGridWidth;     // +0x0c
    long                                 PatchGridHeight;    // +0x10
    void*                                Map;                // +0x14 CEngineMap*
    CCountedPointer<CBankFileAsyncEntry> MapFileEntry;       // +0x18
    CEngineStaticMapFileBlockManager*    FileBlockManager;   // +0x20
    CMapFileHeader                       MapFileHeader;      // +0x24
    CCountedPointer<CBankFileAsyncData>  LoadingPatches;     // +0x38
    unsigned char                        TexturePalette[0x1c];   // +0x40
    bool                                 MemoryFreeingLocked;    // +0x5c
    bool                                 IsDynamic;              // +0x5d
    bool                                 PatchTreeBaseLoading;   // +0x5e
    void Clear();
    void OnMapVisibilityChanged(bool visible);
};

void CEngineLandscapeMap::Clear()
{
    delete[] Patches;
    Patches = 0;
    delete PatchTree;
    PatchTree = 0;
    MapFileEntry.Reset();
    IsDynamic = false;
    if (LoadingPatches.Object)
        DiscardAsyncData(&LoadingPatches);
    if (PatchTreeBaseLoading) {
        FileBlockManager->ReleaseRequest(MapFileHeader.Words[3]);
        PatchTreeBaseLoading = false;
    }
    MapFileHeader.Words[2] = 0;
    MapFileHeader.Words[3] = 0;
    MapFileHeader.Words[4] = 0;
}