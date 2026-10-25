// PKCMessageQueue::PKCMessageQueue(long capacity) -- constructor of the async decoder's fixed-capacity static queue (vtable 0x012B8024): store the capacity, build the 0x40-byte message records with `new[]` (count-prefixed block, each record default-constructed through the `eh vector constructor iterator`), zero the count and the head index
// Leaf: ??0PKCMessageQueue@@QAE@J@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C48F80, 139 bytes (+ int3 padding), __fastcall `ret 4`, returns `this`, EH-framed.
// Library: libpk audio (no PDB/RTTI): descriptive names. The PKCStaticQueue shape of chunk 433 (0x00C1B340) over
//   records with a constructor and destructor; vtable 0x012B8024: Pop 0x00C48E00 (PKCStaticQueue.h "Nothing in this
//   queue - you should have checked first!"), Push 0x00C49010 ("This queue is full!"), GetCount 0x00C48DE0, IsFull
//   0x00C48DF0; the landed destructor 0x00C48DB0 does the matching `delete[]`. Only caller: the decoder base
//   constructor 0x00C485E0 (capacity 8; its PKCMailbox member at +0x18 is bound to this queue). The manifest's
//   "CScriptDefinitionManager_AllocateElementArray" is propagated.
// Callees: operator new[] 0x00BFEB22 (`capacity * 0x40 + 4`), `eh vector constructor iterator` 0x00F3FB96 (element
//   constructor 0x00C48F30, destructor 0x00C48740).
// Shape: initialiser list in declaration order; the new[] result is homed in the dead `capacity` slot for the EH
//   state-0 cleanup; a null block yields a null array (`xor ebx,ebx`).
struct CHeapBlock {
    CHeapBlock();                               // 0x00C167B0
    ~CHeapBlock();                              // 0x00C167C0
    void*         m_pData;
    unsigned long m_size;
};

struct SMessageArgs {
    long m_a;
    long m_b;
    long m_c;
};

struct SDecoderMessage {
    SDecoderMessage();                          // 0x00C48F30
    long          m_type;
    long          m_param;
    SMessageArgs  m_args;
    CHeapBlock    m_block;                      // +0x14
    unsigned char m_payload[0x40 - 0x1c];
};

void* __cdecl operator new[](unsigned int size);   // 0x00BFEB22

class PKCMessageQueue {
public:
    PKCMessageQueue(long capacity);
    virtual void Pop(SDecoderMessage* message);    // slot 0 (0x00C48E00)
    virtual long Push(const SDecoderMessage* message);   // slot 1 (0x00C49010)
    virtual long GetCount();                    // slot 2 (0x00C48DE0)
    virtual long IsFull();                      // slot 3 (0x00C48DF0)

    long             m_capacity;                // +0x04
    SDecoderMessage* m_pRecords;                // +0x08
    long             m_count;                   // +0x0c
    long             m_head;                    // +0x10
};

PKCMessageQueue::PKCMessageQueue(long capacity)
    : m_capacity(capacity),
      m_pRecords(new SDecoderMessage[capacity]),
      m_count(0),
      m_head(0)
{
}