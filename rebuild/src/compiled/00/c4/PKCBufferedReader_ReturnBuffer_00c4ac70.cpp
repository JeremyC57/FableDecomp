// PKCBufferedReader::ReturnBuffer(bool* pMoreResults) -- hand back the oldest outstanding read buffer: report whether the request mailbox still holds results, take the finished request id from it, check it against the slot map, free every consecutive slot (from that id round the ring) that carries the id, and drop the outstanding count
// Leaf: ?ReturnBuffer@PKCBufferedReader@@
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4AC70, 568 bytes (+ int3 padding), `ret 4`. Built under /EHsc (one EH state
//   per assertion's log line: 0, 1, 2). Callers: the landed ReturnAllBuffers 0x00C4B010 (`ReturnBuffer(0)` until the
//   outstanding count is zero) and three more sites in the reader (0x00C4AF9A, 0x00C4B0A6, 0x00C4B1A6).
// Callees: the request mailbox at +0x48: the landed GetCount 0x00C4BE10 (queue count under the mailbox lock) and
//   GetResult 0x00C4BC80 (waits, then pops the finished request id); the libpk assertion line as the landed
//   GetAvailableReadSize / Seek 0x00C4AA40 (CString @ 0x0139C900 / Append 0x00C0DEC0 / AppendNumber 0x00C0DF00 /
//   ~CString 0x00C0BA40, sink 0x0143BB2C, CTrace 0x00C16750). Literals ".\PKCBufferedReader.cpp" lines 56 / 62 / 63:
//   "Error: There are no requests pending!", "Error: returned buffer out of scope", "Error: Request ID out of sync".
// Layout (descriptive, libpk has no PDB): slot-owner map (one byte per buffer slot, 0xFF = free) +0x04, slot count
//   +0x14, request mailbox +0x48, outstanding request count +0x68.
// Shape: the ring walk re-reads the slot count for the modulus and the loop test; a slot that does not carry the id
//   ends the walk by pushing the index to the count (`mov edi,ecx`), not by a `break`.
extern char* g_pchNil_0139c900;                         // 0x0139C900
typedef void (__fastcall* PFNLOGSINK)(const char* text);
extern PFNLOGSINK g_pfnLogSink_0143bb2c;                // 0x0143BB2C

struct CString {                                        // MFC-style string: one word
    char* m_pchData;
    CString() : m_pchData(g_pchNil_0139c900) { }
    ~CString();                                         // 0x00C0BA40
    CString* __fastcall Append(const char* text);       // 0x00C0DEC0
    CString* __fastcall AppendNumber(long value);       // 0x00C0DF00
};

struct CTrace {                                         // empty debug-break temporary
    CTrace();                                           // 0x00C16750
};

#define PK_ASSERT_MSG(cond, line_no, message)                                                                  \
    if (!(cond)) {                                                                                               \
        {                                                                                                        \
            CString line;                                                                                        \
            line.Append(".\\PKCBufferedReader.cpp")->Append("(")->AppendNumber(line_no)->Append(") : ")->Append(message)->Append("\n"); \
            if (g_pfnLogSink_0143bb2c)                                                                           \
                g_pfnLogSink_0143bb2c(line.m_pchData);                                                           \
        }                                                                                                        \
        CTrace();                                                                                                \
    }

class PKCMailbox {
public:
    long          GetCount();                           // 0x00C4BE10
    unsigned long GetResult();                          // 0x00C4BC80
};

class PKCBufferedReader {
public:
    long           m_0;
    unsigned char* m_slotOwners;                        // +0x04 (request id per buffer slot, 0xFF = free)
    unsigned char  _pad08[0x14 - 0x08];
    unsigned long  m_slotCount;                         // +0x14
    unsigned char  _pad18[0x48 - 0x18];
    PKCMailbox     m_mailbox;                           // +0x48
    unsigned char  _pad49[0x68 - 0x49];
    long           m_outstanding;                       // +0x68
    void ReturnBuffer(bool* pMoreResults);
};

void PKCBufferedReader::ReturnBuffer(bool* pMoreResults)
{
    PK_ASSERT_MSG(m_outstanding != 0, 56, "Error: There are no requests pending!")
    if (pMoreResults)
        *pMoreResults = m_mailbox.GetCount() != 0;
    unsigned long id = m_mailbox.GetResult();
    PK_ASSERT_MSG(id < m_slotCount, 62, "Error: returned buffer out of scope")
    PK_ASSERT_MSG(m_slotOwners[id] == id, 63, "Error: Request ID out of sync")
    for (unsigned long i = 0; i < m_slotCount; ++i) {
        unsigned long slot = (i + id) % m_slotCount;
        if (m_slotOwners[slot] == id)
            m_slotOwners[slot] = 0xff;
        else
            i = m_slotCount;
    }
    --m_outstanding;
}