// CWorldMap::CreateNavMap(long map, const set<CCharString>& sections, CAFile& file, CASuspendableProcess& process) -- give the navigator manager the navigation map of a level file: from the matching named section when the file has a section table (replacing a map loaded from a different section), else from the plain navigation block when none is loaded
// Leaf: CreateNavMap
// Retail: Fable.exe (TLC, VC7.1) @ 0x00500230, 425 bytes (+ int3 padding), `ret 10h`.
//   Manifest signature CWorldMap::CreateNavMap(long, set<CCharString>*, CAFile*, CASuspendableProcess*).
// Callees: CAFile vtable slot 5 Seek (+0x14) and slot 7 (+0x1C, the current position), CReplaceSerialise<CFileFormatLevel,
//   ...>::ReadFromFile 0x0051A240 (manifest names the EditMapBrushHeader twin, as the landed GetLevelDimensions
//   0x00506BB0), CAFile::ReadVar<int> 0x00516180, CMemoryFile: CAFile base ctor 0x0099A2F0 / dtor 0x0099A300, vtable
//   0x0122E0FC, CWideString ctor 0x0099AED0 / dtor 0x0099B510, OpenFromFile 0x009E9420, Close 0x009E9390, CRT free
//   0x00BFEA14; the level's section finder 0x004FE630 (manifest "CreateNavMap_LoadParticleMorphs"; returns the
//   chosen section's name and offset by value); CNavigatorManager IsNavigationMapLoadedWithSectionName 0x00A14EE0,
//   IsNavigationMapLoaded 0x00A14E80, RemoveNavigationMap 0x00A155D0, AddNavigationMap(long, const C2DBoxI&,
//   const CCharString&, CAFile&, CASuspendableProcess*) 0x00A157C0; CCharString(const char*, long) 0x0099EBF0 ("" at
//   0x0122D70E) / ~CCharString 0x0099EAE0.
// Shape: first try; the section-table end is an `int` read into the dead `process` argument slot, the navigator
//   pointer lives in the dead `file` slot, the "" name temporary in the dead `map` slot.
// Layout: local models at retail offsets (engine/ headers carry the PDB's 16-byte containers): CWorldMap World +0x08,
//   Maps +0x20 (CMapInfo 0x48 bytes, Area +0), CWorld NavigatorManager +0x48 (as the landed UnloadLevel 0x004FEDE0);
//   CFileFormatLevel packed (as GetLevelDimensions); CMemoryFile as the landed OpenFromFile 0x009E9420.
extern "C" void free(void* block);                               // 0x00BFEA14

class CASuspendableProcess;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);             // 0x0099EBF0
    CCharString(const CCharString& other);                       // 0x0099EC30
    ~CCharString();                                              // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString();                                               // 0x0099AED0
    ~CWideString();                                              // 0x0099B510
};
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };

class CBaseClassNonCopyable { protected:
    CBaseClassNonCopyable();                                     // 0x0099A2F0 ??0CBaseClassNonCopyable@@IAE@XZ
    virtual ~CBaseClassNonCopyable();                            // slot 0 (body 0x0099A300)
};
class __declspec(novtable) CAFile : public CBaseClassNonCopyable { public:
    CAFile() { }                                                 // the base constructor stores the vtable
    virtual void Slot01(); virtual void Slot02(); virtual void Slot03(); virtual void Slot04();
    virtual void Seek(long position);                            // slot 5 (+0x14)
    virtual void Slot06();
    virtual long GetPosition();                                  // slot 7 (+0x1C)
    void ReadVar(int* value, CASuspendableProcess* process);     // 0x00516180
};
struct CArrayUChar {
    unsigned char* m_start;
    unsigned char* m_finish;
    unsigned char* m_endOfStorage;
    CArrayUChar() : m_start(0), m_finish(0), m_endOfStorage(0) { }
    ~CArrayUChar() { if (m_start != 0) free(m_start); }
};
class CMemoryFile : public CAFile { public:                                    // vtable 0x0122E0FC
    long          AccessType;                                    // +0x04
    long          Position;                                      // +0x08
    long          Length;                                        // +0x0C
    CArrayUChar   DataBuffer;                                    // +0x10
    bool          Opened;                                        // +0x1C
    CWideString   Pathname;                                      // +0x20
    CMemoryFile() : Opened(false) { }
    ~CMemoryFile() { Close(); }
    virtual void Close();                                        // 0x009E9390
    void OpenFromFile(CAFile* file, long length, CASuspendableProcess* process);   // 0x009E9420
};

