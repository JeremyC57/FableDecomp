// PKCStaticQueue<SDecoderMessage>::Pop() -- take the oldest 0x40-byte decoder message: trace "Nothing in this queue - you should have checked first!" (PKCStaticQueue.h line 67) and break when the queue is empty, then drop the count and return a copy of the record at the oldest index
// Leaf: ?Pop@?$PKCStaticQueue@USDecoderMessage@@@@UAE?AUSDecoderMessage@@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C48E00, 288 bytes, thiscall `ret 4` (the hidden return-record pointer), EH-framed
//   (the trace string, state 0); the next function starts at once (no padding).
// Identity (descriptive; libpk has no PDB or RTTI): slot 0 of PKCStaticQueue<SDecoderMessage>'s vtable 0x012B8024 (its only
//   reference, 0x012B8024), the class of the landed constructor 0x00C48F80, destructor 0x00C48DB0 and Push 0x00C49010 (slot 1);
//   the trace names "d:\dev\audio\ver_fable_1.5_b\libpk\PKCStaticQueue.h". Same body as the landed slot-0 rows of the
//   other instantiations (0x00C49F10, 0x00C4BEB0: `T Take()` there), with the record returned by value.
// Callees: the oldest-index helper 0x00C48F20 (`head - count`, plus capacity when negative; landed under the propagated
//   CAIStateGroup_HoverHornetQueen::GetNextCirclePoint, NAME_FIXES.tsv), the +4 sub-record's copy constructor 0x00C48770
//   (it calls the heap block's copy constructor 0x00C16900, not the operator= 0x00C16950 that SDecoderMessage::operator=
//   0x00C49100 calls, so the record is copy-CONSTRUCTED into the return slot: a by-value `T Pop()`); the trace string's
//   `__fastcall` appenders 0x00C0DEC0 / 0x00C0DF00, its destructor 0x00C0BA40, the debugger break temporary 0x00C16750;
//   globals 0x0139C900 (empty buffer) and 0x0143BB2C (report hook). Spellings follow the landed Push 0x00C49010.
// Shape: SDecoderMessage's implicit copy constructor inlined: +0 long, the +4 sub-record out of line, +0x20 long, the 16-byte
//   record at +0x24 through `lea` addresses (an embedded struct's implicit copy), +0x34..+0x3C longs; the count is decremented
//   before the record pointer (`index << 6` plus the block) is formed.
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

struct CHeapBlock {                             // 8 bytes: owned data pointer and size (PKCAutoDeleteArray)
    CHeapBlock();                               // 0x00C167B0
    CHeapBlock(const CHeapBlock& other);        // 0x00C16900
    ~CHeapBlock();                              // 0x00C167C0
    void*         m_pData;
    unsigned long m_size;
};

struct SMessageArgs {                           // the three zeroed words of the landed 0x00C48F30 / 0x00C48D90
    long m_a;
    long m_b;
    long m_c;
};

struct CUnprovenDecoderRecord_00C48770 {        // 0x1C bytes at SDecoderMessage+4 (ctor 0x00C48D90, copy ctor 0x00C48770)
    CUnprovenDecoderRecord_00C48770(const CUnprovenDecoderRecord_00C48770& other);   // 0x00C48770
    long         m_param;                       // +0x00
    SMessageArgs m_args;                        // +0x04
    CHeapBlock   m_block;                       // +0x10
    long         m_extra;                       // +0x18
};

struct CUnprovenDecoderQuad_00C48E00 {          // 16 bytes at SDecoderMessage+0x24, copied as one embedded struct
    long m_v[4];
};

struct SDecoderMessage {                        // 0x40 bytes: one async-decoder message record
    __forceinline SDecoderMessage(const SDecoderMessage& other)
        : m_type(other.m_type), m_record(other.m_record), m_flags(other.m_flags), m_quad(other.m_quad),
          m_tail0(other.m_tail0), m_tail1(other.m_tail1), m_tail2(other.m_tail2) { }
    long                            m_type;     // +0x00
    CUnprovenDecoderRecord_00C48770 m_record;   // +0x04
    long                            m_flags;    // +0x20
    CUnprovenDecoderQuad_00C48E00   m_quad;     // +0x24
    long                            m_tail0;    // +0x34
    long                            m_tail1;    // +0x38
    long                            m_tail2;    // +0x3c
};

template<class T> class PKCQueue {              // the queue interface PKCMailbox drives (4 pure slots, no destructor slot)
public:
    virtual T Pop() = 0;                        // slot 0
    virtual long Push(const T* item) = 0;       // slot 1
    virtual unsigned long GetCount() = 0;       // slot 2
    virtual long IsFull() = 0;                  // slot 3
};

template<class T> class PKCStaticQueue : public PKCQueue<T> {   // 0x14 bytes (libpk PKCStaticQueue.h)
public:
    PKCStaticQueue(long capacity);              // 0x00C48F80
    virtual T Pop();                            // slot 0 (0x00C48E00)
    virtual long Push(const T* item);           // slot 1 (0x00C49010)
    virtual unsigned long GetCount();           // slot 2 (0x00C48DE0)
    virtual long IsFull();                      // slot 3 (0x00C48DF0)
    int OldestIndex();                          // 0x00C48F20
    unsigned long m_capacity;                   // +0x04
    T*            m_pRecords;                   // +0x08
    unsigned long m_count;                      // +0x0c
    unsigned long m_head;                       // +0x10
    ~PKCStaticQueue();                          // 0x00C48DB0 (non-virtual: no destructor slot)
};

template<class T> T PKCStaticQueue<T>::Pop()
{
    if (m_count == 0) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCStaticQueue.h" << "(" << 67 << ") : "
              << "Nothing in this queue - you should have checked first!" << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    int index = OldestIndex();
    --m_count;
    return m_pRecords[index];
}

template SDecoderMessage PKCStaticQueue<SDecoderMessage>::Pop();