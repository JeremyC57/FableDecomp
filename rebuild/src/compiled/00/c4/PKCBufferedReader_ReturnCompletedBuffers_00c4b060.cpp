// PKCBufferedReader::ReturnCompletedBuffers() -- while requests are outstanding and the result mailbox at +0x48 holds results, hand the oldest buffer back (ReturnBuffer); a hand-back that reports no further result traces "Error: Should not be blocked here..." (PKCBufferedReader.cpp line 471) and breaks
// Leaf: ?ReturnCompletedBuffers@PKCBufferedReader@@QAEXXZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4B060, 248 bytes, thiscall `ret`, EH-framed (the trace string, state 0).
// Identity (descriptive; libpk has no PDB or RTTI): the trace names ".\PKCBufferedReader.cpp"; one caller 0x00C4B685 in
//   the reader. The catalog's `PKCBufferedReader_FillBuffer` fits nothing in the body (it fills nothing: it only returns
//   finished buffers, like the landed ReturnAllBuffers 0x00C4B010 but non-blocking), so the name is descriptive.
// Callees: the result mailbox's GetCount 0x00C4BE10 (bound as ?GetCount@PKCMailbox@@QAEJXZ, the model the landed
//   ReturnBuffer uses), PKCBufferedReader::ReturnBuffer(bool*) 0x00C4AC70 (landed); the trace string's `__fastcall`
//   appenders 0x00C0DEC0 / 0x00C0DF00, its destructor 0x00C0BA40, the debugger break temporary 0x00C16750; globals
//   0x0139C900 (empty buffer) and 0x0143BB2C (report hook).
// Shape: `while (m_outstanding)` rotates into a guarded do-while with &m_mailbox held in ebp; the flag is a stack byte.
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

class PKCMailbox {
public:
    long          GetCount();                 // 0x00C4BE10
    unsigned long GetResult();                // 0x00C4BC80
};

class PKCBufferedReader {
public:
    long           m_0;
    unsigned char* m_slotOwners;              // +0x04 (request id per buffer slot, 0xFF = free)
    unsigned char  _pad08[0x14 - 0x08];
    unsigned long  m_slotCount;               // +0x14
    unsigned char  _pad18[0x48 - 0x18];
    PKCMailbox     m_mailbox;                 // +0x48
    unsigned char  _pad49[0x68 - 0x49];
    long           m_outstanding;             // +0x68
    void ReturnBuffer(bool* pMoreResults);    // 0x00C4AC70
    void ReturnCompletedBuffers();
};

void PKCBufferedReader::ReturnCompletedBuffers()
{
    while (m_outstanding != 0) {
        if (m_mailbox.GetCount() == 0)
            break;
        bool more;
        ReturnBuffer(&more);
        if (!more) {
            {
                CDebugStream s(g_pDebugBuffer);
                s << ".\\PKCBufferedReader.cpp" << "(" << 471 << ") : " << "Error: Should not be blocked here..." << "\n";
                if (g_pfnErrorReport)
                    g_pfnErrorReport(s.m_pBuffer);
            }
            CDebugBreak();
        }
    }
}