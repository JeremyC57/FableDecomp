// template<class T> PKCStaticQueue<T>::~PKCStaticQueue() -- non-virtual destructor of libpk's fixed-capacity queue, T = SDecoderMessage: re-store the queue's vtable 0x012B8024 and destroy the 0x40-byte message records (count-prefixed `delete[]`: `eh vector destructor iterator` with the message destructor, then operator delete[] of the block)
// Leaf: ??1?$PKCStaticQueue@USDecoderMessage@@@@QAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C48DB0, thiscall.
// Retail bytes:  mov eax,[ecx+8] ; test eax,eax ; mov [ecx],vftable ; je ret ; push 0x00C48740 (~SDecoderMessage) ; push [eax-4] (count) ;
//   push 0x40 ; push eax ; call `eh vector destructor iterator' 0x00BFEED8 ; push eax-4 ; call operator delete[] 0x00BFEB1C ; ret
// Identity (sweep1, 2026-10-25; FINDINGS chunk 439 "C48DB0 = the PKCMessageQueue dtor"): vtable 0x012B8024 is the one
//   the landed constructor 0x00C48F80 installs; its Pop 0x00C48E00 and Push 0x00C49010 carry libpk's
//   "d:\dev\audio\ver_fable_1.5_b\libpk\PKCStaticQueue.h" asserts ("Nothing in this queue - you should have checked first!",
//   "This queue is full!"), the same file as chunk 433's PKCStaticQueue (vtable 0x012B2FC8, ctor 0x00C1B340, dtor 0x00C1AEA0).
//   Four distinct vtables (0x012B2FC8 / 0x012B8024 / 0x012B80C8 / 0x012B8364) over one header file and one body shape are the
//   instantiations of one class template, so this is PKCStaticQueue<SDecoderMessage> (descriptive; no PDB covers libpk). Owner: the async decoder (base constructor 0x00C485E0, capacity 8).
// Callees: `eh vector destructor iterator` 0x00BFEED8 (element size 0x40, the record destructor 0x00C48740 = `add ecx,0x14; jmp
//   0x00C167C0`, i.e. the implicit ~SDecoderMessage destroying its +0x14 PKCAutoDeleteArray), operator delete[] 0x00BFEB1C. `/EHsc` gives
//   the iterator form of the vector delete (without it the lane unrolls the loop inline).
// Label correction: the landed row was CEntryTableOwner_00c48db0::~CEntryTableOwner_00c48db0 (no `// Leaf:` line).
void __cdecl operator delete[](void* block);  // 0x00BFEB1C (without it VC7.1 calls scalar ??3)

struct SDecoderMessage {                        // 0x40 bytes: one async-decoder message record (agent439's 0x00C48F80 model)
    SDecoderMessage();                          // 0x00C48F30
    ~SDecoderMessage();                         // 0x00C48740 (destroys the +0x14 PKCAutoDeleteArray)
    long          m_type;
    long          m_param;
    long          m_args[3];
    unsigned char m_block[8];                   // +0x14 PKCAutoDeleteArray<unsigned short> (data pointer, count)
    unsigned char m_payload[0x40 - 0x1c];
};

template<class T> class PKCQueue {              // the queue interface PKCMailbox drives (4 pure slots, no destructor slot)
public:
    virtual void Pop(T* item) = 0;              // slot 0
    virtual long Push(const T* item) = 0;       // slot 1
    virtual unsigned long GetCount() = 0;       // slot 2
    virtual long IsFull() = 0;                  // slot 3
};

template<class T> class PKCStaticQueue : public PKCQueue<T> {   // 0x14 bytes (libpk PKCStaticQueue.h)
public:
    PKCStaticQueue(long capacity);              // 0x00C48F80
    virtual void Pop(T* item);                  // slot 0 (0x00C48E00)
    virtual long Push(const T* item);           // slot 1 (0x00C49010)
    virtual unsigned long GetCount();           // slot 2 (0x00C48DE0)
    virtual long IsFull();                      // slot 3 (0x00C48DF0)
    long m_capacity;                            // +0x04
    T*   m_pRecords;                            // +0x08
    long m_count;                               // +0x0c
    long m_head;                                // +0x10
    ~PKCStaticQueue();                          // 0x00C48DB0 (non-virtual: no destructor slot)
};

template<class T> PKCStaticQueue<T>::~PKCStaticQueue()
{
    delete[] m_pRecords;
}

template PKCStaticQueue<SDecoderMessage>::~PKCStaticQueue();
