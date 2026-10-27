// CVtable_012b33a4::CopyData(void* buffer) -- copy the object's data block (+0x20, size +8) into buffer; -1 when it holds no data, else 0
// Leaf: ?CopyData@CVtable_012b33a4@@UAEJPAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1E090, 44 bytes, thiscall, `ret 4`.
// Identity (descriptive): slot 2 of the non-RTTI vtable 0x012B33A4 (slot 0 its scalar deleting destructor 0x00C1E0F0,
//   slot 1 0x00C1EA50); libpk has no PDB or RTTI, so the class is named by its vtable.
// Shape: the inline `memcpy` intrinsic (`rep movsd` of size/4, `rep movsb` of size&3).
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int size);
#pragma intrinsic(memcpy)

class CVtable_012b33a4 {                        // vtable 0x012B33A4
public:
    virtual ~CVtable_012b33a4();                // slot 0
    virtual void v01();                         // slot 1 (0x00C1EA50)
    virtual long CopyData(void* buffer);        // slot 2
    long          m_04;                         // +0x04
    unsigned long m_size;                       // +0x08
    unsigned char _pad0c[0x20 - 0x0c];
    void*         m_pData;                      // +0x20
};

long CVtable_012b33a4::CopyData(void* buffer)
{
    if (!m_pData)
        return -1;
    memcpy(buffer, m_pData, m_size);
    return 0;
}