// CVtable_012b4f4c::Read(unsigned long count, long unused, void* a, void* b, void* c, void* d) -- queue a read of count units (count times the manager's unit size, this object's slot 2) of this handle's file (key +8) on the file manager (+4)
// Leaf: ?Read@CVtable_012b4f4c@@UAE_NKJPAX000@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C32F60, 52 bytes, thiscall, `ret 18h`.
// Identity (descriptive): slot 1 of the non-RTTI vtable 0x012B4F4C (see 0x00C32F40); slot 2 is 0x00C32F30
//   (`mov ecx,[ecx+4] ; jmp 0x00C19F90`, the manager's unit-size getter). libpk has no PDB or RTTI.
// Callees: this object's slot 2 (+8), the default file manager's request call @ 0x00C1A110 (EH-framed, six arguments:
//   under its lock it looks the key up and posts a seven-word request to the mailbox at +0x58; not landed).
// Shape: right-to-left evaluation pushes the four pass-through arguments before the slot-2 call; the product
//   `GetUnitSize() * count` is `imul eax,[count]`.
struct CFileManager_00c1a070 {
    bool QueueRead(unsigned long key, unsigned long size, void* a, void* b, void* c, void* d);   // 0x00C1A110
};

class CVtable_012b4f4c {                        // vtable 0x012B4F4C
public:
    virtual bool GetSize(unsigned long* size);  // slot 0 (0x00C32F40)
    virtual bool Read(unsigned long count, long unused, void* a, void* b, void* c, void* d);   // slot 1
    virtual unsigned long GetUnitSize();        // slot 2 (0x00C32F30)
    CFileManager_00c1a070* m_pManager;          // +0x04
    unsigned long          m_key;               // +0x08
};

bool CVtable_012b4f4c::Read(unsigned long count, long unused, void* a, void* b, void* c, void* d)
{
    return m_pManager->QueueRead(m_key, GetUnitSize() * count, a, b, c, d);
}