// 0098e1e0 behaviour test: CThreadedFile::Open (match.cpp body, host-modelled string and Win32 stand-ins)
#include <stdio.h>
#include <string.h>
#include <string>
#include <ctype.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)

class CCharString { public:
    std::string* PStringData;                       // null = empty string, as in the engine
    CCharString() : PStringData(0) {}
    CCharString(const std::string& s) : PStringData(s.empty() ? 0 : new std::string(s)) {}
    CCharString(const CCharString& o) : PStringData(o.PStringData ? new std::string(*o.PStringData) : 0) {}
    ~CCharString() { delete PStringData; }
    CCharString ToUpper() const { std::string s = PStringData ? *PStringData : ""; for (size_t i = 0; i < s.size(); ++i) s[i] = (char)toupper(s[i]); return CCharString(s); }
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return *PStringData == text;
    }
};
class CWideString { public:
    std::string s;
    CWideString() {}
    CWideString(const char* t) : s(t) {}
    const char* c() const { return s.c_str(); }
    CCharString ConvertToCharString() const { return CCharString(s); }
};
static CWideString ConvertPathToFullPath(const CWideString& path)
{
    if (path.s.size() >= 2 && path.s[1] == ':') return path;
    return CWideString((std::string("c:\\game\\") + path.s).c_str());
}
static void SplitPath(const CWideString& path, CWideString* drive, CWideString*, CWideString*, CWideString*)
{
    drive->s = (path.s.size() >= 2 && path.s[1] == ':') ? path.s.substr(0, 2) : std::string();
}
static std::string g_lastName; static unsigned long g_lastAccess, g_lastShare, g_lastDisp, g_lastFlags; static bool g_createFails;
static void* CreateFileW(const char* name, unsigned long access, unsigned long share, void*, unsigned long disposition, unsigned long flags, void*)
{
    g_lastName = name; g_lastAccess = access; g_lastShare = share; g_lastDisp = disposition; g_lastFlags = flags;
    return g_createFails ? (void*)-1 : (void*)0x1234;
}
static unsigned long GetFileSize(void* file, unsigned long* high) { return file == (void*)0x1234 && high == 0 ? 4096 : 0; }

static unsigned long g_ThreadedFilePhysicalSortKey;
enum EThreadedFileDeviceID { THREADED_FILE_DEVICE_HD = 0, THREADED_FILE_DEVICE_D = 1 };
class CThreadedFile { public:
    EThreadedFileDeviceID DeviceID; void* FileHandle; CWideString Filename;
    unsigned long Length, PhysicalSortKey; bool OpenedForWrite, OpenFlag;
    CThreadedFile() : DeviceID(THREADED_FILE_DEVICE_HD), FileHandle(0), Length(0), PhysicalSortKey(0), OpenedForWrite(true), OpenFlag(false) {}
    bool Open(const CWideString& filename, bool unbuffered);
};

// ---- match.cpp body (wide strings modelled as narrow) ----
bool CThreadedFile::Open(const CWideString& filename, bool unbuffered)
{
    CWideString fullPath = ConvertPathToFullPath(filename);
    CWideString drive;
    SplitPath(fullPath, &drive, 0, 0, 0);
    OpenedForWrite = false;
    bool onDriveD = drive.ConvertToCharString().ToUpper() == "D:";
    DeviceID = (EThreadedFileDeviceID)(onDriveD != 0);
    unsigned long flags = 0x40000001;
    if (unbuffered)
        flags = 0x60000001;
    FileHandle = CreateFileW(fullPath.c(), 0x80000000, 1, 0, 3, flags, 0);
    if (FileHandle == (void*)-1)
        return false;
    Filename = filename;
    Length = GetFileSize(FileHandle, 0);
    PhysicalSortKey = ++g_ThreadedFilePhysicalSortKey;
    OpenFlag = true;
    return true;
}

int main()
{
    CThreadedFile a;
    CHECK(a.Open(CWideString("d:\\data\\x.big"), false));
    CHECK(a.DeviceID == THREADED_FILE_DEVICE_D && a.OpenedForWrite == false && a.OpenFlag);
    CHECK(g_lastFlags == 0x40000001 && g_lastAccess == 0x80000000 && g_lastShare == 1 && g_lastDisp == 3);
    CHECK(a.Length == 4096 && a.PhysicalSortKey == 1 && a.Filename.s == "d:\\data\\x.big");
    CThreadedFile b;
    CHECK(b.Open(CWideString("x.big"), true));
    CHECK(b.DeviceID == THREADED_FILE_DEVICE_HD && g_lastFlags == 0x60000001 && g_lastName == "c:\\game\\x.big");
    CHECK(b.PhysicalSortKey == 2 && b.Filename.s == "x.big");
    CThreadedFile c; g_createFails = true;
    CHECK(!c.Open(CWideString("D:\\a"), false));
    CHECK(c.DeviceID == THREADED_FILE_DEVICE_D && !c.OpenFlag && c.Filename.s.empty() && g_ThreadedFilePhysicalSortKey == 2);
    if (!g_fail) printf("0098e1e0_TEST PASS\n");
    return g_fail;
}