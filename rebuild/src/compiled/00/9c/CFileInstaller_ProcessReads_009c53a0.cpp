// CFileInstaller::ProcessReads() -- when no reads or writes are in flight: retire the finished current read, then pull files off the to-copy list (start an active copy when needed, else record the file as completed) until a read is current, then process it
// Leaf: ?ProcessReads@CFileInstaller
// Layout: retail CFileInstaller (STLport lists are one sentinel pointer each): FilesToCopy +0x38, FilesToFinish +0x3c, ActiveFiles +0x40,
// CompletedFiles +0x44, PCurrentReadFile +0x50, NoActiveReads +0x58, NoActiveWrites +0x5c, Completed +0x80; retail CActiveFile 0xb0 bytes.
// Shape: same-TU noinline stand-ins for IsFileCopyNecessary 0x009C4440 (reads info only) and the CActiveFile(const CFileInfoPointer&)
//          ctor 0x009C4330 (writes only its own object) let VC7.1 keep the front item's Data/Info in edi/ebx across the not-needed arm.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
void __cdecl operator delete(void* block);                  // 0x00BFE9BC
typedef void* HANDLE;
extern "C" __declspec(dllimport) int __stdcall CloseHandle(HANDLE h);
#define INVALID_HANDLE_VALUE ((HANDLE)(long)-1)
inline void* operator new(unsigned int, void* where) { return where; }

struct CCountedItem { };
class CCPPointerInfo { public:
    long m_count;
    void (CCountedItem::*m_pfnDestroy)();
    CCountedItem* m_pItem;
};
class CCharString { public:
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CCharString ConvertToCharString() const;                // 0x0099B2C0
};
struct CFileInfo { CWideString Source; };
struct CFileInfoPointer {                                   // CCountedPointer<const CFileInstaller::CFileInfo>
    const CFileInfo* Data;
    CCPPointerInfo*  Info;
    CFileInfoPointer(const CFileInfoPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_count;
    }
    ~CFileInfoPointer()
    {
        if (Info && --Info->m_count == 0) {
            (Info->m_pItem->*Info->m_pfnDestroy)();
            operator delete(Info);
        }
        Data = 0;
        Info = 0;
    }
};
class CCompressorZlib { public:
    unsigned char _body[0x58];
    ~CCompressorZlib();                                     // 0x00A3ACC0
};
struct CSentinelNode { CSentinelNode* next; CSentinelNode* prev; };
struct CSentinelList {
    CSentinelNode* head;
    ~CSentinelList()
    {
        CSentinelNode* node = head->next;
        while (node != head) {
            CSentinelNode* doomed = node;
            node = node->next;
            free(doomed);
        }
        head->next = head;
        head->prev = head;
        if (head)
            free(head);
    }
};
struct CActiveFile {
    HANDLE           SrcFileHandle;                         // +0x00
    HANDLE           DstFileHandle;
    unsigned long    CurrentSrcOffset;
    unsigned long    NoBytesRead;                           // +0x0c
    unsigned long    NoBytesWritten;
    unsigned long    InputSize;                             // +0x14
    unsigned long    OutputSize;
    CFileInfoPointer PFileInfo;                             // +0x1c
    unsigned long    MinWriteSize;
    unsigned long    MinReadSize;
    unsigned char    _pad2c[4];
    double           StartTime;                             // +0x30
    CCompressorZlib  Decompressor;                          // +0x38
    CSentinelList    PendingWrites;                         // +0x90
    CSentinelList    PendingDecompression;                  // +0x94
    unsigned char    _pad98[0x18];
    CActiveFile(const CFileInfoPointer& fileInfo);          // 0x009C4330
    CActiveFile(const CActiveFile& other);                  // 0x009C72B0
};
template <class T> struct CListNode { CListNode* next; CListNode* prev; T value; };
template <class T> struct CList {
    CListNode<T>* head;
    bool empty() const { return head->next == head; }
    T& front() { return head->next->value; }
    T& back() { return head->prev->value; }
    void push_back(const T& value)
    {
        CListNode<T>* last = head;
        CListNode<T>* node = (CListNode<T>*)malloc(sizeof(CListNode<T>));
        new (&node->value) T(value);
        CListNode<T>* prev = last->prev;
        node->next = last;
        node->prev = prev;
        prev->next = node;
        last->prev = node;
    }
    void pop_front()
    {
        CListNode<T>* node = head->next;
        CListNode<T>* next = node->next;
        CListNode<T>* prev = node->prev;
        prev->next = next;
        next->prev = prev;
        node->value.~T();
        free(node);
    }
};
struct CCharStringSet {
    void* Header; void* _pad[2];
    struct CInsertResult { void* node; bool inserted; };
    CInsertResult* insert(CInsertResult* result, const CCharString& key);   // 0x004AC380
};
class CFileInstaller { public:
    unsigned char             _pad[0x38];
    CList<CFileInfoPointer>   FilesToCopy;                  // +0x38
    CList<CFileInfoPointer>   FilesToFinish;                // +0x3c
    CList<CActiveFile>        ActiveFiles;                  // +0x40
    CCharStringSet            CompletedFiles;               // +0x44
    CActiveFile*              PCurrentReadFile;             // +0x50
    CActiveFile*              PCurrentWriteFile;
    long                      NoActiveReads;                // +0x58
    long                      NoActiveWrites;               // +0x5c
    unsigned char             _pad60[0x20];
    bool                      Completed;                    // +0x80
    bool IsFileCopyNecessary(const CFileInfoPointer& info); // 0x009C4440
    void StartRead(CActiveFile* file);                      // 0x009C3590
    void ProcessRead(CActiveFile* file);                    // 0x009C3AE0
    void ProcessReads();
};

#pragma auto_inline(off)
__declspec(noinline) bool CFileInstaller::IsFileCopyNecessary(const CFileInfoPointer& info)
{
    return info.Data != 0;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
__declspec(noinline) CActiveFile::CActiveFile(const CFileInfoPointer& fileInfo) : PFileInfo(fileInfo)
{
    SrcFileHandle = 0;
}
#pragma auto_inline(on)
void CFileInstaller::ProcessReads()
{
    if (NoActiveWrites + NoActiveReads >= 1)
        return;
    CActiveFile* current = PCurrentReadFile;
    if (current && current->NoBytesRead >= current->InputSize) {
        if (current->SrcFileHandle != INVALID_HANDLE_VALUE) {
            CloseHandle(current->SrcFileHandle);
            current->SrcFileHandle = INVALID_HANDLE_VALUE;
        }
        PCurrentReadFile = 0;
    }
    while (PCurrentReadFile == 0 && !FilesToCopy.empty()) {
        const CFileInfoPointer info = FilesToCopy.front();
        if (IsFileCopyNecessary(info)) {
            ActiveFiles.push_back(CActiveFile(info));
            StartRead(&ActiveFiles.back());
            PCurrentReadFile = &ActiveFiles.back();
            FilesToFinish.push_back(info);
        } else {
            CCharStringSet::CInsertResult result;
            CompletedFiles.insert(&result, info.Data->Source.ConvertToCharString());
        }
        FilesToCopy.pop_front();
        if (FilesToFinish.empty() && FilesToCopy.empty())
            Completed = true;
    }
    if (PCurrentReadFile)
        ProcessRead(PCurrentReadFile);
}