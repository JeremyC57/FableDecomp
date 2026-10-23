// Grade: DIFFER(561v559) -- register allocation: retail keeps this in edi and the map offset in esi, reuses ebx (the temp-destruction flags) for levelFile.Info across the three by-value CCountedPointer copies and keeps the Maps base in ebp across the LevStamp call; ours swaps esi/edi and caches thingFile.Info in ebx / levelFile.Info in ebp, so the base is reloaded
// CWorldMap::LoadLevel(long map, ELoadType loadType) -- open the level file and its things file (WAD bank or disk), load them through LoadLevelFromFiles and record both files' modification stamps in the map info
// Leaf: ?LoadLevel@CWorldMap@@IAEXJW4ELoadType@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00502620, `ret 8`.
// Callees: CCharString::ToWideString 0x0099E480, GetThingFilenameFromLevelFilename 0x004FAFF0 (`__fastcall`, CWideString
//   by hidden pointer), ~CWideString 0x0099B510, operator new 0x00BFEA1A, CWADFile ctor 0x00A39D80, CDiskFileWin32 ctor
//   0x0099AD80, CCountedPointer<CAFile>::Reset 0x0050F550 (same-TU stand-in), CWorldMap::LoadLevelFromFiles 0x00501150,
//   GetFileModifiedTime 0x00999320 (`__fastcall`, CDateAndTime in edx:eax), operator delete 0x00BFE9BC.
// Layout: the landed GetLevelDimensions 0x00506E30 model -- LevelWADBank +0xA0, UseWADFiles +0xA8; Maps +0x20 (CMapInfo
//   0x48 bytes: LevelName +0x18, LevStamp +0x30, TngStamp +0x38).
struct CWideString {
    void* PStringData;
    ~CWideString();                                    // 0x0099B510
};
struct CCharString {
    void* PStringData;
    CWideString ToWideString() const;                  // 0x0099E480
};
struct CDateAndTime { unsigned long Low, High; };
CWideString __fastcall GetThingFilenameFromLevelFilename(const CWideString& levelFileName);   // 0x004FAFF0
CDateAndTime __fastcall GetFileModifiedTime(const CWideString& fileName);                     // 0x00999320
enum FileAccessType { FILE_ACCESS_READ = 1 };
enum ELoadType { LOAD_TYPE_IMMEDIATE = 0, LOAD_TYPE_BACKGROUND = 1 };
class CAFile { public: virtual void v00(); };
struct CWADBank;
class CWADFile : public CAFile {
public:
    CWADFile(CWADBank* bank, const CWideString& name, FileAccessType access, unsigned long flags);   // 0x00A39D80
    unsigned char _body[0xc];
};
class CDiskFileWin32 : public CAFile {
public:
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);         // 0x0099AD80
    unsigned char _body[0x18];
};
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        CCPPointerInfo* info = Info;
        if (info) {
            if (--info->RefCount == 0) {
                info->DeleteFunc(info->Data);
                delete info;
            }
        }
    }
    void Reset(T* object);                             // 0x0050F550
    operator T*() const { return Data; }
};
CCPPointerInfo* AllocPointerInfo_00502620(void* object);
#pragma auto_inline(off)
template <class T> __declspec(noinline) void CCountedPointer<T>::Reset(T* object)
{
    CCPPointerInfo* info = Info;
    if (info) {
        if (--info->RefCount == 0) {
            info->DeleteFunc(info->Data);
            delete info;
        }
    }
    Data = object;
    Info = AllocPointerInfo_00502620(object);
}
#pragma auto_inline(on)

struct CMapInfo {                                      // 0x48 bytes
    unsigned char _pad00[0x18];
    CCharString   LevelName;                           // +0x18
    unsigned char _pad1c[0x30 - 0x1c];
    CDateAndTime  LevStamp;                            // +0x30
    CDateAndTime  TngStamp;                            // +0x38
    unsigned char _pad40[0x48 - 0x40];
};
struct CMapInfoArray {
    CMapInfo* _M_start;
    CMapInfo* _M_finish;
    CMapInfo* _M_end_of_storage;
    CMapInfo& operator[](long n) { return _M_start[n]; }
};
class CWorldMap {
public:
    virtual void v00();
    unsigned char             _pad04[0x20 - 0x04];
    CMapInfoArray             Maps;                    // +0x20
    unsigned char             _pad2c[0xa0 - 0x2c];
    CCountedPointer<CWADBank> LevelWADBank;            // +0xA0
    bool                      UseWADFiles;             // +0xA8
    void LoadLevelFromFiles(long map, CCountedPointer<CAFile> levelFile, CCountedPointer<CAFile> topologyFile,
                            CCountedPointer<CAFile> thingFile, ELoadType loadType);   // 0x00501150
    void LoadLevel(long map, ELoadType loadType);
};

void CWorldMap::LoadLevel(long map, ELoadType loadType)
{
    CWideString thingFileName = GetThingFilenameFromLevelFilename(Maps[map].LevelName.ToWideString());
    CCountedPointer<CAFile> levelFile;
    CCountedPointer<CAFile> thingFile;
    if (UseWADFiles) {
        levelFile.Reset(new CWADFile(LevelWADBank, Maps[map].LevelName.ToWideString(), FILE_ACCESS_READ, 2));
        thingFile.Reset(new CWADFile(LevelWADBank, thingFileName, FILE_ACCESS_READ, 2));
    } else {
        levelFile.Reset(new CDiskFileWin32(Maps[map].LevelName.ToWideString(), FILE_ACCESS_READ, 2));
        thingFile.Reset(new CDiskFileWin32(thingFileName, FILE_ACCESS_READ, 2));
    }
    LoadLevelFromFiles(map, levelFile, levelFile, thingFile, loadType);
    Maps[map].LevStamp = GetFileModifiedTime(Maps[map].LevelName.ToWideString());
    Maps[map].TngStamp = GetFileModifiedTime(thingFileName);
}
