// template<class T> PKCStaticQueue<T>::PKCStaticQueue(long capacity) -- constructor of libpk's fixed-capacity queue, T = SDecoderMessage (vtable 0x012B8024): store the capacity, build the 0x40-byte message records with `new T[capacity]` (count-prefixed block, each record default-constructed through the `eh vector constructor iterator`), zero the count and the head index
// Leaf: ??0?$PKCStaticQueue@USDecoderMessage@@@@QAE@J@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C48F80, 139 bytes (+ int3 padding), __thiscall `ret 4`, returns `this`, EH-framed.
// Identity (sweep1, 2026-10-25): agent439 landed this as the non-template PKCMessageQueue::PKCMessageQueue. Its vtable 0x012B8024 (Pop 0x00C48E00,
//   Push 0x00C49010, GetCount 0x00C48DE0, IsFull 0x00C48DF0; destructor 0x00C48DB0) carries libpk's
//   "d:\dev\audio\ver_fable_1.5_b\libpk\PKCStaticQueue.h" asserts in Pop/Push ("Nothing in this queue - you should have checked
//   first!", "This queue is full!"), the same header as chunk 433's PKCStaticQueue (vtable 0x012B2FC8, ctor 0x00C1B340). Four
//   vtables (0x012B2FC8 / 0x012B8024 / 0x012B80C8 / 0x012B8364) over one header and one body shape are the instantiations of
//   one class template: PKCStaticQueue<SDecoderMessage> (descriptive; no PDB covers libpk). Owner: the async decoder base constructor 0x00C485E0 (capacity 8; its PKCMailbox member at +0x18 is bound to this queue).
//   The manifest label is propagated.
// Callees: operator new[] 0x00BFEB22 (`capacity * 0x40 + 4`), `eh vector constructor iterator` 0x00F3FB96 (element
//   constructor 0x00C48F30, destructor 0x00C48740 = `add ecx,0x14; jmp 0x00C167C0`). Shape (agent439): the new[] result is homed in
//   the dead `capacity` slot for the EH state-0 cleanup; a null block yields a null array (`xor ebx,ebx`); `/EHsc` for the frame.
void* __cdecl operator new[](unsigned int size);   // 0x00BFEB22

struct SDecoderMessage {                        // 0x40 bytes: one async-decoder message record
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

template<class T> PKCStaticQueue<T>::PKCStaticQueue(long capacity)
    : m_capacity(capacity),
      m_pRecords(new T[capacity]),
      m_count(0),
      m_head(0)
{
}

template PKCStaticQueue<SDecoderMessage>::PKCStaticQueue(long);
