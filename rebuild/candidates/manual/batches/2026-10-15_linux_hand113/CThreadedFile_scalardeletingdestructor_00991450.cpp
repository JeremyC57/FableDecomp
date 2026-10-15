// CThreadedFile::`scalar deleting destructor'(unsigned int) -- stores the CThreadedFile vtable, closes the file if it is open (inlined Close), destroys the file name, runs CBase::~CBase and frees on bit 0
// Leaf: ??_GCThreadedFile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00991450, length 88 bytes (vtable 0x0129A158 slot 0).
// Callees: the threaded-file manager's pending-request purge @ 0x0098E7D0 on the global manager
// g_ThreadedFileManager @ 0x013BC870, CloseHandle (IAT 0x0143FE30), CWideString::~CWideString @ 0x0099B510,
// CBase::~CBase @ 0x0099A300, operator delete @ 0x00BFE9BC.
// Layout: PDB CThreadedFile (28 bytes): DeviceID +0x04, FileHandle +0x08, Filename (CWideString) +0x0c,
// Length +0x10, PhysicalSortKey +0x14, OpenedForWrite +0x18, OpenFlag +0x19.
// Shape: the destructor body is the inline Close() (0x0098EC40 is its out-of-line copy): the handle
// compare against INVALID_HANDLE_VALUE is scheduled before the vtable store; the manager drops any
// request still queued for this file, the handle is closed and reset, the two flags cleared through
// one `xor al,al` zero register (OpenFlag first). Close must be `__forceinline` here: a plain inline
// member with a call and an import call stays out of line in the deleting destructor.
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);   // IAT 0x0143FE30

struct CThreadedFile;

struct CThreadedFileManager {
    void RemovePendingRequests(CThreadedFile* file);   // 0x0098E7D0
};
extern CThreadedFileManager g_ThreadedFileManager;    // 0x013BC870

struct CWideString {
    void* m_pData;
    CWideString();
    ~CWideString();                                // 0x0099B510
};

struct CBase {                                     // root polymorphic base
    CBase();
    virtual ~CBase();                              // 0x0099A300
};

struct CThreadedFile : CBase {                     // vtable 0x0129A158
    long          DeviceID;                        // +0x04 EThreadedFileDeviceID
    void*         FileHandle;                      // +0x08
    CWideString   Filename;                        // +0x0c
    unsigned long Length;                          // +0x10
    unsigned long PhysicalSortKey;                 // +0x14
    bool          OpenedForWrite;                  // +0x18
    bool          OpenFlag;                        // +0x19
    __forceinline void Close()
    {
        if (FileHandle != (void*)-1) {
            g_ThreadedFileManager.RemovePendingRequests(this);
            CloseHandle(FileHandle);
            FileHandle = (void*)-1;
            OpenFlag = false;
            OpenedForWrite = false;
        }
    }
    CThreadedFile() : FileHandle((void*)-1) { }
    __forceinline virtual ~CThreadedFile() { Close(); }
};

CThreadedFile* Create_CThreadedFile()
{
    return new CThreadedFile;
}
