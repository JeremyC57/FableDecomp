// CVtable_012b4f9c::~CVtable_012b4f9c() -- libpk in-memory stream reader: virtual destructor that closes the stream (own vtable, then its own Close() as a tail jump)
// Leaf: ??1CVtable_012b4f9c@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C34780, 11 bytes: `mov dword ptr [ecx],offset vtable 0x012B4F9C ; jmp 0x00C426E0`.
// Identity: a destructor is named by the vtable it stores first. 0x012B4F9C has no complete-object locator (libpk, no PDB),
//   so the class is named by its vtable (the catalog's ??_7CVtable_012b4f9c / landed deleting destructor 0x00C34710). Its
//   slots: 0 deleting dtor 0x00C34710, 1 SetStreamInput(data, size) 0x00C426C0 (data +4, cursor +8 = 0, size +0xC),
//   2 Close() 0x00C426E0 (clears +4 and +0xC), 3 Read(buffer, size) 0x00C42720. The class with vtable 0x012B4FAC derives
//   from it (its slot 5 0x00C42850 calls SetStreamInput directly, slots 6/7 are the inherited Close/Read) and its deleting
//   destructor 0x00C34790 calls this row.
//   0x00C426E0 is a virtual (slots 2 and 6), not a base destructor: no vtable store, and it sits in the vtable. A virtual
//   call inside a destructor binds statically, so `Close()` here is the direct `jmp`. The catalog label
//   "CChunkedFileChunk::~CChunkedFileChunk" is propagated across the byte-twin shape and is wrong.
// Shape: root polymorphic class (no base destructor runs after the body); the only statement is the call, emitted as a
//   tail jump.
class CVtable_012b4f9c {
public:
    virtual ~CVtable_012b4f9c();
    virtual bool SetStreamInput(void* data, unsigned long size);   // 0x00C426C0
    virtual void Close();                                          // 0x00C426E0
    virtual bool Read(void* buffer, unsigned long size);           // 0x00C42720
    const unsigned char* m_pData;           // +0x4
    unsigned long m_position;               // +0x8
    unsigned long m_size;                   // +0xC
};

CVtable_012b4f9c::~CVtable_012b4f9c()
{
    Close();
}
