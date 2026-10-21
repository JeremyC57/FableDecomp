// CFileInstaller::ThreadMain() -- worker loop: wait (event, alertable) until there are files to copy, then until told to exit service pause requests (draining active reads/writes first), start the next write or read, run ProcessAlways, flip between read and write mode by buffer use, and wait 100 ms; finally mark the thread finished
// Grade: DIFFER(468v469) -- retail tests PCompletedRead in memory and re-reads it (the thread-shared qualifier the purity rule forbids), stores Running before the exit test, and renames the shl temporaries
// Leaf: ThreadMain
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C5690, 469 bytes, `ret`, 8-aligned frame.
// Callees: CCriticalSectionLock ctor 0x00A62920 / Unlock 0x00A62960 / Lock 0x00A62980 / dtor 0x00A629A0,
//   WaitForSingleObjectEx / SetEvent (imports 0x0143FE08 / 0x0143FDE0), the completed-read handler 0x009C4120,
//   StartWritingNextBlock 0x009C3FB0, StartReading 0x009C53A0, ProcessAlways 0x009C4F20.
// Layout: as ProcessAlways; Event +0x84, PauseStartEvent +0x8c, PauseFinishEvent +0x90 (retail packing of the PDB order).
#include <stdlib.h>
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObjectEx(void* handle, unsigned long ms, int alertable);
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* handle);
struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
struct CCriticalSectionLock {
    _RTL_CRITICAL_SECTION* Section;
    bool                   Locked;
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock();                                              // 0x00A629A0
    void Unlock();                                                        // 0x00A62960
    void Lock();                                                          // 0x00A62980
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
    bool                                    IgnoreTimeStampWhenCopying;
    void*                                   Event;                // +0x84
    void*                                   DecompressionEvent;   // +0x88
    void*                                   PauseStartEvent;      // +0x8c
    void*                                   PauseFinishEvent;     // +0x90
    void StartWritingNextBlock(CActiveFile* file);                // 0x009C3FB0
    void StartReading();                                          // 0x009C53A0
    void ThreadMain();
    void ProcessCompletedRead(CActiveRead* read);                 // 0x009C4120
    void StartWritingFile(CActiveFile& file, long offset);        // 0x009C3660
    void ProcessAlways();
};


void CFileInstaller::ThreadMain()
{
    while (!ExitThread) {
        CCriticalSectionLock lock(CriticalSection, false);
        if (!FilesToCopy.empty())
            break;
        lock.Unlock();
        WaitForSingleObjectEx(Event, 0xffffffff, 1);
    }
    Running = true;
    while (!ExitThread) {
        CCriticalSectionLock lock(CriticalSection, false);
        if (Paused) {
            while (NoActiveReads > 0 || NoActiveWrites > 0) {
                lock.Unlock();
                WaitForSingleObjectEx(Event, 0xffffffff, 1);
                lock.Lock();
                CCriticalSectionLock readLock(CriticalSection, false);
                if (PCompletedRead) {
                    ProcessCompletedRead(PCompletedRead);
                    PCompletedRead = 0;
                }
            }
            lock.Unlock();
            SetEvent(PauseStartEvent);
            WaitForSingleObjectEx(PauseFinishEvent, 0xffffffff, 1);
            lock.Lock();
        }
        if (WriteMode) {
            if (PCurrentWriteFile && NoActiveWrites + NoActiveReads < 1)
                StartWritingNextBlock(PCurrentWriteFile);
        } else {
            StartReading();
        }
        ProcessAlways();
        if (WriteMode) {
            if (AllocatedInputBuffers * 2 < NoBufferChunks
                && (PCurrentWriteFile == 0 || (PCurrentWriteFile->Blocks.empty() && PCurrentWriteFile->Pending.empty())))
                WriteMode = false;
        } else {
            if (AllocatedInputBuffers * 2 >= NoBufferChunks || (PCurrentReadFile == 0 && !FilesToFinish.empty()))
                WriteMode = true;
        }
        lock.Unlock();
        WaitForSingleObjectEx(Event, 100, 1);
    }
    CCriticalSectionLock lock(CriticalSection, false);
    ThreadFinished = true;
    Running = false;
    lock.Unlock();
}
