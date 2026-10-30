// CWorld::Load(const CWideString&) -- load a world: remember its name, open the world file (critical message on failure), load level and global quests, set up the startup WAD, read the world, drain the map loader, generate offline data and set the engine's static map
// Grade: DIFFER(1129v1129) -- same length and the same frame-slot order (sub esp 0xA4); three lea/push register picks differ:
// retail `lea ecx,[ebp-50h] ; push ecx` (GetOfflineData arg), `mov eax/ecx/ecx/edx + lea eax` (SetStaticMap) and
// `lea ecx,[ebp-20h] ; push ecx` (Format this) where we pick eax/edx. Tried /O1 /Os /G5 /G6 /G7 /Oy-, the QFE toolset,
// an inline GetEngine() getter vs member chain vs named engine local. Sent to advisor AdvB3.
// Leaf: Load
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A1840, 1129 bytes. EH FuncInfo 0x01369298: one try block, catch
// (CBBBFileException&) [type 0x01378A58, object at ebp-0xB0 -- naming the reference gives retail's sub esp 0xA4], and
// no destructor unwind states (built without /EHsc). Named locals for L"" / the startup-WAD map pointer / the offline
// file name reproduce retail's frame-slot order exactly.
#include "engine/CCountedPointer.h"
#include <wchar.h>
struct CCharString {
    void* PStringData;
    CCharString();                                       // 0x0099E4B0
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
    CCharString& operator=(const CCharString& other);    // 0x0099EFB0
    operator const char*() const;                        // 0x0099E4C0
    void Format(const char* format, ...);                // 0x0099F1F0
};
struct CWideString {
    void* PStringData;
    CWideString();                                       // 0x0099AED0
    CWideString(const wchar_t* text);                    // 0x0099B6B0
    ~CWideString();                                      // 0x0099B510
    CCharString ConvertToCharString() const;             // 0x0099B2C0
    operator const char*() const;                        // 0x0099AF10
};
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* dir,
                          CWideString* name, CWideString* extension);   // 0x00997620
bool __fastcall PathExists(const CWideString& path);   // 0x00999230
void __fastcall LoadingStage(const CCharString& stage, long level);     // 0x009D8240
void __fastcall LoadingStageDone(long level);                           // 0x009D8250
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
CWideString __fastcall GetLevelQuestFilename(const CWideString& world);   // 0x0049D770

struct CSystemManager {
    void DisplayCriticalMessage(const char* message);   // 0x009A4ED0
};
CSystemManager* __fastcall GFGetSystemManager();        // 0x009A4EC0

struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                       // 0x0099A2F0
    ~CBaseClassNonCopyable();                      // 0x0099A300
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
struct CDiskFileWin32 : CBaseClassNonCopyable {   // vtable 0x0122D06C
    long        AccessType;                        // +0x04
    long        Position;                          // +0x08
    long        Length;                            // +0x0c
    void*       WinFileHandle;                     // +0x10
    bool        Opened;                            // +0x14
    CWideString Pathname;                          // +0x18
    CDiskFileWin32() { Opened = false; }
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099A6A0
    virtual void Close();                          // 0x0099A920
};

void __cdecl operator delete(void* p);             // 0x00BFE9BC
struct CMapLoader {
    virtual void v00();
    virtual void Update();                         // slot 1
    bool IsBusy();                                 // 0x006C20A0
};

struct CWorldMap {
    unsigned char _pad00[0xbc];
    CCountedPointer<CMapLoader> PLoader;           // +0xbc
    void SetAsUsingWAD(const CWideString& wad);    // 0x004FDAB0
    void CreateWorldLevelWad(const CWideString& world, const CWideString& wad);   // 0x00502170
    CCountedPointer<CMapLoader> GetLoader() { return PLoader; }
};
struct COfflineData {
    void Generate();                               // 0x004FB320
};
struct CEngineMapOwner {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void SetStaticMap(const CWideString& file);   // slot 52 (+0xD0)
};
struct CDisplayEngine {
    unsigned char _pad00[0x2c];
    CEngineMapOwner* PEngine;                      // +0x2c
};
struct CMainGameComponent {
    unsigned char _pad00[0x28];
    CDisplayEngine* PDisplayEngine;                // +0x28
};
struct CAFile;

