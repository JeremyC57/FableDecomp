// PKCBufferedReader::~PKCBufferedReader() -- EH-framed destructor of libpk's buffered stream reader: its vtable (0x012B8458), every outstanding buffer handed back (ReturnAllBuffers 0x00C4B010; EH state 1), the buffer memory (+4) and the buffer-state array (+0x1c) freed, then the members: the read signal (+0x48: its semaphore 0x00C29FB0, state 2, then its critical section 0x00C1C820, state 0) and the request queue (+0x34, 0x00C4BB00)
// Leaf: ??1PKCBufferedReader@@QAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4B270, 137 bytes, thiscall, plain `ret`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 445 "0xC4B270 CTeleportDataOwner dtor = ~CStreamReader"):
//   - the constructor 0x00C4B300 stores the same vtable 0x012B8458 and builds the same members (the request queue
//     PKCReadRequestQueue(count) 0x00C4BE80 at +0x34, the read signal 0x00C4BC20(&queue, count) at +0x48, zeroes +4,
//     +0x1c and +0x68); its "buffer count" error message names the source file `.\PKCBufferedReader.cpp` (0x012B8200), as
//     the landed seek 0x00C4AA40 does; agent445 called the class CStreamReader, the file string gives libpk's name;
//   - the callers are COggVorbisStream's destructor 0x00C885C0 (the reader member at +4, renamed this pass) and the
//     landed 0x00C3A9D0 (tail jump); ReturnAllBuffers 0x00C4B010 loops ReturnBuffer(0) 0x00C4AC70 while +0x68 is set;
//   - +0x48 is a critical section (InitializeCriticalSection 0x00C1C810 / DeleteCriticalSection 0x00C1C820) with the
//     queue pointer at +0x18 and a PKCSemaphore at +0x1c (CreateSemaphoreA 0x00C29FC0 with `.\PKCSemaphore.cpp`, CloseHandle
//     0x00C29FB0).
//   The landed unattributed CTeleportDataOwner (`_Dest_val<...TeleportData>`) is a propagated label. PKCReadSignal and
//   PKCCriticalSection are descriptive (no PDB/RTTI for libpk).
// Shape: the destructor is not virtual (slot 0 of 0x012B8458 is 0x00C4AB90, an EH-framed member taking a byte flag, not a
//   deleting destructor); the read signal's implicit destructor is inlined.
class PKCCriticalSection {                              // one CRITICAL_SECTION
public:
    PKCCriticalSection();                               // 0x00C1C810 (InitializeCriticalSection)
    ~PKCCriticalSection();                              // 0x00C1C820 (DeleteCriticalSection)
    unsigned char m_section[0x18];
};

class PKCSemaphore {                                    // .\PKCSemaphore.cpp
public:
    PKCSemaphore(long initialCount, long maximumCount); // 0x00C29FC0 (CreateSemaphoreA)
    ~PKCSemaphore();                                    // 0x00C29FB0 (CloseHandle)
    void* m_handle;
};

struct SReadRequest;

class PKCReadRequestQueue {                             // vtable 0x012B8364, 0x14 bytes
public:
    PKCReadRequestQueue(long capacity);                 // 0x00C4BE80
    ~PKCReadRequestQueue();                             // 0x00C4BB00
    virtual void Pop(SReadRequest* request);            // slot 0 (0x00C4BEB0)
    unsigned char _body04[0x10];
};

class PKCReadSignal : public PKCCriticalSection {       // 0x20 bytes (destructor compiler-generated)
public:
    PKCReadSignal(PKCReadRequestQueue* queue, long capacity);   // 0x00C4BC20
    PKCReadRequestQueue* m_pQueue;                      // +0x18
    PKCSemaphore         m_semaphore;                   // +0x1c (initial 0, maximum = capacity)
};

struct SStreamSegment;

class PKCBufferedReader {                               // vtable 0x012B8458, 0x70 bytes
public:
    PKCBufferedReader(void* source, const SStreamSegment& segment);   // 0x00C4B300
    ~PKCBufferedReader();
    virtual void v00();                                 // slot 0: 0x00C4AB90
    void ReturnBuffer(long which);                      // 0x00C4AC70
    void ReturnAllBuffers();                            // 0x00C4B010
    unsigned char*      m_pBufferMemory;                // +0x04
    unsigned long       m_blockSize;                    // +0x08
    unsigned long       m_0c;                           // +0x0c
    unsigned long       m_lookBehind;                   // +0x10
    unsigned long       m_bufferCount;                  // +0x14
    void*               m_pSource;                      // +0x18
    unsigned char*      m_pBufferStates;                // +0x1c
    unsigned long       m_size;                         // +0x20
    unsigned long       m_firstBlock;                   // +0x24
    unsigned long       m_28;                           // +0x28
    unsigned long       m_blockCount;                   // +0x2c
    unsigned long       m_position;                     // +0x30
    PKCReadRequestQueue m_requests;                     // +0x34
    PKCReadSignal       m_signal;                       // +0x48
    unsigned long       m_outstanding;                  // +0x68 buffers handed out
    unsigned long       m_6c;                           // +0x6c
};

PKCBufferedReader::~PKCBufferedReader()
{
    ReturnAllBuffers();
    delete m_pBufferMemory;
    delete m_pBufferStates;
}
