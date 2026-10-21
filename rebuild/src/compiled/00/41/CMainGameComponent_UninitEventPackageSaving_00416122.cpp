// CMainGameComponent::UninitEventPackageSaving() -- stop writing the event-package file: close EventPackageFile if it is open, clear SavingEventPackages
// Leaf: ?UninitEventPackageSaving@CMainGameComponent@@IAEXXZ
// Retail: Fable.exe @ 0x00416122, 38 bytes; Ego_r.pdb ?UninitEventPackageSaving@CMainGameComponent@@IAEXXZ. Virtual IsOpen (slot 10, +0x28)
// then Close (slot 2, +0x08) on the CDiskFileWin32 member (vtable 0x0122D06C; VC7.1 does not devirtualize the member
// calls), then the byte store to SavingEventPackages (+0x16171).
// Layout: Ego_r.pdb CMainGameComponent EventPackageFile +0x16154 (CDiskFileWin32, 28 bytes), LoadingEventPackages
// +0x16170, SavingEventPackages +0x16171 -- retail agrees here. No engine/ header for CMainGameComponent; CAFile /
// CDiskFileWin32 virtual order from the retail vtable.
// Opt: size TU (unaligned entry; /O2 reloads the vptr through edx and reads it before the lea).
#pragma optimize("s", on)
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
protected:
    void UninitEventPackageSaving();
public:
    unsigned char  _pad0[0x16154];
    CDiskFileWin32 EventPackageFile;                     // +0x16154
    bool           LoadingEventPackages;                 // +0x16170
    bool           SavingEventPackages;                  // +0x16171
};
void CMainGameComponent::UninitEventPackageSaving()
{
    if (EventPackageFile.IsOpen())
        EventPackageFile.Close();
    SavingEventPackages = false;
}