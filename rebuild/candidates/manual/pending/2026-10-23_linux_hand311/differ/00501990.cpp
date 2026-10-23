// Grade: DIFFER(936v920) -- first-pass model of the LoadThingsWithSectionNames sibling: retail keeps each new-expression result in a frame slot around the guarded ToWideString temporary (ours keeps it in ebp, 4-byte smaller frame) and the wait loop rotation differs as in 0x00500EB0
// CWorldMap::UpdateNavMaps(const CArray<CCharString>& sectionNames, long background) -- queue every loaded map's level file (from the level WAD bank or from disk) as the nav-map source of one level-loader object, with loading-progress ticks, and unless loading in the background pump the loader until it finishes
// Leaf: ?UpdateNavMaps@CWorldMap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00501990, 919 bytes (+ int3 padding), ret 8. Sibling of LoadThingsWithSectionNames
//   0x00500EB0 (same callees plus CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0 / __fastcall DisplayProgress 0x009E9F40;
//   the ToWideString temporaries live inside the new-expressions, so VC7.1 guards their destruction with flag bits).
// Layout: as 0x00500EB0; CLevelFiles {Map +0, LevFile +4, NavFile +0xC, ThingFile +0x14} (three counted pointers).
#include <stdlib.h>
void* operator new(unsigned int size);                 // 0x00BFEA1A
void operator delete(void* p);                         // 0x00BFE9BC

struct CWideString {
    void* PStringData;
    ~CWideString();                                    // 0x0099B510
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CWideString ToWideString() const;                  // 0x0099E480
};
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
CWideString __fastcall GetThingFilenameFromLevelFilename(const CWideString& levelFilename);   // 0x004FAFF0
enum FileAccessType { FILE_ACCESS_READ = 1 };
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
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        if (Info != other.Info) {
            if (Info) {
                if (--Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = other.Data;
            Info = other.Info;
            if (Info)
                ++Info->RefCount;
        }
        return *this;
    }
    void Reset(T* object);                             // 0x0050F550
};
CCPPointerInfo* AllocPointerInfo_00500eb0(void* object);
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
    Info = AllocPointerInfo_00500eb0(object);
}
#pragma auto_inline(on)

struct CLevelFiles {                                   // CLevelLoader::CObject::CLevelFiles, 0x1C bytes
    long                    Map;                       // +0x00
    CCountedPointer<CAFile> LevFile;                   // +0x04
    CCountedPointer<CAFile> NavFile;                   // +0x0C
    CCountedPointer<CAFile> ThingFile;                 // +0x14
    CLevelFiles() { }
    ~CLevelFiles();                                    // 0x0050F980
};
struct CLevelFilesVector {
    CLevelFiles* _M_start;
    CLevelFiles* _M_finish;
    CLevelFiles* _M_end_of_storage;
    CLevelFilesVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    static void DestroyRange(CLevelFiles* first, CLevelFiles* last)
    {
        for (; first != last; ++first)
            first->~CLevelFiles();
    }
    ~CLevelFilesVector()
    {
        DestroyRange(_M_start, _M_finish);
        if (_M_start) free(_M_start);
    }
    void push_back(const CLevelFiles& files);          // 0x0051AE60
    CLevelFiles& back() { return *(_M_finish - 1); }
};
template <class T> struct CArray { T* _M_start; T* _M_finish; T* _M_end_of_storage; };

