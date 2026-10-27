// PKCBufferedReader::OnReadComplete(bool success, unsigned char* pRequestId) -- the reader's read-completion callback: a failed read traces "Error: Read failed" (PKCBufferedReader.cpp line 5) and breaks; the request id byte is widened to a word and sent to the reader's result mailbox at +0x48
// Leaf: ?OnReadComplete@PKCBufferedReader@@UAEX_NPAE@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4AB90, 213 bytes, thiscall `ret 8`, EH-framed (the trace string, state 0).
// Identity (descriptive method name; libpk has no PDB or RTTI): the only slot of PKCBufferedReader's vtable 0x012B8458
//   (stored by the landed constructor 0x00C4B300 and destructor 0x00C4B270, whose notes record that slot 0 is this byte-flag
//   member and not a deleting destructor); the trace names ".\PKCBufferedReader.cpp". The id it posts is what the landed
//   ReturnBuffer 0x00C4AC70 takes back out of the same mailbox (GetResult 0x00C4BC80). Catalog gapscan name is a placeholder.
// Callees: the mailbox send 0x00C4BD70 (spelled as its landed catalog name CMailboxQueue_00c4bd70::
//   ProcessMessage_CMailboxQueue_00c4bd70; it is the PKCMailbox Send shape), the trace string's `__fastcall` appenders
//   0x00C0DEC0 / 0x00C0DF00, its destructor 0x00C0BA40, the debugger break temporary 0x00C16750 (it lands in the dead
//   `success` argument slot); globals 0x0139C900 (empty buffer) and 0x0143BB2C (report hook).
struct CDebugStream {
    char* m_pBuffer;
    CDebugStream(char* buffer) : m_pBuffer(buffer) { }
    CDebugStream& __fastcall operator<<(const char* text); // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(long value);       // 0x00C0DF00
    ~CDebugStream();                                       // 0x00C0BA40
};
struct CDebugBreak { CDebugBreak(); };                     // 0x00C16750
extern char* g_pDebugBuffer;                               // 0x0139C900
extern void (__fastcall* g_pfnErrorReport)(const char*);   // 0x0143BB2C

struct CMailboxQueue_00c4bd70 {               // the reader's result mailbox (PKCMailbox shape: lock, queue +0x18, semaphore +0x1c)
    unsigned char _body[0x20];
    long ProcessMessage_CMailboxQueue_00c4bd70(void* message);   // 0x00C4BD70 (send; 1 when the queue is full)
};

class PKCBufferedReader {                     // libpk PKCBufferedReader.cpp; vtable 0x012B8458 (one slot)
public:
    unsigned char          _pad04[0x48 - 0x04];
    CMailboxQueue_00c4bd70 m_results;          // +0x48
    long                   m_outstanding;      // +0x68
    virtual void OnReadComplete(bool success, unsigned char* pRequestId);   // slot 0 (0x00C4AB90)
};

void PKCBufferedReader::OnReadComplete(bool success, unsigned char* pRequestId)
{
    if (!success) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << ".\\PKCBufferedReader.cpp" << "(" << 5 << ") : " << "Error: Read failed" << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    unsigned long id = *pRequestId;
    m_results.ProcessMessage_CMailboxQueue_00c4bd70(&id);
}