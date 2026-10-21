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
class CScratchBuffer {
public:
    bool Lock(void** buffer);                            // 0x009F4980
    void Unlock();                                       // 0x009F49A0
};
extern CScratchBuffer g_ScratchBuffer;                   // 0x013CB498
class CGameEventPackageSet {
public:
    void InitFromCompressedBuffer(const unsigned char* buffer);   // 0x009F1AC0
};
class CEventPackageFileHeader {
public:
    long NoPlayers;                                      // +0x00
};
class CMainGameComponent {
protected:
    bool InitEventPackageLoading(const wchar_t* file_name);
    bool InitEventPackageSaving(const wchar_t* file_name);
    bool GetEventPackageSetFromSave(CGameEventPackageSet* set);
public:
    unsigned char           _pad0[0x16154];
    CDiskFileWin32          EventPackageFile;            // +0x16154
    bool                    LoadingEventPackages;        // +0x16170
    bool                    SavingEventPackages;         // +0x16171
    CEventPackageFileHeader EventPackageFileHeader;      // +0x16174
};
bool CMainGameComponent::GetEventPackageSetFromSave(CGameEventPackageSet* set)
{
    void* buffer;
    if (g_ScratchBuffer.Lock(&buffer))
    {
        long size;
        EventPackageFile.Read(&size, 4, NULL);
        EventPackageFile.Read(buffer, size, NULL);
        set->InitFromCompressedBuffer((const unsigned char*)buffer);
        g_ScratchBuffer.Unlock();
        return true;
    }
    return false;
}

#include <new>
static int g_write, g_sizes[4]; static long g_firstLong; static const void* g_srcs[4];
CAFile::~CAFile() {}
CDiskFileWin32::~CDiskFileWin32() {}
void CDiskFileWin32::Open(const CWideString&, FileAccessType, unsigned long) {}
void CDiskFileWin32::Close() {}
static int g_read, g_rsizes[4]; static void* g_rdst[4];
void CDiskFileWin32::Read(void* dst, long size, CASuspendableProcess* p)
{ if (g_read == 0) *(long*)dst = 12; else ((unsigned char*)dst)[0] = 0x5a; g_rdst[g_read] = dst; g_rsizes[g_read++] = p ? -1 : size; }
void CDiskFileWin32::Write(const void* src, long size, CASuspendableProcess* p)
{ if (g_write == 0) g_firstLong = *(const long*)src; g_srcs[g_write] = src; g_sizes[g_write++] = p ? -1 : size; }
void CDiskFileWin32::SetPosition(long) {}
void CDiskFileWin32::MovePosition(long) {}
long CDiskFileWin32::GetPosition() const { return 0; }
void CDiskFileWin32::SetLength(long) {}
long CDiskFileWin32::GetLength() const { return 0; }
bool CDiskFileWin32::IsOpen() const { return false; }
static unsigned char g_buf[64]; static bool g_lockOk; static int g_locks, g_unlocks;
bool CScratchBuffer::Lock(void** buffer) { ++g_locks; if (!g_lockOk) return false; *buffer = g_buf; return true; }
void CScratchBuffer::Unlock() { ++g_unlocks; }
CScratchBuffer g_ScratchBuffer;
static const unsigned char* g_init;
void CGameEventPackageSet::InitFromCompressedBuffer(const unsigned char* buffer) { g_init = buffer; }
struct CMainGameComponent_Test : CMainGameComponent {
    bool Run(CGameEventPackageSet* s) { return GetEventPackageSetFromSave(s); }
};
int main()
{
    CMainGameComponent_Test* c = (CMainGameComponent_Test*)new unsigned char[sizeof(CMainGameComponent)];
    new (&c->EventPackageFile) CDiskFileWin32;
    CGameEventPackageSet set;
    g_lockOk = false; bool r = c->Run(&set);
    bool ok = !r && g_locks == 1 && g_read == 0 && g_unlocks == 0 && g_init == 0;
    g_lockOk = true; r = c->Run(&set);
    ok = ok && r && g_locks == 2 && g_read == 2 && g_unlocks == 1 && g_rsizes[0] == 4 && g_rdst[1] == g_buf && g_rsizes[1] == 12
            && g_init == g_buf && g_buf[0] == 0x5a && g_write == 0;
    std::printf(ok ? "00416148_TEST PASS\n" : "00416148_TEST FAIL\n");
    return ok ? 0 : 1;
}