struct SRbNode { char _M_color; SRbNode* _M_parent; SRbNode* _M_left; SRbNode* _M_right; long key; };
struct CLevelLoaderObject {                            // CLevelLoader::CObject, 0x20 bytes
    SRbNode* _M_header;                                // +0x00 map header
    long     _M_node_count;                            // +0x04
    long     _M_compare;                               // +0x08
    long     Files[3];                                 // +0x0C vector
    long     f18;                                      // +0x18
    long     f1c;
    CLevelLoaderObject()
    {
        _M_header = 0;
        _M_header = (SRbNode*)malloc(sizeof(SRbNode));
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
        Files[0] = 0; Files[1] = 0; Files[2] = 0;
        f18 = 0;
    }
    void SetFiles(const CLevelFilesVector& files, const CArray<CCharString>& sectionNames, bool flag);   // 0x006C27A0
};
void __fastcall DeleteLevelLoaderObject(void* object);   // 0x0051AF00
template <class T> struct CCountedObjectPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedObjectPointer(T* object) : Data(object)
    {
        if (object) {
            CCPPointerInfo* info = (CCPPointerInfo*)operator new(sizeof(CCPPointerInfo));
            if (info) {
                info->RefCount = 1;
                info->DeleteFunc = DeleteLevelLoaderObject;
                info->Data = Data;
                Info = info;
                return;
            }
        }
        Info = 0;
    }
    ~CCountedObjectPointer()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CLevelLoader {
    virtual void v00();
    virtual void Update();                             // slot 1 (+0x04)
    void AddObject(const CCountedObjectPointer<CLevelLoaderObject>& object);   // 0x006C2120
    bool IsLoading() const;                            // 0x006C20A0
};
struct CMapInfo {
    unsigned char _pad00[0x18];
    CCharString   LevelName;                           // +0x18
    unsigned char _pad1c[0x2c];
};

class CWorldMap {
public:
    unsigned char           _pad00[0x20];
    CArray<CMapInfo>        Maps;                      // +0x20
    unsigned char           _pad2c[0x64];
    CArray<long>            LoadedMaps;                // +0x90
    long                    LoadedRegion;              // +0x9C
    CCountedPointer<CWADBank> LevelWADBank;            // +0xA0
    bool                    UseLevelWADBank;           // +0xA8
    unsigned long           CurrentMapUIDCount;        // +0xAC
    CArray<long>            DestroyedThemePositions;   // +0xB0
    CLevelLoader*           PLevelLoader_Data;         // +0xBC
    CCPPointerInfo*         PLevelLoader_Info;         // +0xC0
    void UpdateNavMaps(const CArray<CCharString>& sectionNames, long background);
};

void CWorldMap::UpdateNavMaps(const CArray<CCharString>& sectionNames, long background)
{
    CLevelFilesVector files;
    for (long* it = LoadedMaps._M_start; it != LoadedMaps._M_finish; ++it)
    {
        {
            CCharString text("CWorldMap::UpdateNavMaps");
            DisplayProgress(text, -1.0f, false, false);
        }
        long map = *it;
        CCountedPointer<CAFile> file;
        if (UseLevelWADBank)
            file.Reset(new CWADFile(LevelWADBank.Data, Maps._M_start[map].LevelName.ToWideString(), FILE_ACCESS_READ, 2));
        else
            file.Reset(new CDiskFileWin32(Maps._M_start[map].LevelName.ToWideString(), FILE_ACCESS_READ, 2));
        files.push_back(CLevelFiles());
        files.back().Map = map;
        files.back().NavFile = file;
    }
    CCountedObjectPointer<CLevelLoaderObject> object(new CLevelLoaderObject);
    object.Data->SetFiles(files, sectionNames, false);
    {
        CCharString text("CWorldMap::UpdateNavMaps - SetAsLoading");
        DisplayProgress(text, -1.0f, false, false);
    }
    PLevelLoader_Data->AddObject(object);
    {
        CCharString text("CWorldMap::UpdateNavMaps - Add");
        DisplayProgress(text, -1.0f, false, false);
    }
    if (!background)
    {
        while (PLevelLoader_Data->IsLoading())
        {
            {
                CCharString text("CWorldMap::UpdateNavMaps - WAIT_FOR_LOAD");
                DisplayProgress(text, -1.0f, false, false);
            }
            PLevelLoader_Data->Update();
        }
    }
}