#pragma pack(push, 1)
class CFileFormatLevel { public:
    unsigned long ByteLength;                                    // +0x00 CReplaceSerialise
    unsigned char SerialiseVersionNumber;                        // +0x04
    long          MapDataOffset;                                 // +0x05
    long          ThingDataOffset;                               // +0x09
    long          BrushDataOffset;                               // +0x0D
    long          NavigationDataOffset;                          // +0x11
    long          NavigationSectionsOffset;                      // +0x15
    void ReadFromFile(CAFile* file, CASuspendableProcess* process);   // 0x0051A240
};
#pragma pack(pop)

struct CNavigationSection {                                      // the section finder's result
    CCharString Name;
    long        Offset;
};
struct CCharStringSet;                                           // std::set<CCharString>

class CNavigatorManager { public:
    bool IsNavigationMapLoadedWithSectionName(long map, const CCharString& name) const;   // 0x00A14EE0
    bool IsNavigationMapLoaded(long map) const;                  // 0x00A14E80
    void RemoveNavigationMap(long map);                          // 0x00A155D0
    void AddNavigationMap(long map, const C2DBoxI& area, const CCharString& section, CAFile& file,
                          CASuspendableProcess* process);        // 0x00A157C0
};
class CWorld { public:
    unsigned char _pad[0x48];
    CNavigatorManager* NavigatorManager;                         // +0x48
};
class CMapInfo { public:                                                // 0x48 bytes
    C2DBoxI       Area;                                          // +0x00
    unsigned char _rest[0x38];
};
struct CMapInfoArray { CMapInfo* Start; CMapInfo* Finish; CMapInfo* EndOfStorage; };

class CWorldMap { public:
    void*         m_pVtable;
    long          m_04;
    CWorld*       World;                                         // +0x08
    unsigned char _pad0c[0x20 - 0x0c];
    CMapInfoArray Maps;                                          // +0x20
    CNavigationSection FindNavigationSection(CMemoryFile& file, const CCharStringSet& sections);   // 0x004FE630
    void CreateNavMap(long map, const CCharStringSet& sections, CAFile& file, CASuspendableProcess& process);
};

void CWorldMap::CreateNavMap(long map, const CCharStringSet& sections, CAFile& file, CASuspendableProcess& process)
{
    file.Seek(0);
    CFileFormatLevel level;
    level.ReadFromFile(&file, &process);
    CNavigatorManager* navigator = World->NavigatorManager;
    if (level.NavigationSectionsOffset != 0) {
        file.Seek(level.NavigationSectionsOffset);
        int sectionsEnd;
        file.ReadVar(&sectionsEnd, &process);
        CMemoryFile sectionTable;
        sectionTable.OpenFromFile(&file, sectionsEnd - file.GetPosition(), &process);
        CNavigationSection section = FindNavigationSection(sectionTable, sections);
        if (section.Offset > 0 && !navigator->IsNavigationMapLoadedWithSectionName(map, section.Name)) {
            if (navigator->IsNavigationMapLoaded(map))
                navigator->RemoveNavigationMap(map);
            file.Seek(section.Offset);
            navigator->AddNavigationMap(map, Maps.Start[map].Area, section.Name, file, &process);
        }
    }
    else if (level.NavigationDataOffset != 0 && !navigator->IsNavigationMapLoaded(map)) {
        file.Seek(level.NavigationDataOffset);
        navigator->AddNavigationMap(map, Maps.Start[map].Area, "", file, &process);
    }
}