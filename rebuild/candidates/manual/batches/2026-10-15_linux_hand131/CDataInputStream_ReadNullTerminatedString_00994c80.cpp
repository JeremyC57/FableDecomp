// CDataInputStream::ReadNullTerminatedString() -- read bytes one at a time until a zero byte, appending each to a string, and return the string by value
// Leaf: ReadNullTerminatedString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994C80, length 149 bytes (`ret 4`: the hidden result pointer).
// Callees: CCharString::CCharString() @ 0x0099E4B0, CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long)
// @ 0x00993CA0 (landed) when the current chunk is empty, CCharString::operator+=(char) @ 0x0099F180 (the manifest's
// "CWideString::operator+=" is propagated), CCharString::CCharString(const CCharString&) @ 0x0099EC30, ~CCharString @ 0x0099EAE0.
// Layout: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0x0c, SourceChunkBytesRemaining +0x14).
// Shape: the inlined one-byte Read(buffer, size) of the landed TransferBinaryIn family (0x007AD7F0); the character is an
// uninitialised local (an out-of-range read leaves it as it was) passed by value as a whole dword; the string is a named
// local copied into the hidden result at the end (no NRVO) and then destroyed. CCharString is modelled locally because
// fable_string.h inlines the copy constructor that retail calls.
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CCharString {
public:
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator+=(char c);                // 0x0099F180
};

struct CDataInputStreamReader : CDataInputStream {   // the engine layout plus the retail member functions
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
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
    CCharString ReadNullTerminatedString();
};

CCharString CDataInputStreamReader::ReadNullTerminatedString()
{
    CCharString text;
    for (;;) {
        char c;
        Read(&c, 1);
        if (c == 0)
            break;
        text += c;
    }
    return text;
}
