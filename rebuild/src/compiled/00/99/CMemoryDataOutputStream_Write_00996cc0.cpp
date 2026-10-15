// CMemoryDataOutputStream::Write(const void*, long) -- append/overwrite bytes at the current position, growing the buffer as needed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00996CC0, 129 bytes.
// Shape: a positive size is range-checked as a 64-bit end position ((__int64)CurrentPosition +
// size within [0, 0x7fffffff]); when the end passes the CArray<char> size the vector grows with
// resize(end, char()) (0x004122D0, the fill temporary sits in the dead size-argument slot); the bytes
// are copied with an inlined memcpy (rep movsd + rep movsb) to GetData() + CurrentPosition (GetData =
// `size() ? start : (char*)this`, 24e select) and the position advances. Offsets follow the landed
// ctor 0x00996AF0 / GetPosition 0x009945B0 (position at +0x14; the generated header's +0x18 is off).
#include <string.h>

struct CArrayChar {                                // CArray<char> (STLport vector<char>)
    char* m_start;                                 // +0
    char* m_finish;                                // +4
    char* m_endOfStorage;                          // +8
    unsigned long size() const { return m_finish - m_start; }
    char* GetData() { return size() ? m_start : (char*)this; }
    void resize(unsigned long newSize, const char& fill = char());   // 0x004122D0
};

struct CMemoryDataOutputStream {
    virtual void v00();
    void*         m_lineTerminator;                // +0x04 CCharString
    CArrayChar    Data;                            // +0x08
    unsigned long CurrentPosition;                 // +0x14
    void Write(const void* data, long size);
};

void CMemoryDataOutputStream::Write(const void* data, long size)
{
    if (size > 0) {
        __int64 end = (__int64)CurrentPosition + size;
        if (end >= 0 && end <= 0x7fffffff) {
            unsigned long newEnd = CurrentPosition + size;
            if (newEnd > Data.size())
                Data.resize(newEnd);
            memcpy(Data.GetData() + CurrentPosition, data, size);
            CurrentPosition += size;
        }
    }
}