// PKCOneShotQueue<CUnprovenItem_012b54b4>::Pop(CUnprovenItem_012b54b4* item) -- take the single 16-byte item: trace "Nothing to return!" (PKCOneShotQueue.h line 28) and break when the queue was never filled, then clear the filled flag, copy the item out and return item
// Leaf: ?Pop@?$PKCOneShotQueue@UCUnprovenItem_012b54b4@@@@UAEPAUCUnprovenItem_012b54b4@@PAU2@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3A330, 226 bytes, thiscall `ret 4`, EH-framed (the trace string, state 0).
// Identity: slot 0 of the one-shot queue vtable 0x012B54B4 (its only reference, 0x012B54B4; slot 1 Push 0x00C3A420,
//   slot 2 0x00C3A500, slot 3 0x00C3A510), stored by the landed inline constructor 0x00C3A310 (item words +0x04..+0x10,
//   filled flag +0x14) whose callers 0x00C391A2 / 0x00C3A763 then build a capacity-1 mailbox over it at +0x18. The trace
//   names "d:\dev\audio\ver_fable_1.5_b\libpk\PKCOneShotQueue.h": the same template as PKCOneShotQueue<SFileReply>
//   (vtable 0x012B332C, Pop 0x00C1DD70) over a 16-byte item whose type is unproven. Catalog gapscan name is a placeholder.
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

struct CUnprovenItem_012b54b4 {               // 16 bytes, copied as four words (item type unproven: no PDB covers libpk)
    long m_a;
    long m_b;
    long m_c;
    long m_d;
};

template<class T> class PKCQueue {            // the queue interface PKCMailbox drives (4 pure slots, no destructor slot)
public:
    virtual T* Pop(T* item) = 0;              // slot 0 (returns item)
    virtual void Push(const T* item) = 0;     // slot 1
    virtual unsigned long GetCount() = 0;     // slot 2
    virtual long IsFull() = 0;                // slot 3
};

template<class T> class PKCOneShotQueue : public PKCQueue<T> {   // libpk PKCOneShotQueue.h
public:
    T    m_item;                              // +0x04
    long m_bFilled;                           // +0x04 + sizeof(T)
    PKCOneShotQueue();
    virtual T* Pop(T* item);                  // slot 0
    virtual void Push(const T* item);         // slot 1
    virtual unsigned long GetCount();         // slot 2
    virtual long IsFull();                    // slot 3
};

template<class T> T* PKCOneShotQueue<T>::Pop(T* item)
{
    if (m_bFilled == 0) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCOneShotQueue.h" << "(" << 28 << ") : " << "Nothing to return!" << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    m_bFilled = 0;
    *item = m_item;
    return item;
}

template CUnprovenItem_012b54b4* PKCOneShotQueue<CUnprovenItem_012b54b4>::Pop(CUnprovenItem_012b54b4*);