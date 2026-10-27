// StreamReader::Read(void* buffer, unsigned long size) -- copy the next `size` bytes of the in-memory stream (data +4, cursor +8, size +0xc) and advance; false (nothing copied) when they run past the end
// Leaf: ?Read@StreamReader@@UAE_NPAXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C42720, 70 bytes, thiscall `ret 8`.
// Identity: slot 3 of the two stream-reader vtables 0x012B4F9C / 0x012B4FAC (slot 2 0x00C426E0 clears +4 and +0xc, the
//   landed "Close"); the class name follows the landed StreamReader::AdvancePositionRelative 0x00C426F0 (cursor +8,
//   size +0xc, both unsigned) next to the RIFF chunk walker 0x00C427D0. Descriptive (libpk, no PDB/RTTI).
// Shape: `this` kept in eax (`mov eax,ecx`); memcpy is the /Oi intrinsic (`rep movsd` + `rep movsb`); `xor al,al` / `mov al,1`.
#include <string.h>

class StreamReader {                         // vtables 0x012B4F9C / 0x012B4FAC
public:
    virtual void v00();
    virtual void v01();
    virtual void Close();                    // slot 2 (0x00C426E0)
    virtual bool Read(void* buffer, unsigned long size);
    const unsigned char* m_pBuffer;          // +0x4
    unsigned long m_position;                // +0x8
    unsigned long m_size;                    // +0xc
};

bool StreamReader::Read(void* buffer, unsigned long size)
{
    if (m_position + size > m_size)
        return false;
    memcpy(buffer, m_pBuffer + m_position, size);
    m_position += size;
    return true;
}