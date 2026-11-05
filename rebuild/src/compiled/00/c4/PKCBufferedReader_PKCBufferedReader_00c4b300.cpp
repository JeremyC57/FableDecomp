// PKCBufferedReader::PKCBufferedReader(void* source, const SStreamSegment& segment) -- set up libpk's buffered reader: block size and buffer counts from the segment, the request queue and mailbox, the buffer-state bytes (all free) and the block memory, then the file size from the source
// Leaf: ??0PKCBufferedReader@@QAE@PAXABUSStreamSegment@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4B300, length 830 bytes, thiscall, `ret 8`, EH-framed (byte states 0..5).
// Identity: libpk PKCBufferedReader (stores vtable 0x012B8458; its four error traces name `.\PKCBufferedReader.cpp` lines
//   34 / 35 / 38 / 44); callers COggVorbisStream::COggVorbisStream 0x00C88780 (m_reader at +4) and CBufferedReader_00c3aac0
//   0x00C3AAC0 (base). The landed callers spell it ??0PKCBufferedReader@@QAE@PAXABUSStreamSegment@@@Z (COggVorbisStream rows);
//   reloc_bindings also holds the stale ??0CStreamReader@@... / ??0CReaderBase_00c4b300@@... spellings (NAME_FIXES.tsv).
// Callees: PKCStaticQueue<SReadRequest>(long) 0x00C4BE80 (+0x34), PKCMailbox<SReadRequest>(PKCQueue<SReadRequest>*, long)
//   0x00C4BC20 (+0x48; byte twin of PKCMailbox<SFileReply> 0x00C1D9D0 apart from the EH handler, so the landed
//   CScriptEntry_00c4bc20 / PKCReadSignal spellings are this template, NAME_FIXES.tsv), scalar operator new 0x00BFEA1A (this
//   TU declares no operator new[]), CString operator<< 0x00C0DEC0 / (long) 0x00C0DF00 / (unsigned long) 0x00C0DF10,
//   ~CString 0x00C0BA40, CDebugBreak 0x00C16750, the source's slot 0 (size query into +0x20).
// Layout: the landed ~PKCBufferedReader 0x00C4B270 model; +0x04 is the per-buffer state bytes (filled with 0xFF here) and +0x1C
//   the block memory (blockSize * bufferCount). SStreamSegment {granularity, look-behind, look-ahead}: descriptive names from
//   the copies (+8, +0x10, +0x0C) and the "Granularity cannot be 0" / "Num buffers" checks.
// Shape: member-order initialisers; each trace is a nested-block CString then CDebugBreak, both homed in the dead segment slot.
#include "libpk/PKCProtectionInstance.h"
#include "libpk/PKCSemaphore.h"
void* __cdecl operator new(unsigned int size);  // 0x00BFEA1A
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c4aa40)(const char* text);
extern PFNLOGSINK_00c4aa40 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

class CString { public:
    char* m_pchData;
    CString() : m_pchData(g_pchNil_0139c900) { }
    ~CString();                                         // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);         // 0x00C0DF00
    CString& __fastcall operator<<(unsigned long value);   // 0x00C0DF10
};
struct CDebugBreak {
    CDebugBreak();                         // 0x00C16750
};

struct SReadRequest {
    unsigned long m_id;
};
template<class T> class PKCQueue {
public:
    virtual void Pop(T* item) = 0;
    virtual long Push(const T* item) = 0;
    virtual unsigned long GetCount() = 0;
    virtual long IsFull() = 0;
};
template<class T> class PKCStaticQueue : public PKCQueue<T> {
public:
    PKCStaticQueue(long capacity);              // 0x00C4BE80
    ~PKCStaticQueue();                          // 0x00C4BB00
    virtual void Pop(T* item);
    virtual long Push(const T* item);
    virtual unsigned long GetCount();
    virtual long IsFull();
    long m_capacity;
    T* m_pItems;
    long m_count;
    long m_head;
};
template<class T> class PKCMailbox {
public:
    PKCCriticalSection m_lock;                  // +0x00
    PKCQueue<T>*       m_pQueue;                // +0x18
    PKCSemaphore       m_available;             // +0x1c
    PKCMailbox(PKCQueue<T>* queue, long capacity);          // 0x00C4BC20 for SReadRequest
};

