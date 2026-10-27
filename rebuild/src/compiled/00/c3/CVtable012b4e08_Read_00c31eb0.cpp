// CVtable_012b4e08::Read(void* buffer, unsigned long size) -- copy size bytes from the read cursor (+8) into buffer and advance it; when fewer than size bytes remain (+0xC) the stream is emptied and false returned
// Leaf: ?Read@CVtable_012b4e08@@UAE_NPAXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C31EB0, 73 bytes, thiscall, `ret 8`, bool result.
// Identity (descriptive): slot 0 of the non-RTTI vtable 0x012B4E08 (stored by 0x00C31F1F and 0x00C31F88): a memory
//   input stream over a byte block. libpk has no PDB or RTTI, so the class is named by its vtable.
// Shape: `this` kept in eax; the inline `memcpy` intrinsic; the remaining count and the cursor re-read after the copy.
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int size);
#pragma intrinsic(memcpy)

class CVtable_012b4e08 {                        // vtable 0x012B4E08
public:
    virtual bool Read(void* buffer, unsigned long size);   // slot 0
    long           m_04;                        // +0x04
    unsigned char* m_pCursor;                   // +0x08
    unsigned long  m_remaining;                 // +0x0c
};

bool CVtable_012b4e08::Read(void* buffer, unsigned long size)
{
    if (size > m_remaining) {
        m_remaining = 0;
        return false;
    }
    memcpy(buffer, m_pCursor, size);
    m_remaining -= size;
    m_pCursor += size;
    return true;
}