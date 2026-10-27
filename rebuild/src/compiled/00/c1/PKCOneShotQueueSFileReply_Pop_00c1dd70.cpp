// PKCOneShotQueue<SFileReply>::Pop(SFileReply* item) -- take the single reply: trace "Nothing to return!" (PKCOneShotQueue.h line 28) and break when the queue was never filled, then clear the filled flag, copy the reply into item and return item
// Leaf: ?Pop@?$PKCOneShotQueue@USFileReply@@@@UAEPAUSFileReply@@PAU2@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1DD70, 208 bytes, thiscall `ret 4`, EH-framed (the trace string, state 0).
// Identity (descriptive; libpk has no PDB or RTTI): slot 0 of PKCOneShotQueue<SFileReply>'s vtable 0x012B332C (its only
//   reference; slot 1 Push 0x00C1DE50, slot 2 GetCount 0x00C1DF30, slot 3 IsFull 0x00C1DF40), the reply queue the
//   landed constructor 0x00C1DD50 builds inside CReplyMailbox; the trace names
//   "d:\dev\audio\ver_fable_1.5_b\libpk\PKCOneShotQueue.h". It returns item in eax, as the mailbox Receive does (so the
//   queue interface's slot 0 is `T* Pop(T*)`). The catalog's gapscan name is a placeholder.
// Callees: the trace string's `__fastcall` appenders 0x00C0DEC0 / 0x00C0DF00, its destructor 0x00C0BA40, the debugger
//   break temporary 0x00C16750; globals 0x0139C900 (empty buffer) and 0x0143BB2C (report hook).
extern char* _afxPchNil;                                  // 0x0139C900 (the empty string)
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;                      // 0x0143BB2C

class CString {                                            // the libpk MFC-style string: one word
public:
    char* m_pchData;
    CString() : m_pchData(_afxPchNil) { }
    ~CString();                                            // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);      // 0x00C0DEC0
    CString& __fastcall operator<<(long value);            // 0x00C0DF00
};

struct CDebugBreak {                                       // empty temporary whose constructor breaks into the debugger
    CDebugBreak();                                         // 0x00C16750
};

struct SFileReply {                           // 8 bytes: the answer a PKCASyncFileReader request posts back
    bool m_bSuccess;                          // +0x00
    long m_value;                             // +0x04
    SFileReply() : m_bSuccess(true), m_value(0) {}
};

template<class T> class PKCQueue {            // the queue interface PKCMailbox drives (4 pure slots, no destructor slot)
public:
    virtual T* Pop(T* item) = 0;              // slot 0 (returns item)
    virtual long Push(const T* item) = 0;     // slot 1
    virtual unsigned long GetCount() = 0;     // slot 2
    virtual long IsFull() = 0;                // slot 3
};

template<class T> class PKCOneShotQueue : public PKCQueue<T> {   // 0x10 bytes (libpk PKCOneShotQueue.h)
public:
    T    m_item;                              // +0x04
    long m_bFilled;                           // +0x0c
    PKCOneShotQueue();                        // 0x00C1DD50 (vtable 0x012B332C)
    virtual T* Pop(T* item);                  // slot 0 (0x00C1DD70)
    virtual long Push(const T* item);         // slot 1 (0x00C1DE50)
    virtual unsigned long GetCount();         // slot 2 (0x00C1DF30)
    virtual long IsFull();                    // slot 3 (0x00C1DF40)
};

template<class T> T* PKCOneShotQueue<T>::Pop(T* item)
{
    if (m_bFilled == 0) {
        {
            CString s;
            s << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCOneShotQueue.h" << "(" << 28 << ") : " << "Nothing to return!" << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(s.m_pchData);
        }
        CDebugBreak();
    }
    m_bFilled = 0;
    *item = m_item;
    return item;
}

template SFileReply* PKCOneShotQueue<SFileReply>::Pop(SFileReply*);