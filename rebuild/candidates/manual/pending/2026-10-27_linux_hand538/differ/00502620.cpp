// CWorldMap::LoadLevel(long map, ELoadType loadType) -- open the level file and its things file (from the level WAD bank or from disk), load them through LoadLevelFromFiles and record both files' modification stamps in the map info
// Grade: DIFFER(561v559) -- register allocation: the temp-destruction flag now sits in ebx as in retail, but retail keeps the map offset in esi, this in edi and the new result in ebp; ours ebp/esi/edi, and the 'mapInfo' reference adds one add per Maps access
// Leaf: ?LoadLevel@CWorldMap@@IAEXJW4ELoadType@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00502620, 559 bytes, `ret 8`. FableWin.pdb ?LoadLevel@CWorldMap@@IAEXJW4ELoadType@1@@Z.
// Callees: CCharString::ToWideString 0x0099E480, static __fastcall CWorldMap::GetThingFilenameFromLevelFilename 0x004FAFF0,
//   ~CWideString 0x0099B510, operator new 0x00BFEA1A, CWADFile(CWADBank&, ...) 0x00A39D80, CDiskFileWin32 0x0099AD80,
//   CCountedPointer<CAFile>::Reset 0x0050F550 (same-TU body), CWorldMap::LoadLevelFromFiles 0x00501150,
//   static __fastcall CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber 0x00999320 (unsigned __int64 in edx:eax), operator delete 0x00BFE9BC.
// Layout: retail CWorldMap Maps +0x20 (CMapInfo 0x48 bytes: LevelName +0x18, LevStamp +0x30, TngStamp +0x38),
//   LevelWADBank +0xA0, UseLevelWADBank +0xA8 (engine/CWorldMap.h offsets).
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                    // 0x0099B510
};
class CCharString {
public:
    void* PStringData;
    CWideString ToWideString() const;                  // 0x0099E480
};
enum FileAccessType { FILE_ACCESS_READ = 1 };
class CAFile {
public:
    virtual ~CAFile();
    static unsigned __int64 __fastcall GetFileModifiedTimeAsSomeWackyWin32BigNumber(const CWideString& filename);   // 0x00999320
};
class CWADBank;
class CWADFile : public CAFile {
public:
    CWADFile(CWADBank& bank, const CWideString& filename, FileAccessType access, unsigned long flags);   // 0x00A39D80
    unsigned char _body[0xc];
};
class CDiskFileWin32 : public CAFile {
public:
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    unsigned char _body[0x18];
};
class CCPPointerInfo {
public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
    CCPPointerInfo(unsigned long refCount, void (__fastcall* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { ReleaseData(); }
    T& operator*() const { return *Data; }
    void Reset(T* data);                               // 0x0050F550
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    static void __fastcall DeleteData(void* data);     // 0x0050BE40 (folded deleter body)
};
// Same-TU body of 0x0050F550 so VC7.1 sees Reset keeps the pointer's address to itself.
#pragma auto_inline(off)
template <class T> __declspec(noinline) void CCountedPointer<T>::Reset(T* data)
{
    ReleaseData();
    Data = data;
    if (data)
        Info = new CCPPointerInfo(1, DeleteData, Data);
}
#pragma auto_inline(on)

class CMapInfo {                                       // 0x48 bytes
public:
    unsigned char _pad00[0x18];
    CCharString   LevelName;                           // +0x18
    unsigned char _pad1c[0x30 - 0x1c];
    unsigned __int64 LevStamp;                         // +0x30
    unsigned __int64 TngStamp;                         // +0x38
    unsigned char _pad40[0x48 - 0x40];
};
template <class T> class CArray {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](long n) { return _M_start[n]; }
};
class CWorldMap {
public:
    enum ELoadType { LOAD_TYPE_IMMEDIATE = 0 };
    virtual ~CWorldMap();
    unsigned char             _pad04[0x20 - 0x04];
    CArray<CMapInfo>          Maps;                    // +0x20
    unsigned char             _pad2c[0xa0 - 0x2c];
    CCountedPointer<CWADBank> LevelWADBank;            // +0xA0
    bool                      UseLevelWADBank;         // +0xA8
    static CWideString __fastcall GetThingFilenameFromLevelFilename(const CWideString& levelFilename);   // 0x004FAFF0
protected:
    void LoadLevelFromFiles(long map, CCountedPointer<CAFile> levelFile, CCountedPointer<CAFile> topologyFile,
                            CCountedPointer<CAFile> thingFile, ELoadType loadType);   // 0x00501150
    void LoadLevel(long map, ELoadType loadType);
};

void CWorldMap::LoadLevel(long map, ELoadType loadType)
{
    CMapInfo& mapInfo = Maps[map];
    CWideString thingFilename = GetThingFilenameFromLevelFilename(mapInfo.LevelName.ToWideString());
    CCountedPointer<CAFile> levelFile;
    CCountedPointer<CAFile> thingFile;
    if (UseLevelWADBank) {
        levelFile.Reset(new CWADFile(*LevelWADBank, Maps[map].LevelName.ToWideString(), FILE_ACCESS_READ, 2));
        thingFile.Reset(new CWADFile(*LevelWADBank, thingFilename, FILE_ACCESS_READ, 2));
    } else {
        levelFile.Reset(new CDiskFileWin32(Maps[map].LevelName.ToWideString(), FILE_ACCESS_READ, 2));
        thingFile.Reset(new CDiskFileWin32(thingFilename, FILE_ACCESS_READ, 2));
    }
    LoadLevelFromFiles(map, levelFile, levelFile, thingFile, loadType);
    Maps[map].LevStamp = CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber(Maps[map].LevelName.ToWideString());
    Maps[map].TngStamp = CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber(thingFilename);
}
