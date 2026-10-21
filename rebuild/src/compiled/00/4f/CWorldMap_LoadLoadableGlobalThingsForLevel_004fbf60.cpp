// CWorldMap::LoadLoadableGlobalThingsForLevel(long) -- open the level's global-things file (from the level WAD bank when one is in use, else from disk) and hand it to the thing manager
// ToWideString call, the CDiskFileWin32 path push, the by-value copy's Data load and the World vtable load
// Retail: Fable.exe @ 0x004FBF60, 240 bytes. PDB ?LoadLoadableGlobalThingsForLevel@CWorldMap@@IAEXJ@Z (protected)
// Callees: CCharString::ToWideString() const 0x0099E480, static __fastcall
// CWorldMap::GetThingFilenameFromLevelFilename(const CWideString&) 0x004FAFF0, ~CWideString 0x0099B510,
// operator new 0x00BFEA1A, CWADFile::CWADFile(CWADBank&, const CWideString&, FileAccessType, unsigned long) 0x00A39D80,
// CDiskFileWin32::CDiskFileWin32(const CWideString&, FileAccessType, unsigned long) 0x0099AD80,
// CCountedPointer<CAFile>::Reset(CAFile*) 0x0050F550, CWorld virtual slot 3 GetThingManager (0x0049E1B0),
// CThingManager::LoadLoadableGlobalThingsFromFile(CCountedPointer<CAFile>, long) 0x005223F0 (manifest
// "LoadGlobalThingBatch"), operator delete 0x00BFE9BC.
// Retail CWorldMap offsets (12-byte containers): World +0x08, Maps (CArray<CMapInfo>, 0x48-byte elements, LevelName
// +0x18) +0x20, LevelWADBank {Data +0xA0, Info +0xA4} (PDB +0xC0), UseLevelWADBank +0xA8 (PDB +0xC8).
// Shape: the file pointer lives in its own block (so it shares the frame words of the ToWideString temporary);
// the thing-file path reuses the dead `level` argument slot; the file pointer is default-constructed and
// assigned through Reset (the two `new` arms merge into one Reset call); the by-value CCountedPointer argument is
// built in the argument area through the user copy constructor and the local's inline release keeps the count block
// in esi (`dec [esi] ; jne`). Register choice follows the container idioms: the map array's operator[] is the
// STLport `*(begin() + index)` through an inline begin() (direct `Start[index]` rotates eax/ecx/edx in the address
// and ToWideString setup), and the WAD bank is reached through CCountedPointer's inline `operator*` (`*LevelWADBank`;
// spelling out `.Data` rotates the registers in the CDiskFileWin32 arm, the by-value copy and the World vtable load).
struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    ~CWideString();                                          // 0x0099B510
};
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CWideString ToWideString() const;                        // 0x0099E480
};

struct CCPPointerInfo {
    long  Count;                                             // +0
    void (__fastcall* Destroy)(void* data);                  // +4
    void* Data;                                              // +8
};
template<class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->Count; }
    T& operator*() const { return *Data; }
    ~CCountedPointer()
    {
        if (Info && --Info->Count == 0)
        {
            Info->Destroy(Info->Data);
            delete Info;
        }
    }
    static void __fastcall DeleteData(void* data);           // 0x0050BE40 (CCountedPointer<CAFile>)
    void Reset(T* data);                                     // 0x0050F550 (CCountedPointer<CAFile>)
};

// The landed 0x0050F550 body, defined above the caller under auto_inline(off): VC7.1 then knows Reset is the only
// writer of the pointer, so the copy and the final release keep the count block in a register.
#pragma auto_inline(off)
template<class T> void CCountedPointer<T>::Reset(T* data)
{
    if (Info) {
        --Info->Count;
        if (Info->Count == 0) {
            Info->Destroy(Info->Data);
            delete Info;
        }
    }
    Info = 0;
    Data = data;
    if (data) {
        CCPPointerInfo* info = new CCPPointerInfo;
        info->Count = 1;
        info->Destroy = DeleteData;
        info->Data = Data;
        Info = info;
    }
}
#pragma auto_inline(on)

enum FileAccessType { FILE_ACCESS_READ = 1 };
struct CAFile { };
struct CWADBank;
struct CWADFile : CAFile {
    CWADFile(CWADBank& bank, const CWideString& name, FileAccessType access, unsigned long flags);   // 0x00A39D80
    unsigned char _body[0x10];
};
struct CDiskFileWin32 : CAFile {
    CDiskFileWin32(const CWideString& name, FileAccessType access, unsigned long flags);              // 0x0099AD80
    unsigned char _body[0x1c];
};

struct CThingManager {
    void LoadLoadableGlobalThingsFromFile(CCountedPointer<CAFile> file, long level);   // 0x005223F0
};
struct CWorld {
    virtual void _slot0();
    virtual void _slot1();
    virtual void _slot2();
    virtual CThingManager* GetThingManager();                // slot 3 (0x0049E1B0)
};

struct CMapInfo {                                            // 0x48 bytes
    unsigned char _pad00[0x18];
    CCharString   LevelName;                                 // +0x18
    unsigned char _pad1c[0x2c];
};
struct CMapInfoArray {                                       // CArray<CMapInfo> (12 bytes in retail)
    CMapInfo* Start;
    CMapInfo* Finish;
    CMapInfo* EndOfStorage;
    CMapInfo* begin() const { return Start; }
    CMapInfo& operator[](long index) { return *(begin() + index); }
};

struct CWorldMap {
    void*                     _vptr;                         // +0x00
    long                      f4;                            // +0x04
    CWorld*                   World;                         // +0x08
    unsigned char             _pad0c[0x14];
    CMapInfoArray             Maps;                          // +0x20
    unsigned char             _pad2c[0x74];
    CCountedPointer<CWADBank> LevelWADBank;                  // +0xA0 (PDB +0xC0)
    bool                      UseLevelWADBank;               // +0xA8 (PDB +0xC8)

    static CWideString __fastcall GetThingFilenameFromLevelFilename(const CWideString& level);   // 0x004FAFF0
    void LoadLoadableGlobalThingsForLevel(long level);
};

void CWorldMap::LoadLoadableGlobalThingsForLevel(long level)
{
    CWideString path = GetThingFilenameFromLevelFilename(Maps[level].LevelName.ToWideString());
    {
        CCountedPointer<CAFile> file;
        if (UseLevelWADBank)
            file.Reset(new CWADFile(*LevelWADBank, path, FILE_ACCESS_READ, 2));
        else
            file.Reset(new CDiskFileWin32(path, FILE_ACCESS_READ, 2));
        World->GetThingManager()->LoadLoadableGlobalThingsFromFile(file, level);
    }
}