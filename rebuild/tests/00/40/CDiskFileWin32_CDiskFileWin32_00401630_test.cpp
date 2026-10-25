#include <cstdio>

static int g_order[8];
static int g_n = 0;

class CWideString {
public:
    ~CWideString() { g_order[g_n++] = 2; }
    void* PStringData;
};

class CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable() { }
    virtual ~CBaseClassNonCopyable() { g_order[g_n++] = 3; }
};

enum FileAccessType { FAT_READ };

class __declspec(novtable) CAFile : public CBaseClassNonCopyable {
public:
    virtual ~CAFile() { }
    virtual void Close() = 0;
};

class CDiskFileWin32 : public CAFile {
public:
    CDiskFileWin32() : AccessType(FAT_READ), Position(0), Length(0), WinFileHandle(0), Opened(true) { Pathname.PStringData = 0; }
    virtual ~CDiskFileWin32();
    virtual void Close() { g_order[g_n++] = 1; Opened = false; }

    FileAccessType AccessType;
    long           Position;
    long           Length;
    void*          WinFileHandle;
    bool           Opened;
    CWideString    Pathname;
};

CDiskFileWin32::~CDiskFileWin32()
{
    Close();
}

int main()
{
    CAFile* f = new CDiskFileWin32;
    delete f;
    int ok = g_n == 3 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 3
          && sizeof(CDiskFileWin32) == 0x1C;
    std::printf(ok ? "00401630_TEST PASS\n" : "00401630_TEST FAIL\n");
    return ok ? 0 : 1;
}
