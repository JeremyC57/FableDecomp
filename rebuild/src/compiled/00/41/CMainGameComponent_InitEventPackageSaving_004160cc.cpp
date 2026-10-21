// CMainGameComponent::InitEventPackageSaving(const wchar_t* file_name) -- create the event-package file for writing and write a one-player header; set SavingEventPackages
// Leaf: ?InitEventPackageSaving@CMainGameComponent@@IAE_NPB_W@Z
// Retail: Fable.exe @ 0x004160CC, 86 bytes, `ret 4`; caller 0x004173DA (passes the global name's `operator const wchar_t*`);
// Ego_r.pdb ?InitEventPackageSaving@CMainGameComponent@@IAE_NPB_W@Z.
// Body: EventPackageFile.Open(CWideString(file_name) (0x0099B6B0, homed in the dead argument slot), access 4, flags 2)
// (virtual slot 1); ~CWideString (0x0099B510); EventPackageFileHeader.NoPlayers = 1;
// EventPackageFile.Write(&EventPackageFileHeader, 4, 0) (slot 4); SavingEventPackages = true; return true.
// Layout: Ego_r.pdb CMainGameComponent EventPackageFile +0x16154 (CDiskFileWin32), LoadingEventPackages +0x16170,
// SavingEventPackages +0x16171, EventPackageFileHeader +0x16174 (CEventPackageFileHeader {long NoPlayers}) -- retail agrees.
// CAFile virtual order from the retail vtable (as in the landed 0x004160A6 twin).
// Opt: size TU with frame pointers (`push ebp / mov ebp,esp`, unaligned entry).
#pragma optimize("s", on)
#pragma optimize("y", off)
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
bool CMainGameComponent::InitEventPackageSaving(const wchar_t* file_name)
{
    EventPackageFile.Open(file_name, FILE_ACCESS_WRITE, 2);
    EventPackageFileHeader.NoPlayers = 1;
    EventPackageFile.Write(&EventPackageFileHeader, sizeof(EventPackageFileHeader), 0);
    SavingEventPackages = true;
    return true;
}