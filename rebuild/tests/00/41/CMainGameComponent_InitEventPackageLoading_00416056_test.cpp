#include <cstdio>
#include <stddef.h>
class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    CWideString(const wchar_t* text);                    // 0x0099B6B0
    ~CWideString();                                      // 0x0099B510
};
class CASuspendableProcess;
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };   // values as pushed by the two Init twins
class CAFile {
public:
    virtual ~CAFile();
    virtual void Open(const CWideString& name, FileAccessType access, unsigned long flags) = 0;
    virtual void Close() = 0;                                                    // slot 2
    virtual void Read(void* dst, long size, CASuspendableProcess* process) = 0;
    virtual void Write(const void* src, long size, CASuspendableProcess* process) = 0;
    virtual void SetPosition(long pos) = 0;
    virtual void MovePosition(long delta) = 0;
    virtual long GetPosition() const = 0;
    virtual void SetLength(long length) = 0;
    virtual long GetLength() const = 0;
    virtual bool IsOpen() const = 0;                                             // slot 10
};
class CDiskFileWin32 : public CAFile {
public:
    virtual ~CDiskFileWin32();
    virtual void Open(const CWideString& name, FileAccessType access, unsigned long flags);
    virtual void Close();
    virtual void Read(void* dst, long size, CASuspendableProcess* process);
    virtual void Write(const void* src, long size, CASuspendableProcess* process);
    virtual void SetPosition(long pos);
    virtual void MovePosition(long delta);
    virtual long GetPosition() const;
    virtual void SetLength(long length);
    virtual long GetLength() const;
    virtual bool IsOpen() const;
    unsigned char _body[0x18];                                                   // +0x04
};
class CEventPackageFileHeader {
public:
    long NoPlayers;                                      // +0x00
};
class CMainGameComponent {
protected:
    bool InitEventPackageLoading(const wchar_t* file_name);
    bool InitEventPackageSaving(const wchar_t* file_name);
public:
    unsigned char           _pad0[0x16154];
    CDiskFileWin32          EventPackageFile;            // +0x16154
    bool                    LoadingEventPackages;        // +0x16170
    bool                    SavingEventPackages;         // +0x16171
    CEventPackageFileHeader EventPackageFileHeader;      // +0x16174
};
bool CMainGameComponent::InitEventPackageLoading(const wchar_t* file_name)
{
    EventPackageFile.Open(file_name, FILE_ACCESS_READ, 2);
    EventPackageFile.Read(&EventPackageFileHeader, sizeof(EventPackageFileHeader), 0);
    LoadingEventPackages = true;
    return true;
}

#include <new>
static int g_open_access, g_open_flags, g_strings_alive, g_strings_made, g_read, g_write, g_io_size;
static const wchar_t* g_open_text;
static long g_written_players;
CWideString::CWideString(const wchar_t* text) { PStringData = (CWideStringData*)text; ++g_strings_alive; ++g_strings_made; }
CWideString::~CWideString() { --g_strings_alive; }
CAFile::~CAFile() {}
CDiskFileWin32::~CDiskFileWin32() {}
void CDiskFileWin32::Open(const CWideString& name, FileAccessType access, unsigned long flags) { g_open_text = (const wchar_t*)name.PStringData; g_open_access = access; g_open_flags = (int)flags; }
void CDiskFileWin32::Close() {}
void CDiskFileWin32::Read(void* dst, long size, CASuspendableProcess*) { ++g_read; g_io_size = size; *(long*)dst = 3; }
void CDiskFileWin32::Write(const void* src, long size, CASuspendableProcess*) { ++g_write; g_io_size = size; g_written_players = *(const long*)src; }
void CDiskFileWin32::SetPosition(long) {}
void CDiskFileWin32::MovePosition(long) {}
long CDiskFileWin32::GetPosition() const { return 0; }
void CDiskFileWin32::SetLength(long) {}
long CDiskFileWin32::GetLength() const { return 0; }
bool CDiskFileWin32::IsOpen() const { return false; }
struct CMainGameComponent_Test : CMainGameComponent {
    bool Run(const wchar_t* n) { return InitEventPackageLoading(n); }
};
int main()
{
    CMainGameComponent_Test* c = (CMainGameComponent_Test*)new unsigned char[sizeof(CMainGameComponent)];
    new (&c->EventPackageFile) CDiskFileWin32;
    c->LoadingEventPackages = false; c->SavingEventPackages = false; c->EventPackageFileHeader.NoPlayers = 0;
    static const wchar_t name[] = L"events.pkg";
    bool r = c->Run(name);
    bool ok = r && g_open_text == name && g_open_access == 1 && g_open_flags == 2 && g_strings_made == 1 && g_strings_alive == 0
           && g_io_size == 4 && c->LoadingEventPackages == true && g_read == 1 && g_write == 0 && c->EventPackageFileHeader.NoPlayers == 3 && c->SavingEventPackages == false;
    ok = ok && (char*)&c->EventPackageFileHeader - (char*)c == 0x16174;
    std::printf(ok ? "00416056_TEST PASS\n" : "00416056_TEST FAIL\n");
    return ok ? 0 : 1;
}