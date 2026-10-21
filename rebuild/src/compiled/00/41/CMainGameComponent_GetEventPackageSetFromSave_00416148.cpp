// CMainGameComponent::GetEventPackageSetFromSave(CGameEventPackageSet* set) -- read size + compressed bytes from the event package file into the scratch buffer and decode the set
// Leaf: ?GetEventPackageSetFromSave@CMainGameComponent@@IAE_NPAVCGameEventPackageSet@@@Z
// Retail: Fable.exe @ 0x00416148, 95 bytes, `ret 4`; Ego_r.pdb ?GetEventPackageSetFromSave@CMainGameComponent@@IAE_NPAVCGameEventPackageSet@@@Z.
// Body: the global CScratchBuffer at 0x013CB498 Lock(&buffer) (0x009F4980); on success EventPackageFile.Read(&size, 4, 0)
// and Read(buffer, size, 0) (CAFile slot 3), set->InitFromCompressedBuffer(buffer) (0x009F1AC0, Ego_r.pdb
// ?InitFromCompressedBuffer@CGameEventPackageSet@@QAEXPBE@Z), Unlock() (0x009F49A0), true; else false. The load twin of
// AddEventPackageSetToSave 0x004161A7.
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