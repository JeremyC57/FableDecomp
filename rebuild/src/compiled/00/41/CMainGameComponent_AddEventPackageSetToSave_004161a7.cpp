// CMainGameComponent::AddEventPackageSetToSave(const CGameEventPackageSet* set) -- compress the set into the scratch buffer and append size + bytes to the event package file
// Leaf: ?AddEventPackageSetToSave@CMainGameComponent@@IAEXPBVCGameEventPackageSet@@@Z
// Retail: Fable.exe @ 0x004161A7, 91 bytes, `ret 4`; Ego_r.pdb ?AddEventPackageSetToSave@CMainGameComponent@@IAEXPBVCGameEventPackageSet@@@Z.
// Body: the global CScratchBuffer at 0x013CB498 Lock(&buffer) (0x009F4980); on success size =
// set->CompressIntoBuffer(buffer) (0x009F19A0, Ego_r.pdb ?CompressIntoBuffer@CGameEventPackageSet@@QBEJPAE@Z; the long lands in
// the dead `set` slot), EventPackageFile.Write(&size, 4, 0) and Write(buffer, size, 0) (CAFile slot 4), then Unlock() (0x009F49A0).
// Layout: CMainGameComponent EventPackageFile +0x16154 (CDiskFileWin32), as InitEventPackageSaving 0x004160CC.
// Opt: size TU with frame pointer (`push ebp / mov ebp,esp`, `leave`, unaligned entry).
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
class CScratchBuffer {
public:
    bool Lock(void** buffer);                            // 0x009F4980
    void Unlock();                                       // 0x009F49A0
};
extern CScratchBuffer g_ScratchBuffer;                   // 0x013CB498
class CGameEventPackageSet {
public:
    long CompressIntoBuffer(unsigned char* buffer) const;   // 0x009F19A0
};
class CEventPackageFileHeader {
public:
    long NoPlayers;                                      // +0x00
};
class CMainGameComponent {
protected:
    bool InitEventPackageLoading(const wchar_t* file_name);
    bool InitEventPackageSaving(const wchar_t* file_name);
    void AddEventPackageSetToSave(const CGameEventPackageSet* set);
public:
    unsigned char           _pad0[0x16154];
    CDiskFileWin32          EventPackageFile;            // +0x16154
    bool                    LoadingEventPackages;        // +0x16170
    bool                    SavingEventPackages;         // +0x16171
    CEventPackageFileHeader EventPackageFileHeader;      // +0x16174
};
void CMainGameComponent::AddEventPackageSetToSave(const CGameEventPackageSet* set)
{
    void* buffer;
    if (g_ScratchBuffer.Lock(&buffer))
    {
        long size = set->CompressIntoBuffer((unsigned char*)buffer);
        EventPackageFile.Write(&size, 4, NULL);
        EventPackageFile.Write(buffer, size, NULL);
        g_ScratchBuffer.Unlock();
    }
}