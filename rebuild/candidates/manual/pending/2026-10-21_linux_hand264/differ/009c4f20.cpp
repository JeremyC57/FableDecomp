// CFileInstaller::ProcessAlways() -- under the lock, process any completed read; account for a finished current write; retire every finished active file at the front of the list (drop it from the files to finish, finish the write, record its name as completed, flag completion when both queues are empty); then start writing the next active file
// Grade: DIFFER(485v490) -- retail re-reads PCompletedRead after the test and adds to NoTotalBytesWritten via load/add/store (thread-shared members look qualified, which the purity rule forbids); also front-node addressing order
// Leaf: ProcessAlways
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C4F20, 490 bytes, `ret`.
// Callees: CCriticalSectionLock ctor 0x00A62920 / dtor 0x00A629A0, the completed-read handler 0x009C4120,
//   list<CCountedPointer<CFileInfo const>>::remove 0x009C6E50, CActiveFile::OnWriteFinished 0x009C38C0, the file info's
//   name accessor 0x0099B2C0 (by value), set<CCharString>::insert 0x004AC380, ~CCharString 0x0099EAE0, the compressor dtor
//   0x00A3ACC0, StartWritingFile 0x009C3660, free 0x00BFEA14, operator delete 0x00BFE9BC.
// Layout: retail STLport lists are one node pointer, so the PDB members pack to FilesToCopy +0x38, FilesToFinish +0x3c,
//   ActiveFiles +0x40, CompletedFiles +0x44, PCurrentWriteFile +0x54, NoTotalBytesWritten +0x6c, PCompletedRead +0x74,
//   Completed +0x80.
#include <stdlib.h>
struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
struct CCriticalSectionLock {
    _RTL_CRITICAL_SECTION* Section;
    bool                   Locked;
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock();                                              // 0x00A629A0
};
struct CCharString {
    void* PStringData;
    ~CCharString();                                      // 0x0099EAE0
};
struct CFileInfo {
    unsigned char Data[0xd];
    bool          Skip;                                  // +0x0d
    CCharString GetName() const;                         // 0x0099B2C0
};
struct CCPPointerInfo { long Count; void (__fastcall* Delete)(void*); void* Object; };
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->Count;
            if (Info->Count == 0) {
                Info->Delete(Info->Object);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    T* operator->() const { return Data; }
};
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class T> struct _List_node : _List_node_base { T _M_data; };
template <class T> struct list {
    _List_node_base* _M_node;
    ~list()
    {
        _List_node_base* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _List_node_base* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
        if (_M_node)
            free(_M_node);
    }
    bool empty() const { return _M_node->_M_next == _M_node; }
    T& front() { return ((_List_node<T>*)_M_node->_M_next)->_M_data; }
    void pop_front()
    {
        _List_node_base* pos = _M_node->_M_next;
        _List_node_base* next = pos->_M_next;
        _List_node_base* prev = pos->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        ((_List_node<T>*)pos)->_M_data.~T();
        free(pos);
    }
    void remove(const T& value);                         // 0x009C6E50
};
struct CCompressorZlib {
    unsigned char Data[0x58];
    ~CCompressorZlib();                                  // 0x00A3ACC0
};
struct CActiveFile {
    unsigned char                     Data[0x10];
    unsigned long                     BytesWritten;     // +0x10
    long                              Offset;           // +0x14
    unsigned long                     FileSize;         // +0x18
    CCountedPointer<const CFileInfo>  FileInfo;         // +0x1c
    unsigned char                     Data2[0x14];
    CCompressorZlib                   Compressor;       // +0x38
    list<long>                        Blocks;           // +0x90
    list<long>                        Pending;          // +0x94
    bool IsWriteFinished() const { return FileSize != 0 && BytesWritten >= FileSize; }
    void OnWriteFinished();                              // 0x009C38C0
};
struct _Rb_tree_node_base;
struct CStringSetIterator { _Rb_tree_node_base* _M_node; };
struct CStringSetInsertResult { CStringSetIterator first; bool second; };
struct CStringSet {
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    long                _M_compare;
    CStringSetInsertResult insert(const CCharString& value);   // 0x004AC380
};
struct CActiveRead;
struct CFileInstaller {
    void*                                   VTable;
    _RTL_CRITICAL_SECTION                   CriticalSection;      // +0x04
    unsigned char                           BufferChunks[0xc];    // +0x1c
    unsigned char*                          PReadBuffer;          // +0x28
    unsigned char*                          PWriteBuffer;         // +0x2c
    long                                    AvailableBuffers;     // +0x30
    long                                    AllocatedInputBuffers;   // +0x34
    list<CCountedPointer<const CFileInfo> > FilesToCopy;          // +0x38
    list<CCountedPointer<const CFileInfo> > FilesToFinish;        // +0x3c
    list<CActiveFile>                       ActiveFiles;          // +0x40
    CStringSet                              CompletedFiles;       // +0x44
    CActiveFile*                            PCurrentReadFile;     // +0x50
    CActiveFile*                            PCurrentWriteFile;    // +0x54
    long                                    NoActiveReads;        // +0x58
    long                                    NoActiveWrites;       // +0x5c
    bool                                    WriteMode;            // +0x60
    unsigned long                           BufferChunkSize;      // +0x64
    unsigned long                           NoBufferChunks;       // +0x68
    long                                    NoTotalBytesWritten;  // +0x6c
    long                                    TotalSizeOfAllFiles;  // +0x70
    CActiveRead*                            PCompletedRead;       // +0x74
    bool                                    ThreadFinished;       // +0x78
    bool                                    DecompressionThreadFinished;
    bool                                    ExitThread;
    bool                                    Running;
    unsigned long                           Paused;               // +0x7c
    bool                                    Completed;            // +0x80
    void ProcessCompletedRead(CActiveRead* read);                 // 0x009C4120
    void StartWritingFile(CActiveFile& file, long offset);        // 0x009C3660
    void ProcessAlways();
};

void CFileInstaller::ProcessAlways()
{
    {
        CCriticalSectionLock lock(CriticalSection, false);
        if (PCompletedRead) {
            ProcessCompletedRead(PCompletedRead);
            PCompletedRead = 0;
        }
    }
    if (PCurrentWriteFile && PCurrentWriteFile->IsWriteFinished()) {
        NoTotalBytesWritten += PCurrentWriteFile->FileSize;
        PCurrentWriteFile = 0;
    }
    while (!ActiveFiles.empty()) {
        CActiveFile& file = ActiveFiles.front();
        if (!file.IsWriteFinished())
            break;
        FilesToFinish.remove(file.FileInfo);
        file.OnWriteFinished();
        CompletedFiles.insert(file.FileInfo->GetName());
        ActiveFiles.pop_front();
        if (FilesToFinish.empty() && FilesToCopy.empty())
            Completed = true;
    }
    if (PCurrentWriteFile == 0 && !ActiveFiles.empty()) {
        CActiveFile& file = ActiveFiles.front();
        if (!file.FileInfo->Skip)
            StartWritingFile(file, file.Offset);
        PCurrentWriteFile = &ActiveFiles.front();
    }
}
