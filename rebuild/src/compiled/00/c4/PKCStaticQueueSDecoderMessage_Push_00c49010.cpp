// PKCStaticQueue<SDecoderMessage>::Push(const SDecoderMessage* item) -- append a 0x40-byte decoder message: trace "This queue is full!" (PKCStaticQueue.h line 57) and break when the count has reached the capacity, assign the item into the head slot, advance the head modulo the capacity and return the new count
// Leaf: ?Push@?$PKCStaticQueue@USDecoderMessage@@@@UAEJPBUSDecoderMessage@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C49010, 230 bytes, thiscall `ret 4`, EH-framed (the trace string, state 0).
// Identity (descriptive; libpk has no PDB or RTTI): slot 1 of PKCStaticQueue<SDecoderMessage>'s vtable 0x012B8024 (its
//   only reference, 0x012B8028), the class of the landed constructor 0x00C48F80 and destructor 0x00C48DB0; the trace names
//   "d:\dev\audio\ver_fable_1.5_b\libpk\PKCStaticQueue.h". Same body as PKCStaticQueue<SFileRequest>::Push 0x00C1AEC0,
//   except that the 0x40-byte record is assigned through its out-of-line operator= 0x00C49100.
// Callees: the record's operator= 0x00C49100 (landed under the descriptive SQueuedSoundEvent; the queue's ctor/dtor rows
//   name the record SDecoderMessage, so the call is spelled on SDecoderMessage -- see NOTES "Label corrections"); the
//   trace string's `__fastcall` appenders 0x00C0DEC0 / 0x00C0DF00, its destructor 0x00C0BA40, the debugger break
//   temporary 0x00C16750; globals 0x0139C900 (empty buffer) and 0x0143BB2C (report hook).
// Shape: count and capacity are unsigned (`jb`, `div`); the slot is `head << 6` plus the block.
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

struct SDecoderMessage {                        // 0x40 bytes: one async-decoder message record
    SDecoderMessage();                          // 0x00C48F30
    ~SDecoderMessage();                         // 0x00C48740 (destroys the +0x14 PKCAutoDeleteArray)
    SDecoderMessage& operator=(const SDecoderMessage& other);   // 0x00C49100
    long          m_type;
    long          m_param;
    long          m_args[3];
    unsigned char m_block[8];                   // +0x14 PKCAutoDeleteArray<unsigned short> (data pointer, count)
    unsigned char m_payload[0x40 - 0x1c];
};

template<class T> class PKCQueue {              // the queue interface PKCMailbox drives (4 pure slots, no destructor slot)
public:
    virtual T* Pop(T* item) = 0;                // slot 0
    virtual long Push(const T* item) = 0;       // slot 1
    virtual unsigned long GetCount() = 0;       // slot 2
    virtual long IsFull() = 0;                  // slot 3
};

template<class T> class PKCStaticQueue : public PKCQueue<T> {   // 0x14 bytes (libpk PKCStaticQueue.h)
public:
    PKCStaticQueue(long capacity);              // 0x00C48F80
    virtual T* Pop(T* item);                    // slot 0 (0x00C48E00)
    virtual long Push(const T* item);           // slot 1 (0x00C49010)
    virtual unsigned long GetCount();           // slot 2 (0x00C48DE0)
    virtual long IsFull();                      // slot 3 (0x00C48DF0)
    unsigned long m_capacity;                   // +0x04
    T*            m_pRecords;                   // +0x08
    unsigned long m_count;                      // +0x0c
    unsigned long m_head;                       // +0x10
    ~PKCStaticQueue();                          // 0x00C48DB0 (non-virtual: no destructor slot)
};

template<class T> long PKCStaticQueue<T>::Push(const T* item)
{
    if (m_count >= m_capacity) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCStaticQueue.h" << "(" << 57 << ") : " << "This queue is full!" << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    m_pRecords[m_head] = *item;
    m_head = (m_head + 1) % m_capacity;
    return ++m_count;
}

template long PKCStaticQueue<SDecoderMessage>::Push(const SDecoderMessage*);