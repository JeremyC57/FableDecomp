// CFileInstaller::ProcessActiveFileForWrite(CActiveFile* file) -- take the file's first pending write, copy its buffer (rounded up to the file's sector size) into the installer's write buffer, free the source buffer and issue an overlapped WriteFileEx; on success drop the pending entry and count the write, on failure flag the installer error and delete the request
// Leaf: ProcessActiveFileForWrite
// Grade: DIFFER(358v359) -- the pending write copied by value (`SPendingWrite write = front()`: buffer/offset/size in
//   ebp/edi/frame as retail) and the failure arm as the in-class inline CFileInstaller::SetDirtyDiscError(true) (landed
//   0x009C3530 body); landed names: CTBaseSingleton<CFileInstaller>::Get (was CFileInstallerSingleton::Get),
//   CFileInstaller::DirtyDiscError (was g_installerNoPauseWait), ExitThread +0x7A (was Error), CriticalSection +0x04
//   (was Section), CCriticalSectionLock; residue: register colouring of the write copy (eax vs ecx) and the request
//   memset zero register, plus retail's dead ExitThread load
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C3FB0, length 357 bytes (`ret 4`).
// Callees: operator new @ 0x00BFEA1A / operator delete @ 0x00BFE9BC, SetEvent / WriteFileEx (IAT 0x0143FDE0 / 0x0143FDD8),
//   free @ 0x00BFEA14 (the list node), CTBaseSingleton<CFileInstaller>::Get @ 0x00404440 (`__fastcall`), CCriticalSectionLock
//   ctor/dtor @ 0x00A62920 / 0x00A629A0, the write completion routine @ 0x009C3820.
// Layout: CActiveFile handle +4, sector size +0x24, PendingWrites list +0x90 (sentinel pointer; nodes {next, prev, buffer,
//   offset, size}); CFileInstaller critical section +4, write buffer +0x2C, free/pending buffer counts +0x30/+0x34, pending
//   write count +0x5C, error flag +0x7A, events +0x84/+0x88 (the landed FreeBuffer 0x009C3330 inline here).
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);   // IAT 0x0143FDE0
struct OVERLAPPED_Fable { unsigned long Internal, InternalHigh, Offset, OffsetHigh; void* hEvent; };
typedef void (__stdcall* LPOVERLAPPED_COMPLETION_ROUTINE_Fable)(unsigned long, unsigned long, OVERLAPPED_Fable*);
extern "C" __declspec(dllimport) int __stdcall WriteFileEx(void* file, const void* buffer, unsigned long bytes,
                                                         OVERLAPPED_Fable* overlapped, LPOVERLAPPED_COMPLETION_ROUTINE_Fable routine);   // IAT 0x0143FDD8
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
struct SInstallBuffer {
    unsigned char* Data;                                // +0x00
    bool           InUse;                               // +0x04
    bool           Pending;                             // +0x05
};
struct SPendingWrite { SInstallBuffer* Buffer; unsigned long Offset; unsigned long Size; };
struct SPendingWriteNode { SPendingWriteNode* _M_next; SPendingWriteNode* _M_prev; SPendingWrite _M_data; };
struct CPendingWriteList {
    SPendingWriteNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    SPendingWrite& front() { return _M_node->_M_next->_M_data; }
    void pop_front()
    {
        SPendingWriteNode* node = _M_node->_M_next;
        SPendingWriteNode* next = node->_M_next;
        SPendingWriteNode* prev = node->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(node);
    }
};
struct CActiveFile {
    unsigned char     _pad00[4];
    void*             Handle;                           // +0x04
    unsigned char     _pad08[0x1c];
    unsigned long     SectorSize;                       // +0x24
    unsigned char     _pad28[0x68];
    CPendingWriteList PendingWrites;                    // +0x90
};
struct CFileInstaller;
struct SWriteRequest {                                  // 0x20 bytes
    CActiveFile*     File;                              // +0x00
    SInstallBuffer*  Buffer;                            // +0x04
    OVERLAPPED_Fable Overlapped;                        // +0x08
    CFileInstaller*  Installer;                         // +0x1C
    SWriteRequest(CActiveFile* file, SInstallBuffer* buffer, CFileInstaller* installer) : File(file), Buffer(buffer), Installer(installer) {}
};
void __stdcall WriteCompletionRoutine(unsigned long error, unsigned long bytes, OVERLAPPED_Fable* overlapped);   // 0x009C3820
struct CRITICAL_SECTION_Fable { unsigned char _body[0x18]; };
struct CCriticalSectionLock {
    CRITICAL_SECTION_Fable* m_pSection;
    bool                    m_locked;
    CCriticalSectionLock(CRITICAL_SECTION_Fable* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock();                                               // 0x00A629A0
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                 // CFileInstaller: 0x00404440 (out of line)
};
struct CFileInstaller {
    unsigned char          _pad00[4];
    CRITICAL_SECTION_Fable CriticalSection;             // +0x04
    unsigned char          _pad1c[0x10];
    unsigned char*         WriteBuffer;                 // +0x2C
    long                   FreeBuffers;                 // +0x30
    long                   PendingBuffers;              // +0x34
    unsigned char          _pad38[0x24];
    long                   PendingWriteCount;           // +0x5C
    unsigned char          _pad60[0x1a];
    bool                   ExitThread;                  // +0x7A
    unsigned char          _pad7b[9];
    void*                  BufferFreedEvent;            // +0x84
    void*                  WakeEvent;                   // +0x88
    void FreeBuffer(SInstallBuffer* buffer)
    {
        if (buffer->InUse) {
            buffer->InUse = false;
            ++FreeBuffers;
            PendingBuffers -= (buffer->Pending ? 1 : 0);
            SetEvent(BufferFreedEvent);
            SetEvent(WakeEvent);
        }
    }
    static bool DirtyDiscError;                         // 0x013CA814
    static void __fastcall SetDirtyDiscError(bool error)   // 0x009C3530 (in-class inline; landed out-of-line copy)
    {
        CCriticalSectionLock lock(&CTBaseSingleton<CFileInstaller>::Get().CriticalSection, false);
        DirtyDiscError = error;
        if (CTBaseSingleton<CFileInstaller>::Get().ExitThread || error)
            error = true;
        CTBaseSingleton<CFileInstaller>::Get().ExitThread = error;
    }
    void ProcessActiveFileForWrite(CActiveFile* file);
};
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memset, memcpy)

void CFileInstaller::ProcessActiveFileForWrite(CActiveFile* file)
{
    if (file->PendingWrites.empty())
        return;
    SPendingWrite write = file->PendingWrites.front();
    unsigned long offset = write.Offset;
    unsigned long size = write.Size;
    SWriteRequest* request = new SWriteRequest(file, write.Buffer, this);
    memset(&request->Overlapped, 0, sizeof(request->Overlapped));
    request->Overlapped.Offset = offset;
    request->Overlapped.hEvent = request;
    unsigned long bytes = (size + file->SectorSize - 1) / file->SectorSize * file->SectorSize;
    memcpy(WriteBuffer, request->Buffer->Data, bytes);
    FreeBuffer(request->Buffer);
    request->Buffer = 0;
    if (WriteFileEx(file->Handle, WriteBuffer, bytes, &request->Overlapped, WriteCompletionRoutine)) {
        file->PendingWrites.pop_front();
        ++PendingWriteCount;
        return;
    }
    SetDirtyDiscError(true);
    delete request;
}
