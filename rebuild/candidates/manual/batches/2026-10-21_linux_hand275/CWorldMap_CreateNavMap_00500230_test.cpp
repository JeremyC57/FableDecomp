// Behaviour test for retail 0x00500230: candidate source embedded verbatim, then a scripted level file, a recording
// navigator manager and a main covering the section-table path, the plain navigation block and the early outs.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

struct CASuspendableProcess;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);             // 0x0099EBF0
    CCharString(const CCharString& other);                       // 0x0099EC30
    ~CCharString();                                              // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString();                                               // 0x0099AED0
    ~CWideString();                                              // 0x0099B510
};
struct C2DBoxI { long TLX, TLY, BRX, BRY; };

struct CAFile {
    CAFile();                                                    // 0x0099A2F0
    virtual ~CAFile();                                           // slot 0 (body 0x0099A300)
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
struct CMemoryFile : CAFile {                                    // vtable 0x0122E0FC
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
struct CFileFormatLevel {
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

struct CNavigatorManager {
    bool IsNavigationMapLoadedWithSectionName(long map, const CCharString& name) const;   // 0x00A14EE0
    bool IsNavigationMapLoaded(long map) const;                  // 0x00A14E80
    void RemoveNavigationMap(long map);                          // 0x00A155D0
    void AddNavigationMap(long map, const C2DBoxI& area, const CCharString& section, CAFile& file,
                          CASuspendableProcess* process);        // 0x00A157C0
};
struct CWorld {
    unsigned char _pad[0x48];
    CNavigatorManager* NavigatorManager;                         // +0x48
};
struct CMapInfo {                                                // 0x48 bytes
    C2DBoxI       Area;                                          // +0x00
    unsigned char _rest[0x38];
};
struct CMapInfoArray { CMapInfo* Start; CMapInfo* Finish; CMapInfo* EndOfStorage; };

struct CWorldMap {
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

// ---- stubs ----
static int g_liveStrings = 0, g_liveWide = 0, g_liveFiles = 0, g_closeCalls = 0;
CCharString::CCharString(const char* text, long) { PStringData = (void*)text; ++g_liveStrings; }
CCharString::CCharString(const CCharString& other) { PStringData = other.PStringData; ++g_liveStrings; }
CCharString::~CCharString() { --g_liveStrings; }
CWideString::CWideString() { PStringData = 0; ++g_liveWide; }
CWideString::~CWideString() { --g_liveWide; }

static long g_seeks[8]; static int g_seekCount = 0;
static long g_position = 0;
static long g_level[2];                       // NavigationDataOffset, NavigationSectionsOffset
static int g_sectionsEnd = 0;
static long g_openLength = -1;
static const char* g_sectionName = "SECTION_A"; static long g_sectionOffset = 0;

CAFile::CAFile() { ++g_liveFiles; }
CAFile::~CAFile() { --g_liveFiles; }
void CAFile::Slot01() { } void CAFile::Slot02() { } void CAFile::Slot03() { } void CAFile::Slot04() { } void CAFile::Slot06() { }
void CAFile::Seek(long position) { if (g_seekCount < 8) g_seeks[g_seekCount++] = position; g_position = position; }
long CAFile::GetPosition() { return g_position + 4; }
void CAFile::ReadVar(int* value, CASuspendableProcess*) { *value = g_sectionsEnd; }
void CMemoryFile::Close() { ++g_closeCalls; }
void CMemoryFile::OpenFromFile(CAFile*, long length, CASuspendableProcess*) { g_openLength = length; }
void CFileFormatLevel::ReadFromFile(CAFile*, CASuspendableProcess*)
{
    NavigationDataOffset = g_level[0]; NavigationSectionsOffset = g_level[1];
}
CNavigationSection CWorldMap::FindNavigationSection(CMemoryFile&, const CCharStringSet&)
{
    CNavigationSection section = { CCharString(g_sectionName), g_sectionOffset };
    return section;
}

static bool g_loaded = false; static const char* g_loadedSection = 0;
static int g_removeCalls = 0, g_addCalls = 0;
static long g_addMap = -1; static const C2DBoxI* g_addArea = 0; static const char* g_addName = 0;
static CASuspendableProcess* g_addProcess = 0;
bool CNavigatorManager::IsNavigationMapLoadedWithSectionName(long, const CCharString& name) const
{
    return g_loaded && g_loadedSection && strcmp(g_loadedSection, (const char*)name.PStringData) == 0;
}
bool CNavigatorManager::IsNavigationMapLoaded(long) const { return g_loaded; }
void CNavigatorManager::RemoveNavigationMap(long) { ++g_removeCalls; g_loaded = false; }
void CNavigatorManager::AddNavigationMap(long map, const C2DBoxI& area, const CCharString& section, CAFile&,
                                         CASuspendableProcess* process)
{
    ++g_addCalls; g_addMap = map; g_addArea = &area; g_addName = (const char*)section.PStringData; g_addProcess = process;
}

static void Reset(long dataOffset, long sectionsOffset, bool loaded, const char* loadedSection)
{
    g_seekCount = 0; g_level[0] = dataOffset; g_level[1] = sectionsOffset; g_loaded = loaded; g_loadedSection = loadedSection;
    g_removeCalls = g_addCalls = g_closeCalls = 0; g_addMap = -1; g_addArea = 0; g_addName = 0; g_openLength = -1;
}

int main()
{
    int ok = 1;
    CNavigatorManager navigator;
    static CWorld world; world.NavigatorManager = &navigator;
    CMapInfo infos[3];
    CWorldMap worldMap; worldMap.World = &world;
    worldMap.Maps.Start = infos; worldMap.Maps.Finish = infos + 3; worldMap.Maps.EndOfStorage = infos + 3;
    CAFile file;
    CASuspendableProcess* process = (CASuspendableProcess*)&infos[0];
    const CCharStringSet& sections = *(const CCharStringSet*)&infos[1];

    // section table, nothing loaded: seek 0, seek table, seek section, add with the section name
    Reset(0x300, 0x500, false, 0); g_sectionsEnd = 0x600; g_sectionOffset = 0x700;
    worldMap.CreateNavMap(2, sections, file, *process);
    if (g_seekCount != 3 || g_seeks[0] != 0 || g_seeks[1] != 0x500 || g_seeks[2] != 0x700) ok = 0;
    if (g_openLength != 0x600 - (0x500 + 4) || g_closeCalls != 1) ok = 0;
    if (g_addCalls != 1 || g_addMap != 2 || g_addArea != &infos[2].Area || strcmp(g_addName, "SECTION_A") != 0) ok = 0;
    if (g_addProcess != process || g_removeCalls != 0) ok = 0;

    // section table, a different section loaded: removed then added
    Reset(0, 0x500, true, "OTHER");
    worldMap.CreateNavMap(1, sections, file, *process);
    if (g_removeCalls != 1 || g_addCalls != 1 || g_addMap != 1) ok = 0;

    // section table, same section already loaded: nothing
    Reset(0, 0x500, true, "SECTION_A");
    worldMap.CreateNavMap(1, sections, file, *process);
    if (g_removeCalls != 0 || g_addCalls != 0 || g_seekCount != 2) ok = 0;

    // section table without a usable section (offset 0): nothing
    Reset(0, 0x500, false, 0); g_sectionOffset = 0;
    worldMap.CreateNavMap(1, sections, file, *process);
    if (g_addCalls != 0) ok = 0;

    // no section table: plain navigation block when not loaded, with an empty section name
    Reset(0x300, 0, false, 0);
    worldMap.CreateNavMap(0, sections, file, *process);
    if (g_seekCount != 2 || g_seeks[1] != 0x300 || g_addCalls != 1 || g_addMap != 0 || strcmp(g_addName, "") != 0) ok = 0;
    if (g_closeCalls != 0) ok = 0;

    // no section table, already loaded: nothing
    Reset(0x300, 0, true, 0);
    worldMap.CreateNavMap(0, sections, file, *process);
    if (g_addCalls != 0 || g_seekCount != 1) ok = 0;

    // neither block: only the rewind
    Reset(0, 0, false, 0);
    worldMap.CreateNavMap(0, sections, file, *process);
    if (g_addCalls != 0 || g_seekCount != 1) ok = 0;

    if (g_liveStrings != 0 || g_liveWide != 0 || g_liveFiles != 1) ok = 0;
    printf(ok ? "00500230_TEST PASS\n" : "00500230_TEST FAIL\n");
    return ok ? 0 : 1;
}
