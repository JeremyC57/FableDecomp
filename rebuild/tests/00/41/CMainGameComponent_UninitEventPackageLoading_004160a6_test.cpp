// test for 004160a6 UninitEventPackageLoading
#include <stdio.h>
#include <string.h>
class CWideString;
class CASuspendableProcess;
enum FileAccessType { FILE_ACCESS_READ };
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
class CMainGameComponent {
public:
public:
    void UninitEventPackageLoading();
public:
    unsigned char  _pad0[0x16154];
    CDiskFileWin32 EventPackageFile;                     // +0x16154
    bool           LoadingEventPackages;                 // +0x16170
    bool           SavingEventPackages;                  // +0x16171
};
void CMainGameComponent::UninitEventPackageLoading()
{
    if (EventPackageFile.IsOpen())
        EventPackageFile.Close();
    LoadingEventPackages = false;
}

static int g_open, g_close;
CAFile::~CAFile() {}
CDiskFileWin32::~CDiskFileWin32() {}
void CDiskFileWin32::Open(const CWideString&, FileAccessType, unsigned long) {}
void CDiskFileWin32::Close() { ++g_close; g_open = 0; }
void CDiskFileWin32::Read(void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::Write(const void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::SetPosition(long) {}
void CDiskFileWin32::MovePosition(long) {}
long CDiskFileWin32::GetPosition() const { return 0; }
void CDiskFileWin32::SetLength(long) {}
long CDiskFileWin32::GetLength() const { return 0; }
bool CDiskFileWin32::IsOpen() const { return g_open != 0; }
int main()
{
    CMainGameComponent* c = new CMainGameComponent;
    c->LoadingEventPackages = true; g_open = 1;
    c->UninitEventPackageLoading();
    int ok = g_close == 1 && c->LoadingEventPackages == false;
    c->LoadingEventPackages = true;
    c->UninitEventPackageLoading();
    ok &= g_close == 1 && c->LoadingEventPackages == false;
    ok &= (char*)&c->EventPackageFile - (char*)c == 0x16154 && (char*)&c->LoadingEventPackages - (char*)c == 0x16170;
    printf(ok ? "004160a6_TEST PASS\n" : "004160a6_TEST FAIL\n");
    return ok ? 0 : 1;
}