// (the ToWideString stub returns a local, which VC7.1 copies without NRVO, so the wide-string destructor count is a lower bound)
// Behaviour test for CWorldMap::GetLevelDimensions(const CCharString&) (0x00506E30): model and body verbatim, the file
// constructors, string conversion, count-block allocator and the file overload stubbed; both the WAD and disk paths,
// and the counted pointer's release of the adopted file.
#include <stdio.h>
#include <stdlib.h>

class CWideString { public:
    void* PStringData;
    ~CWideString();                                    // 0x0099B510
};
class CCharString { public:
    void* PStringData;
    CWideString ToWideString() const;                  // 0x0099E480
};
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
enum FileAccessType { FILE_ACCESS_READ = 1 };
class CAFile { public: virtual void v00(); };
class CWADBank;
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
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
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
    T* Get() const { return Data; }
    operator T*() const { return Data; }
};
CCPPointerInfo* AllocPointerInfo_00506e30(void* object);   // stand-in callee of the stand-in body below
// Same-TU stand-in for CCountedPointer<CAFile>::Reset @ 0x0050F550 (a template member compiled in retail's TU): releases the
// old count block and adopts the object with a fresh one; it only writes the two members, so VC7.1 knows the pointer's
// address does not escape the call.
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
    Info = AllocPointerInfo_00506e30(object);
}
#pragma auto_inline(on)
class CWorldMap {
public:
    unsigned char _pad00[0xa0];
    CCountedPointer<CWADBank> LevelWADBank;              // +0xA0
    bool          UseWADFiles;                         // +0xA8
    C2DBoxI GetLevelDimensions(CAFile* file);          // 0x00506BB0 (landed, PDB takes CAFile&)
    C2DBoxI GetLevelDimensions(const CCharString& levelName);
};
C2DBoxI CWorldMap::GetLevelDimensions(const CCharString& levelName)
{
    CCountedPointer<CAFile> file;
    if (UseWADFiles)
        file.Reset(new CWADFile(LevelWADBank, levelName.ToWideString(), FILE_ACCESS_READ, 2));
    else
        file.Reset(new CDiskFileWin32(levelName.ToWideString(), FILE_ACCESS_READ, 2));
    return GetLevelDimensions(file.Data);
}

static int g_wadCtor = 0, g_diskCtor = 0, g_toWide = 0, g_wideDtor = 0, g_deleted = 0, g_infoFreed = 0;
static CAFile* g_lastFile = 0;
static CWADBank* g_lastBank = 0;
static C2DBoxI g_box = { 1, 2, 3, 4 };

CWideString::~CWideString() { ++g_wideDtor; }
CWideString CCharString::ToWideString() const { ++g_toWide; CWideString w; w.PStringData = PStringData; return w; }
void CAFile::v00() {}
CWADFile::CWADFile(CWADBank* bank, const CWideString& name, FileAccessType access, unsigned long flags) { ++g_wadCtor; g_lastBank = bank; }
CDiskFileWin32::CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags) { ++g_diskCtor; }
C2DBoxI CWorldMap::GetLevelDimensions(CAFile* file) { g_lastFile = file; return g_box; }
static void __fastcall DeleteFile_Stub(void* data) { ++g_deleted; free(data); }
CCPPointerInfo* AllocPointerInfo_00506e30(void* object)
{
    CCPPointerInfo* info = (CCPPointerInfo*)malloc(sizeof(CCPPointerInfo));
    info->RefCount = 1; info->DeleteFunc = DeleteFile_Stub; info->Data = object;
    return info;
}

static int fail(const char* why) { printf("FAIL: %s\n", why); return 1; }

int main()
{
    CWorldMap* map = (CWorldMap*)calloc(1, sizeof(CWorldMap));
    map->LevelWADBank.Data = (CWADBank*)0x5150;
    CCharString name; name.PStringData = (void*)0x77;

    map->UseWADFiles = true;
    C2DBoxI box = map->GetLevelDimensions(name);
    if (box.TLX != 1 || box.TLY != 2 || box.BRX != 3 || box.BRY != 4) return fail("box (wad)");
    if (g_wadCtor != 1 || g_diskCtor != 0 || g_lastBank != (CWADBank*)0x5150) return fail("wad ctor");
    if (g_toWide != 1 || g_wideDtor < 1) return fail("wide string temp (wad)");
    if (g_lastFile == 0 || g_deleted != 1) return fail("file adopted and released (wad)");

    map->UseWADFiles = false;
    box = map->GetLevelDimensions(name);
    if (box.BRY != 4) return fail("box (disk)");
    if (g_wadCtor != 1 || g_diskCtor != 1) return fail("disk ctor");
    if (g_toWide != 2 || g_wideDtor < 2 || g_deleted != 2) return fail("disk path temp / release");

    free(map);
    printf("00506e30_TEST PASS\n");
    return 0;
}