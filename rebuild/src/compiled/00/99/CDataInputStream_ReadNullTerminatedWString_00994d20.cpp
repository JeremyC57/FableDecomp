// CDataInputStream::ReadNullTerminatedWString() -- read 16-bit characters one at a time until a zero one, appending each to a wide string, and return it by value
// Leaf: ReadNullTerminatedWString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994D20, length 155 bytes (`ret 4`: the hidden result pointer).
// Callees: CWideString::CWideString() @ 0x0099AED0 (manifest "CCharString::CCharString" is propagated),
// CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) @ 0x00993CA0 (landed) when the current chunk holds
// fewer than 2 bytes, CWideString::operator+=(wchar_t) @ 0x0099B9A0 (manifest "CCharString::InsertChar" is propagated),
// CWideString::CWideString(const CWideString&) @ 0x0099B720, ~CWideString @ 0x0099B510.
// Layout: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0x0c, SourceChunkBytesRemaining +0x14).
// Shape: the ReadNullTerminatedString twin (0x00994C80) with 2-byte characters: the inlined two-byte Read(buffer, size) of the landed TransferBinaryIn family (0x007AD7F0); the character is an
// uninitialised local (an out-of-range read leaves it as it was) passed by value as a whole dword; the string is a named
// local copied into the hidden result at the end (no NRVO) and then destroyed; the zero test compares the slot in
// memory (`cmp word ptr [esp+8],0`). wchar_t is `unsigned short` (no /Zc:wchar_t in the retail build).
#define FABLE_CDATAINPUTSTREAM_FRIEND struct CDataInputStreamReader   // reaches the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0)
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CWideString {
public:
    void* PStringData;
    CWideString();                                  // 0x0099AED0
    CWideString(const CWideString& other);          // 0x0099B720
    ~CWideString();                                 // 0x0099B510
    CWideString& operator+=(unsigned short c);      // 0x0099B9A0
};

struct CDataInputStreamReader : CDataInputStream {   // the engine layout plus the retail member functions
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    CWideString ReadNullTerminatedWString();
};

CWideString CDataInputStreamReader::ReadNullTerminatedWString()
{
    CWideString text;
    for (;;) {
        unsigned short c;
        Read(&c, 2);
        if (c == 0)
            break;
        text += c;
    }
    return text;
}