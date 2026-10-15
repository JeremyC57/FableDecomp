// CMemoryDataInputStream::ReadNullTerminatedWString() -- read a null-terminated wide string at the current position and advance past it (terminator included)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994960, 144 bytes (padding included).
// Leaf: ?ReadNullTerminatedWString@CMemoryDataInputStream@@UAE?AVCWideString@@XZ
// Shape: the wide twin of the landed ReadNullTerminatedString (0x00995D30): a CWideString local is
// built from StreamSource + GetPosition() (a named local, vtable slot 2; CWideString(const wchar_t*) 0x0099B6B0),
// its byte size (0x0099B240, (end - begin) rounded to whole characters) plus the 2-byte terminator is
// the advance; when positive the new position is range-checked as a 64-bit value against
// [0, 0x7fffffff] and the chunk window (chunkBase += delta, chunkLength -= delta, position = new) moves
// with it. The local is copied into the hidden return slot (0x0099B720) and destroyed (0x0099B510).
#include <stddef.h>    // wchar_t (a typedef under VC7.1 without /Zc:wchar_t)

struct CWideStringData;

struct CWideString {
    CWideStringData* PStringData;
    CWideString(const wchar_t* text);           // 0x0099B6B0
    CWideString(const CWideString& other);      // 0x0099B720
    ~CWideString();                             // 0x0099B510
    long GetDataSize() const;                   // 0x0099B240: size in bytes
};

struct CMemoryDataInputStream {
    virtual void v00();
    virtual void v01();
    virtual long GetPosition();                 // slot 2
    long  position;                             // +0x04
    long  sourceLength;                         // +0x08
    char* chunkBase;                            // +0x0c
    long  chunkPosition;                        // +0x10
    long  chunkLength;                          // +0x14
    char* StreamSource;                         // +0x18
    virtual CWideString ReadNullTerminatedWString();
};

CWideString CMemoryDataInputStream::ReadNullTerminatedWString()
{
    long startPosition = GetPosition();
    CWideString temp((const wchar_t*)(StreamSource + startPosition));
    long consumed = temp.GetDataSize() + 2;
    if (consumed > 0) {
        __int64 newPosWide = GetPosition() + (unsigned long)consumed;
        if (newPosWide >= 0 && newPosWide <= 0x7fffffff) {
            long newPos = (long)newPosWide;
            long oldPos = position;
            chunkBase += newPos - oldPos;
            chunkLength += oldPos - newPos;
            position = newPos;
        }
    }
    return temp;
}