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
#define free TestFree   // route the model's free() into the test spy
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

class CCharString { public:
    void* PStringData;
    ~CCharString();                                    // 0x0099EAE0
};

class CCPPointerInfo { public:
    long  References;                                  // +0x00
    void (__fastcall* Destroy)(void* object);          // +0x04
    void* Object;                                      // +0x08
};
template <class T> class CCountedPointer { public:
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

class CGraphicDataBank;
class CEngineTexturePalette { public:                         // retail 0x18 bytes
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

class CEngineStaticMapFileBlockManager { public:
    void ReleaseRequest(long request);                 // 0x00BE0400
};
class CLandscapeBackgroundTreeNode { public:
    virtual ~CLandscapeBackgroundTreeNode();
};
class CEngineLandscapePatch { public:
    virtual ~CEngineLandscapePatch();
};
class CBankFileAsyncEntry;
class CBankFileAsyncData;
class CEngineMap;

class CMovableResource { public:                              // vtable 0x0129B860
    virtual ~CMovableResource() { }
};

class CEngineLandscapeMap : public CMovableResource { public:        // vtable 0x012A7D6C
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

// ---- test doubles and driver ----


#undef free
static void free_real(void* p) { std::free(p); }
static int g_log[64]; static int g_n = 0; static bool g_armed = false; static int g_frees = 0; static int g_deletes = 0; static int g_arrayDeletes = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
extern "C" void __cdecl TestFree(void*) { ++g_frees; }
void __cdecl operator delete(void* block) { if (g_armed) { ++g_deletes; return; } ::free_real(block); }
void __cdecl operator delete[](void* block) { if (g_armed) { ++g_arrayDeletes; return; } ::free_real(block); }
CCharString::~CCharString() { Log(1); }
static long g_released = -1;
void CEngineStaticMapFileBlockManager::ReleaseRequest(long request) { g_released = request; Log(2); }
CLandscapeBackgroundTreeNode::~CLandscapeBackgroundTreeNode() { Log(3); }
CEngineLandscapePatch::~CEngineLandscapePatch() { Log(4); }
static int g_destroyed = 0;
static void __fastcall DestroyObject(void*) { ++g_destroyed; }
int main()
{
    static double raw[32];
    CEngineLandscapeMap* m = new (raw) CEngineLandscapeMap;
    static CEngineStaticMapFileBlockManager manager;
    m->PatchTreeBaseLoading = true; m->FileBlockManager = &manager; m->MapFileHeader.PatchTreeFileBlockPos = 77;
    m->Patches = new CEngineLandscapePatch[2];
    m->PatchTree = new CLandscapeBackgroundTreeNode;
    static CPaletteEntry entries[3];
    m->TexturePalette.Palette._M_start = entries; m->TexturePalette.Palette._M_finish = entries + 3;
    static CCPPointerInfo info1, info2;
    info1.References = 1; info1.Destroy = DestroyObject; info2.References = 2; info2.Destroy = DestroyObject;
    m->LoadingPatches.Info = &info1; m->LoadingPatches.Data = 0;
    m->MapFileEntry.Info = &info2; m->MapFileEntry.Data = 0;
    g_armed = true;
    m->CEngineLandscapeMap::~CEngineLandscapeMap();
    g_armed = false;
    // file block released, two patches and the tree destroyed, three palette names; loading block destroyed and deleted, map-file block kept
    static const int want[] = { 2, 4, 4, 3, 1, 1, 1 };
    bool ok = g_released == 77 && m->PatchTreeBaseLoading == false && g_n == 7 && g_frees == 1 && g_arrayDeletes == 1
        && g_deletes == 2 && g_destroyed == 1 && info2.References == 1 && m->LoadingPatches.Info == 0 && m->MapFileEntry.Info == 0;
    for (int i = 0; ok && i < 7; ++i) ok = g_log[i] == want[i];
    if (!ok) { std::printf("00bdd210_TEST FAIL %d %d %d %d\n", g_n, g_frees, g_arrayDeletes, g_deletes); return 1; }
    std::printf("00bdd210_TEST PASS\n");
    return 0;
}