struct CExceptionBase { };
enum EFileExceptionType { BBB_FILE_ERROR_NONE };
struct CBBBFileException : CExceptionBase {      // catch type 0x01378A58 (.?AVCBBBFileException@@), by reference
    EFileExceptionType Type;                       // +0
};

extern bool g_CreateStartupWad;                    // 0x01375456
extern bool g_GenerateOfflineData;                 // 0x01375446

class CWorld {
public:
    virtual void v00();
    virtual void v01();
    virtual void LoadFromFile(CDiskFileWin32& file, long version);   // slot 2 (+8)
    virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual COfflineData* GetOfflineData(const CWideString& file, CEngineMapOwner* engine);   // slot 12 (+0x30)

    unsigned char       _base_0x04[4];             // +0x04
    CMainGameComponent* Component;                 // +0x08
    unsigned char       _pad0c[0x14 - 0x0c];
    CWorldMap*          PWorldMap;                 // +0x14
    unsigned char       _pad18[0x84 - 0x18];
    CCharString         CurrentWorldName;          // +0x84

    CWideString GetStaticMapFilename(const CWideString& world);   // 0x0049DDD0
    CWideString GetWadFilename(const CWideString& world);         // 0x0049D610
    void LoadQuestsFromFile(CDiskFileWin32* file, bool levelQuests);   // 0x004A0D90
    CEngineMapOwner* GetEngine() const { return Component->PDisplayEngine->PEngine; }
    void Load(const CWideString& filename);
};

void CWorld::Load(const CWideString& filename)
{
    CWideString name;
    SplitPath(filename, 0, 0, &name, 0);
    CurrentWorldName = name.ConvertToCharString();
    CDiskFileWin32 file;
    try {
        file.Open(filename, FILE_ACCESS_READ, 2);
    }
    catch (CBBBFileException& exception) {
        CCharString message;
        message.Format("Unable to load world %s", (const char*)filename);
        GFGetSystemManager()->DisplayCriticalMessage(message);
        return;
    }
    LoadingStage("Load Quests", 0);
    CWideString staticMap = GetStaticMapFilename(filename);
    CWideString levelQuests = GetLevelQuestFilename(filename);
    if (PathExists(levelQuests)) {
        CDiskFileWin32 questFile(levelQuests, FILE_ACCESS_READ, 2);
        LoadQuestsFromFile(&questFile, true);
    }
    CWideString globalQuests(L"Data\\Levels\\GlobalQuests.qst");
    if (PathExists(globalQuests)) {
        CDiskFileWin32 questFile(globalQuests, FILE_ACCESS_READ, 2);
        LoadQuestsFromFile(&questFile, false);
    }
    {
        CWideString noWad(L"");
        PWorldMap->SetAsUsingWAD(noWad);
    }
    LoadingStage("Startup Wad", 0);
    DisplayProgress("Startup WAD", -1.0f, false, false);
    if (g_CreateStartupWad) {
        CWideString wad = GetWadFilename(filename);
        PWorldMap->CreateWorldLevelWad(filename, wad);
        CWorldMap* map = PWorldMap;
        map->SetAsUsingWAD(GetWadFilename(filename));
    }
    LoadingStageDone(0);
    LoadFromFile(file, -1);
    while (PWorldMap->GetLoader()->IsBusy())
        PWorldMap->GetLoader()->Update();
    LoadingStage("Generate Offline Data", 0);
    DisplayProgress("Generate Offline Data", -1.0f, false, false);
    if (g_GenerateOfflineData) {
        CWideString offlineName = GetStaticMapFilename(filename);
        GetOfflineData(offlineName, GetEngine())->Generate();
    }
    LoadingStage("Set Static Map for Engine", 0);
    DisplayProgress("Set Static Map for Engine", -1.0f, false, false);
    GetEngine()->SetStaticMap(staticMap);
    DisplayProgress("Set Static Map for Engine: End", -1.0f, false, false);
    LoadingStageDone(0);
}
