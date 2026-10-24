// CBuffer_00c29af0::~CBuffer_00c29af0() -- deletes the block at +0x4 and zeroes the three fields
// Leaf: ??1CBuffer_00c29af0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C29AF0, 42 bytes.
// Callee: operator delete @ 0x00BFE9BC (landed).  Callers: only EH unwind funclets in the
// 0x0121xxxx cleanup area (`jmp` into this destructor), i.e. a stack object's destructor.
// Destructor of a small owning container -- the shape of VC7.1's vector `_Tidy` (allocator byte at
// +0, first/last/end pointers at +4/+8/+0xc): release the heap block at +0x4 under an explicit null
// test, then clear the pointer and the two other words.  The element type is not identifiable from
// this body, so the class keeps the descriptive name of the eleven landed byte twins (0x00C14460,
// 0x00C29CA0, ...).  The manifest label Stack<unsigned long,64>::~Stack is propagated: the PDB's
// Stack<unsigned long,64> is an empty 1-byte struct, it has no members at +4..+0xc.
struct CBuffer_00c29af0 {
    unsigned char _pad[0x4];
    unsigned long* m_data;                 // +0x4
    unsigned long m_count;                 // +0x8
    unsigned long m_capacity;              // +0xc
    ~CBuffer_00c29af0();
};

CBuffer_00c29af0::~CBuffer_00c29af0()
{
    if (m_data)
        delete m_data;
    m_data = 0;
    m_count = 0;
    m_capacity = 0;
}