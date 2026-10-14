// CMemoryBuffer::Copy(unsigned long length, const void* data) -- append `length` bytes to the small buffer (the data at +5, the fill byte at +0x25): an intrinsic memcpy to data + fill, then the fill byte grows by the length
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F1760, length 64 bytes.
// Leaf: Copy
// Shape: the length in edx/ebx, `rep movsd ; rep movsb` with the destination `lea edi,[edi+eax+5]`, the fill byte re-read for the add, `ret 8`.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int size);
#pragma intrinsic(memcpy)

struct CMemoryBuffer {
    unsigned char m_0[5];
    unsigned char m_data[0x20];                    // +0x05
    unsigned char m_fill;                          // +0x25
    void Copy(unsigned long length, const void* data);
};

void CMemoryBuffer::Copy(unsigned long length, const void* data)
{
    memcpy(m_data + m_fill, data, length);
    m_fill += (unsigned char)length;
}