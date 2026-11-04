// PKCDefaultFileManager::AsyncReadThread() -- the file manager's worker loop: receive each read request, seek and read it, trace a failed or short read, report completion to the request's callback and release the file; a quit request (key -1) ends the loop and releases the done semaphore
// Leaf: ?AsyncReadThread@PKCDefaultFileManager@@QAEXXZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1A9C0, 485 bytes, thiscall `ret`, EH-framed (the two trace CStrings).
// Identity (class proven, method name descriptive; libpk has no PDB or RTTI): the traces name ".\PKCDefaultFileManager.cpp"
//   (lines 40 and 45); the only caller is the landed thread procedure PKCDefaultFileManager::ThreadProc 0x00C1AC30, which
//   binds this address as ?AsyncReadThread@PKCDefaultFileManager@@QAEXXZ (the catalog's "FileManager_AsyncReadThread").
//   The request record is the seven-word SFileRequest the landed QueueRead 0x00C1A110 posts (handle, key, then the read
//   offset, size, buffer, a context word and the completion object); the destructor 0x00C1A230 posts key -1 to stop it.
// Callees: PKCMailbox<SFileRequest>::Receive 0x00C1B030 (mailbox +0x58), KERNEL32 SetFilePointer / ReadFile (IAT
//   0x0143FDE8 / 0x0143FED0, loaded once into esi / edi), the libpk trace stream (CString operator<< 0x00C0DEC0 text,
//   0x00C0DF00 long, 0x00C0DF10 unsigned long, 0x00C0DEF0 unsigned int, ~CString 0x00C0BA40, buffer 0x0139C900, hook
//   0x0143BB2C: the spellings of the landed CloseFile 0x00C1A370 of this TU), the completion object's slot 0,
//   PKCDefaultFileManager::CloseFile 0x00C1A370 and PKCSemaphore::Release 0x00C2A150 (done semaphore +0x78).
// Shape: an unrotated loop (`top: Receive ; cmp key,-1 ; je exit ; ... ; jmp top`, `this` spilled) comes from a
//   do/while over a quit flag (for(;;)+break, while(Receive()->key != -1) and a return inside the loop all rotate the
//   Receive above the loop); `success = false` after each trace's own scope gives the shared ~CString tail followed by
//   the flag store. The short-read size goes out as unsigned long and the count read as unsigned int (two operator<<).
class PKCCriticalSection;
extern "C" __declspec(dllimport) unsigned long __stdcall SetFilePointer(void* hFile, long distance, long* distanceHigh, unsigned long method);   // 0x0143FDE8
extern "C" __declspec(dllimport) int __stdcall ReadFile(void* hFile, void* buffer, unsigned long toRead, unsigned long* read, void* overlapped);       // 0x0143FED0

class CString { public:
    char* m_pBuffer;
    CString(char* buffer) : m_pBuffer(buffer) { }
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);         // 0x00C0DF00
    CString& __fastcall operator<<(unsigned int value); // 0x00C0DEF0
    CString& __fastcall operator<<(unsigned long value);// 0x00C0DF10
    ~CString();                                         // 0x00C0BA40
};
extern char* g_pDebugBuffer;                               // 0x0139C900
extern void (__fastcall* g_pfnErrorReport)(const char*);   // 0x0143BB2C

class PKCReadCallback { public:                 // the request's completion object
    virtual void OnReadComplete(bool success, void* context);   // slot 0
};

struct SFileRequest {                           // seven words
    void*            m_hFile;                   // +0x00
    unsigned long    m_key;                     // +0x04 (-1 = quit)
    unsigned long    m_offset;                  // +0x08
    unsigned long    m_size;                    // +0x0c
    void*            m_pBuffer;                 // +0x10
    void*            m_context;                 // +0x14
    PKCReadCallback* m_pCallback;               // +0x18
};

template<class T> class PKCMailbox {
public:
    unsigned char m_body[0x20];
    T* Receive(T* item);                        // 0x00C1B030
};

class PKCSemaphore { public:
    void* m_hSemaphore;
    void Release();                             // 0x00C2A150
};

class PKCDefaultFileManager {
public:
    void* m_vtable;                             // +0x00
    unsigned char m_04[0x54];                   // +0x04 lock, allocator, open-file map, request queue
    PKCMailbox<SFileRequest> m_mailbox;         // +0x58
    PKCSemaphore m_done;                        // +0x78
    void CloseFile(unsigned long key);          // 0x00C1A370
    void AsyncReadThread();
};

void PKCDefaultFileManager::AsyncReadThread()
{
    bool quit = false;
    do {
        SFileRequest request;
        m_mailbox.Receive(&request);
        if (request.m_key == (unsigned long)-1) {
            quit = true;
        } else {
            SetFilePointer(request.m_hFile, request.m_offset, 0, 0);
            unsigned int read;
            bool success = true;
            if (!ReadFile(request.m_hFile, request.m_pBuffer, request.m_size, (unsigned long*)&read, 0)) {
                {
                    CString s(g_pDebugBuffer);
                    s << ".\\PKCDefaultFileManager.cpp" << "(" << 40L << ") : " << "win32 error" << "\n";
                    if (g_pfnErrorReport)
                        g_pfnErrorReport(s.m_pBuffer);
                }
                success = false;
            } else if (read != request.m_size) {
                {
                    CString s(g_pDebugBuffer);
                    s << ".\\PKCDefaultFileManager.cpp" << "(" << 45L << ") : " << "asked for " << request.m_size
                      << "bytes, only read " << read << " bytes" << "\n";
                    if (g_pfnErrorReport)
                        g_pfnErrorReport(s.m_pBuffer);
                }
                success = false;
            }
            request.m_pCallback->OnReadComplete(success, request.m_context);
            CloseFile(request.m_key);
        }
    } while (!quit);
    m_done.Release();
}