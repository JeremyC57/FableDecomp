// CWorldMap::LoadScriptBrushesFromFile(long, CCountedPointer<CAFile>) -- load one map's scripted map brushes from a level file
// Leaf: ?LoadScriptBrushesFromFile@CWorldMap@@MAEXJV?$CCountedPointer@VCAFile@@@@@Z
// Retail: Fable.exe @ 0x004FC930, 56 bytes, `ret 0Ch`. PDB ?LoadScriptBrushesFromFile@CWorldMap@@MAEXJV?$CCountedPointer@VCAFile@@@@@Z
// (protected virtual, primary vtable 0x01244AEC slot 29).
// Callees: CMap::LoadBrushesFromFile(CAFile&) 0x00821010 (manifest label "CWorldMap_LoadScriptBrushesFromFile" is a
// guess; PDB ?LoadBrushesFromFile@CMap@@QAEXAAVCAFile@@@Z), operator delete 0x00BFE9BC.
// Retail CWorldMap (engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport vectors / CArrays and
// the map member are 12 bytes, so a local model is used): +0x20 Maps (CArray<CMapInfo>, PDB +0x24); CMapInfo (0x48
// bytes): +0x10 PMap (CCountedPointer<CMap>::Data).
// Shape: the file argument (`*file`, its Data word) is pushed before the map is looked up (right-to-left), then the
// callee-destroyed by-value counted pointer is released inline: CCPPointerInfo {Count, static __fastcall
// DeleteData(void*), Data} (`dec [esi] ; jne ; mov ecx,[esi+8] ; call [esi+4]`, then the info block is deleted).
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class CAFile;

class CMap {
public:
    void LoadBrushesFromFile(CAFile& file);     // 0x00821010
};

class CCPPointerInfo {
public:
    long  Count;                                // +0x00
    void (__fastcall* DeleteData)(void* data);  // +0x04
    void* Data;                                 // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                       // +0x00
    CCPPointerInfo* Info;                       // +0x04
    ~CCountedPointer()
    {
        if (Info)
        {
            if (--Info->Count == 0)
            {
                Info->DeleteData(Info->Data);
                operator delete(Info);
            }
        }
    }
    T& operator*() const { return *Data; }
};

class CMapInfo {
public:
    C2DBoxI       Area;                         // +0x00
    CMap*         PMap;                         // +0x10 CCountedPointer<CMap>::Data
    void*         PMapInfo;                     // +0x14 CCountedPointer<CMap>::Info
    unsigned char _rest[0x30];                  // +0x18 .. +0x48
};

template <class T> class CArray {
public:
    T* Start;                                   // +0x00
    T* Finish;                                  // +0x04
    T* EndOfStorage;                            // +0x08
    const T& operator[](unsigned long i) const { return Start[i]; }
};

class CWorldMap {
public:
    virtual void v00();
    unsigned char    _pad04[0x1c];
    CArray<CMapInfo> Maps;                      // +0x20 (PDB +0x24)
protected:
    virtual void LoadScriptBrushesFromFile(long map, CCountedPointer<CAFile> file);
};

void CWorldMap::LoadScriptBrushesFromFile(long map, CCountedPointer<CAFile> file)
{
    Maps[map].PMap->LoadBrushesFromFile(*file);
}