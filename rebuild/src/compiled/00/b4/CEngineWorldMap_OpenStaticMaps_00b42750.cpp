// CEngineWorldMap::OpenStaticMaps(CEngineMap::STATIC_MAP_OPEN_STATE state) -- switch the world's static maps to a new open state: build the grid, then either open every map's development static map in place (state 2), or open the static-map bank file, hand it to the water renderer, and load the whole bank at once when it carries the global entry, else open every map from it
// Leaf: OpenStaticMaps
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B42750, length 400 bytes (with int3 padding; body 0x17f), `ret 4`.
// PDB: ?OpenStaticMaps@CEngineWorldMap@@AAEXW4STATIC_MAP_OPEN_STATE@CEngineMap@@@Z.
// Callees: AutoBuildWorldMapGrid @ 0x00B40070; CCharString(const char*, long) @ 0x0099EBF0 / copy @ 0x0099EC30 /
//   ~CCharString @ 0x0099EAE0; __fastcall NProgressDisplay::DisplayProgress(const CCharString&, float, bool, bool)
//   @ 0x009E9F40; CEngineMap::OpenDevelopmentStaticMap(CStaticMapBankFile&, STATIC_MAP_OPEN_STATE) @ 0x00B42530; the bank
//   file's virtual slot 3 (open by name with flags 0x62); its memory-pool setter @ 0x00B3E820 (by-value counted pointer;
//   manifest "CMeshDataBank::SetAnimationMemoryPool", propagated); CEngineWaterRenderer::OpenStaticMapForUse
//   (CStaticMapBankFile*, CCharString) @ 0x00B6D4D0 (global @ 0x01436E54); the bank entry lookup @ 0x009CCDC0 (const
//   CCharString&, as the landed FindEntry callers); CEngineWorldMap::OpenMapFile(long) @ 0x00B420F0 (manifest name).
// Globals: the engine @ 0x01436E98 (memory pool counted pointer at +0x8e0), two CCharString globals @ 0x01436EC4 (the
//   water bank symbol) and 0x01436ECC (the global map entry symbol). String @ 0x012A1F84 "SetStaticMapFileForUse: OpenStaticMap".
// Header: engine/CEngineWorldMap.h flattens MapList/VisibleMapList/StaticMapFile to byte arrays and carries the PDB offsets
//   (StaticMapFile +0x40, GameWorld +0x218) where retail has +0x34/+0x198, so the class is modelled locally.
// Layout: retail CEngineWorldMap (as DeleteMap/AddMap/SetWorld): MapList +0x20, StaticMapBankName (CWideString) +0x30,
//   StaticMapFile +0x34, StaticMapOpenState +0x1a8.
// Shape: the progress text is an implicit conversion temporary; the state-2 loop passes the folded constant 2; the open
//   result is kept in bl across the pool and water calls; the single state store after the if/else is duplicated by VC
//   into each exit (storing ebp, not the propagated 2), which separate early-return stores do not reproduce.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
};

namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}

struct CMovableResourceMemoryPool;
template <class T> struct CCountedPointer {
    T*    Object;
    long* Count;
    CCountedPointer(const CCountedPointer& other) : Object(other.Object), Count(other.Count) { if (Count) ++*Count; }
    ~CCountedPointer();
};

struct CStaticMapBankFile {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual bool Open(const CWideString& name, unsigned long flags);   // slot 3
    void SetMemoryPool(CCountedPointer<CMovableResourceMemoryPool> pool);   // 0x00B3E820
    long FindEntry(const CCharString& name);            // 0x009CCDC0
};

struct CEngineGlobal_01436e98 {
    unsigned char _pad[0x8e0];
    CCountedPointer<CMovableResourceMemoryPool> MemoryPool;   // +0x8e0
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

struct CEngineWaterRenderer {
    void OpenStaticMapForUse(CStaticMapBankFile* file, CCharString symbol);   // 0x00B6D4D0
};
extern CEngineWaterRenderer* g_pEngineWaterRenderer;   // 0x01436E54
extern CCharString g_WaterBankSymbol;                  // 0x01436EC4
extern CCharString g_GlobalMapEntrySymbol;             // 0x01436ECC

struct CEngineMap {
    enum STATIC_MAP_OPEN_STATE { STATIC_MAP_CLOSED, STATIC_MAP_RETAIL, STATIC_MAP_DEVELOPMENT };
    void OpenDevelopmentStaticMap(CStaticMapBankFile& file, STATIC_MAP_OPEN_STATE state);   // 0x00B42530
};

template <class T> struct CArray {                  // retail CArray (STLport vector): begin, end, capacity
    T* First;
    T* Last;
    T* End;
    unsigned long size() const { return Last - First; }
    T& operator[](unsigned long index) { return First[index]; }
};

struct CEngineWorldMap {
    void*                 _vptr;                    // +0x000
    unsigned char         _pad004[0x1c];
    CArray<CEngineMap*>   MapList;                  // +0x020
    unsigned char         _pad02c[0x4];             // +0x02c VisibleMapList
    CWideString           StaticMapBankName;        // +0x030
    CStaticMapBankFile    StaticMapFile;            // +0x034
    unsigned char         _pad038[0x1a8 - 0x38];
    CEngineMap::STATIC_MAP_OPEN_STATE StaticMapOpenState;   // +0x1a8
    void AutoBuildWorldMapGrid();                   // 0x00B40070
    void OpenMapFile(long entry);                   // 0x00B420F0
    void OpenStaticMaps(CEngineMap::STATIC_MAP_OPEN_STATE state);
};

void CEngineWorldMap::OpenStaticMaps(CEngineMap::STATIC_MAP_OPEN_STATE state)
{
    if (StaticMapOpenState == state)
        return;
    AutoBuildWorldMapGrid();
    if (state == CEngineMap::STATIC_MAP_DEVELOPMENT) {
        for (unsigned long i = 0; i < MapList.size(); ++i) {
            if (MapList[i]) {
                NProgressDisplay::DisplayProgress("SetStaticMapFileForUse: OpenStaticMap", -1.0f, false, false);
                MapList[i]->OpenDevelopmentStaticMap(StaticMapFile, state);
            }
        }
    }
    else {
        bool opened = StaticMapFile.Open(StaticMapBankName, 0x62);
        StaticMapFile.SetMemoryPool(g_pEngine_01436e98->MemoryPool);
        if (!opened)
            return;
        g_pEngineWaterRenderer->OpenStaticMapForUse(&StaticMapFile, g_WaterBankSymbol);
        long entry = StaticMapFile.FindEntry(g_GlobalMapEntrySymbol);
        if (entry > 0)
            OpenMapFile(entry);
        else {
            for (unsigned long i = 0; i < MapList.size(); ++i) {
                if (MapList[i]) {
                    NProgressDisplay::DisplayProgress("SetStaticMapFileForUse: OpenStaticMap", -1.0f, false, false);
                    MapList[i]->OpenDevelopmentStaticMap(StaticMapFile, state);
                }
            }
        }
    }
    StaticMapOpenState = state;
}