struct SStreamSegment {                     // 12 bytes
    unsigned long m_granularity;            // +0 the block size
    unsigned long m_lookBehind;             // +4
    unsigned long m_lookAhead;              // +8
};
class PKCReadSource {                       // the reader's data source (descriptive): slot 0 reports the size
public:
    virtual bool GetSize(unsigned long* size) = 0;
};

class PKCBufferedReader {                   // vtable 0x012B8458, 0x70 bytes
public:
    PKCBufferedReader(void* source, const SStreamSegment& segment);
    virtual void v00();                     // slot 0: 0x00C4AB90
    unsigned char*      m_pBufferStates;    // +0x04 one byte per buffer, 0xFF = free
    unsigned long       m_blockSize;        // +0x08
    unsigned long       m_lookAhead;        // +0x0c
    unsigned long       m_lookBehind;       // +0x10
    unsigned long       m_bufferCount;      // +0x14
    PKCReadSource*      m_pSource;          // +0x18
    unsigned char*      m_pBufferMemory;    // +0x1c
    unsigned long       m_size;             // +0x20
    unsigned long       m_firstBlock;       // +0x24
    unsigned long       m_28;               // +0x28
    unsigned long       m_blockCount;       // +0x2c
    unsigned long       m_position;         // +0x30
    PKCStaticQueue<SReadRequest> m_requests;   // +0x34
    PKCMailbox<SReadRequest> m_mailbox;     // +0x48
    unsigned long       m_outstanding;      // +0x68
    unsigned long       m_6c;               // +0x6c
};

PKCBufferedReader::PKCBufferedReader(void* source, const SStreamSegment& segment)
    : m_pBufferStates(0)
    , m_blockSize(segment.m_granularity)
    , m_lookAhead(segment.m_lookAhead)
    , m_lookBehind(segment.m_lookBehind)
    , m_bufferCount(segment.m_lookBehind + segment.m_lookAhead)
    , m_pSource((PKCReadSource*)source)
    , m_pBufferMemory(0)
    , m_size(0)
    , m_firstBlock(0)
    , m_28(0)
    , m_blockCount(0)
    , m_position(0)
    , m_requests(m_bufferCount)
    , m_mailbox(&m_requests, m_bufferCount)
    , m_outstanding(0)
    , m_6c(0)
{
    if (m_bufferCount == 0 || m_bufferCount >= 255) {
        {
            CString line;
            line << ".\\PKCBufferedReader.cpp" << "(" << 34L << ") : " << "Error: Num buffers must be less than " << 255UL
                 << " and non zero (" << m_bufferCount << ")" << "\n";
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CDebugBreak();
    }
    if (m_blockSize == 0) {
        {
            CString line;
            line << ".\\PKCBufferedReader.cpp" << "(" << 35L << ") : " << "Error: Granularity cannot be 0" << "\n";
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CDebugBreak();
    }
    m_pBufferStates = new unsigned char[m_bufferCount];
    if (m_pBufferStates == 0) {
        {
            CString line;
            line << ".\\PKCBufferedReader.cpp" << "(" << 38L << ") : " << "Error: EMEM" << "\n";
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CDebugBreak();
    }
    for (unsigned long i = 0; i < m_bufferCount; ++i)
        m_pBufferStates[i] = 0xff;
    m_pBufferMemory = new unsigned char[m_blockSize * m_bufferCount];
    if (m_pBufferMemory == 0) {
        {
            CString line;
            line << ".\\PKCBufferedReader.cpp" << "(" << 44L << ") : " << "Error: EMEM" << "\n";
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CDebugBreak();
    }
    if (!m_pSource->GetSize(&m_size))
        m_size = 0;
}