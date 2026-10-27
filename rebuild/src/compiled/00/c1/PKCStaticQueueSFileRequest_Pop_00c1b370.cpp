// PKCStaticQueue<SFileRequest>::Pop(SFileRequest* item) -- take the oldest record: trace "Nothing in this queue - you should have checked first!" (PKCStaticQueue.h line 67) and break when the queue is empty, then copy the record at the tail index into item, drop the count and return item
// Leaf: ?Pop@?$PKCStaticQueue@USFileRequest@@@@UAEPAUSFileRequest@@PAU2@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1B370, 225 bytes, thiscall `ret 4`, EH-framed (the trace string, state 0).
// Identity (descriptive; libpk has no PDB or RTTI): slot 0 of PKCStaticQueue<SFileRequest>'s vtable 0x012B2FC8 (slot 1
//   Push 0x00C1AEC0), the class the landed constructor 0x00C1B340 names from this very trace
//   ("d:\dev\audio\ver_fable_1.5_b\libpk\PKCStaticQueue.h"). The catalog's gapscan name is a placeholder.
// Callees: GetTailIndex 0x00C1B460 (catalog "CAIStateGroup_HoverHornetQueen::GetNextCirclePoint", propagated: it is
//   `head - count`, plus the capacity when negative), the trace string's `__fastcall` appenders 0x00C0DEC0 / 0x00C0DF00,
//   its destructor 0x00C0BA40, the debugger break temporary 0x00C16750; globals 0x0139C900 and 0x0143BB2C.
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

struct SFileRequest {                         // seven words: handle, key and five qualifiers (0x1C bytes)
    void* m_hFile;
    void* m_pKey;
    long m_a;
    long m_b;
    long m_c;
    long m_d;
    long m_e;
};

template<class T> class PKCQueue {            // the queue interface PKCMailbox drives (4 pure slots, no destructor slot)
public:
    virtual T* Pop(T* item) = 0;              // slot 0 (returns item)
    virtual long Push(const T* item) = 0;     // slot 1
    virtual unsigned long GetCount() = 0;     // slot 2
    virtual long IsFull() = 0;                // slot 3
};

// libpk PKCStaticQueue.h: 0x14 bytes; vtable 0x012B2FC8 for SFileRequest. The count and capacity are unsigned (retail
// `jb` / `div`); the landed constructor 0x00C1B340 spells them long (same layout).
template<class T> class PKCStaticQueue : public PKCQueue<T> {
public:
    PKCStaticQueue(long capacity);            // 0x00C1B340
    virtual T* Pop(T* item);                  // slot 0 (0x00C1B370)
    virtual long Push(const T* item);         // slot 1 (0x00C1AEC0)
    virtual unsigned long GetCount();         // slot 2 (0x00C1AFB0)
    virtual long IsFull();                    // slot 3 (0x00C1AFC0)
    long GetTailIndex();                      // 0x00C1B460: head - count, wrapped by the capacity
    unsigned long m_capacity;                 // +0x04
    T*            m_pRecords;                 // +0x08
    unsigned long m_count;                    // +0x0c
    unsigned long m_head;                     // +0x10 (the next write slot)
    ~PKCStaticQueue();                        // 0x00C1AEA0 (non-virtual: no destructor slot)
};

template<class T> T* PKCStaticQueue<T>::Pop(T* item)
{
    if (m_count == 0) {
        {
            CString s;
            s << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCStaticQueue.h" << "(" << 67 << ") : " << "Nothing in this queue - you should have checked first!" << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(s.m_pchData);
        }
        CDebugBreak();
    }
    long index = GetTailIndex();
    --m_count;
    *item = m_pRecords[index];
    return item;
}

template SFileRequest* PKCStaticQueue<SFileRequest>::Pop(SFileRequest*);