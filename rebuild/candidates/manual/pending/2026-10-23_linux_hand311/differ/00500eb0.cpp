// Grade: DIFFER(684v670) -- the loader wait loop stays rotated in ours (retail tests IsLoading at the aligned loop top and jumps back), plus scratch-register rotation in the ToWideString/push sequence; tried while, for(;;) break, continue/break forms; frame layout matches after the CLevelFiles vector-member ctor and DestroyRange helper
// CWorldMap::LoadThingsWithSectionNames(const CArray<CCharString>& sectionNames, long background) -- queue every loaded map's thing file (from the level WAD bank or from disk) as one level-loader object restricted to the given sections, and unless loading in the background pump the loader until it finishes
// Leaf: ?LoadThingsWithSectionNames@CWorldMap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00500EB0, 668 bytes (+ int3 padding), ret 8.
// Callees: CCharString::ToWideString 0x0099E480 (manifest "GetDefaultVal<CAnimationSet>"), __fastcall
//   GetThingFilenameFromLevelFilename 0x004FAFF0, ~CWideString 0x0099B510, operator new 0x00BFEA1A, CWADFile ctor 0x00A39D80,
//   CDiskFileWin32 ctor 0x0099AD80, CCountedPointer<CAFile>::Reset 0x0050F550 (same-TU stand-in), vector<CLevelFiles>::push_back
//   0x0051AE60, ~CLevelFiles 0x0050F980, malloc 0x00BFEA0E, CLevelLoader::CObject::SetFiles 0x006C27A0 (manifest "AddChild"),
//   CLevelLoader::AddObject 0x006C2120, CLevelLoader::IsLoading 0x006C20A0 (manifest "IsActionPointFree"), loader slot 1
//   (+0x04), DeleteObject<CLevelLoader::CObject> 0x0051AF00 (__fastcall), operator delete 0x00BFE9BC, free 0x00BFEA14.
// Layout (retail CWorldMap, 12-byte containers): Maps +0x20 (0x48-byte CMapInfo, LevelName +0x18), LoadedMaps +0x90,
//   LevelWADBank +0xA0, UseLevelWADBank +0xA8, PLevelLoader +0xBC. CLevelFiles 0x1C bytes (Map +0, File counted pointer +0x14).
#include <stdlib.h>
void* operator new(unsigned int size);                 // 0x00BFEA1A
void operator delete(void* p);                         // 0x00BFE9BC

struct CWideString {
    void* PStringData;
    ~CWideString();                                    // 0x0099B510
};
struct CCharString {
    void* PStringData;
    CWideString ToWideString() const;                  // 0x0099E480
};
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

struct CArrayLong { long* a; long* b; long* c; CArrayLong() : a(0), b(0), c(0) {} };
struct CLevelFiles {                                   // CLevelLoader::CObject::CLevelFiles, 0x1C bytes
    long                    Map;                       // +0x00
    CArrayLong              Names;                     // +0x04 vector
    long                    f10;                       // +0x10
    CCountedPointer<CAFile> File;                      // +0x14
    CLevelFiles() : f10(0) { }
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
    void LoadThingsWithSectionNames(const CArray<CCharString>& sectionNames, long background);
};

void CWorldMap::LoadThingsWithSectionNames(const CArray<CCharString>& sectionNames, long background)
{
    CLevelFilesVector files;
    for (long* it = LoadedMaps._M_start; it != LoadedMaps._M_finish; ++it)
    {
        long map = *it;
        CWideString thingFilename = GetThingFilenameFromLevelFilename(Maps._M_start[map].LevelName.ToWideString());
        CCountedPointer<CAFile> file;
        if (UseLevelWADBank)
            file.Reset(new CWADFile(LevelWADBank.Data, thingFilename, FILE_ACCESS_READ, 2));
        else
            file.Reset(new CDiskFileWin32(thingFilename, FILE_ACCESS_READ, 2));
        files.push_back(CLevelFiles());
        files.back().Map = map;
        files.back().File = file;
    }
    CCountedObjectPointer<CLevelLoaderObject> object(new CLevelLoaderObject);
    object.Data->SetFiles(files, sectionNames, false);
    PLevelLoader_Data->AddObject(object);
    if (!background)
    {
        for (;;)
        {
            if (!PLevelLoader_Data->IsLoading())
                break;
            PLevelLoader_Data->Update();
        }
    }
}
