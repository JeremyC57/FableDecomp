// PKCOneShotQueue<SFileReply>::Push(const SFileReply* item) -- store the single reply: trace "Already filled this queue!" (PKCOneShotQueue.h line 35) and break when the queue already holds one, then copy the two-word reply in and set the filled flag
// Leaf: ?Push@?$PKCOneShotQueue@USFileReply@@@@UAEXPBUSFileReply@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1DE50, 210 bytes, thiscall `ret 4`, EH-framed (the trace string, state 0).
// Identity (descriptive; libpk has no PDB or RTTI): slot 1 of PKCOneShotQueue<SFileReply>'s vtable 0x012B332C (its only
//   reference, 0x012B3330; slot 0 Pop 0x00C1DD70, slot 2 GetCount 0x00C1DF30, slot 3 IsFull 0x00C1DF40), the reply
//   queue the landed constructor 0x00C1DD50 builds inside CReplyMailbox; the trace names
//   "d:\dev\audio\ver_fable_1.5_b\libpk\PKCOneShotQueue.h". Retail sets no result register (eax is left holding item), so
//   Push returns void here; the landed PKCQueue declarations spell slot 1 `long` (the mailbox Send ignores the result).
// Callees: the trace string's `__fastcall` appenders 0x00C0DEC0 / 0x00C0DF00, its destructor 0x00C0BA40, the debugger
//   break temporary 0x00C16750; globals 0x0139C900 (empty buffer) and 0x0143BB2C (report hook).
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

struct SFileReply {                           // 8 bytes: the answer a PKCASyncFileReader request posts back
    bool m_bSuccess;                          // +0x00
    long m_value;                             // +0x04
    SFileReply() : m_bSuccess(true), m_value(0) {}
};

template<class T> class PKCQueue {            // the queue interface PKCMailbox drives (4 pure slots, no destructor slot)
public:
    virtual T* Pop(T* item) = 0;              // slot 0 (returns item)
    virtual void Push(const T* item) = 0;     // slot 1
    virtual unsigned long GetCount() = 0;     // slot 2
    virtual long IsFull() = 0;                // slot 3
};

template<class T> class PKCOneShotQueue : public PKCQueue<T> {   // 0x10 bytes (libpk PKCOneShotQueue.h)
public:
    T    m_item;                              // +0x04
    long m_bFilled;                           // +0x0c
    PKCOneShotQueue();                        // 0x00C1DD50 (vtable 0x012B332C)
    virtual T* Pop(T* item);                  // slot 0 (0x00C1DD70)
    virtual void Push(const T* item);         // slot 1 (0x00C1DE50)
    virtual unsigned long GetCount();         // slot 2 (0x00C1DF30)
    virtual long IsFull();                    // slot 3 (0x00C1DF40)
};

template<class T> void PKCOneShotQueue<T>::Push(const T* item)
{
    if (m_bFilled != 0) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCOneShotQueue.h" << "(" << 35 << ") : " << "Already filled this queue!" << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    m_item = *item;
    m_bFilled = 1;
}

template void PKCOneShotQueue<SFileReply>::Push(const SFileReply*);