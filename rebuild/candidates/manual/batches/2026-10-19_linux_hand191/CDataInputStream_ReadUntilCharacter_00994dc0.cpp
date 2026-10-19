// CDataInputStream::ReadUntilCharacter(char terminator, bool* found) -- read characters into a string until the terminator (which is pushed back by seeking one byte back) or the end of the stream; *found says which
// Leaf: ReadUntilCharacter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994DC0, length 198 bytes (+ int3 padding), `ret 0Ch` (hidden result,
// terminator, found).
// Callees: CCharString::CCharString() @ 0x0099E4B0, CCharString::operator+=(char) @ 0x0099F180, copy constructor
// @ 0x0099EC30, ~CCharString @ 0x0099EAE0, CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0, virtual slot 2
// GetPosition (+8) and slot 1 SetPosition (+4) on `this`.
// Layout: engine/CDataInputStream.h (StreamPos +4, StreamSize +8, CurrentSourcePtr +0xc, SourceChunkStreamPos +0x10,
// SourceChunkBytesRemaining +0x14), redeclared below with the virtual base chain so the slots can be called (the
// header models the vptr as a plain word) -- the same model as the landed ReadLine (0x00994B50).
// Shape: the inlined one-byte Read (64-bit position guard, chunk fast path, overflow call) homes the read character in
// the terminator's argument slot after the terminator is cached in bl (an unread character therefore compares equal);
// a `for (;;)` loop with both exits as `break` merges the two `*found` stores into one copy-out tail (two `return
// result;` statements duplicate it); VC7.1 has no NRVO, so the local result is copied into the hidden return slot and destroyed.
#define CDataInputStream CDataInputStreamLayout
#include "engine/CDataInputStream.h"
#undef CDataInputStream
#include <string.h>
#pragma intrinsic(memcpy)

struct CCharString {
    void* PStringData;
    CCharString();                          // 0x0099E4B0
    CCharString(const CCharString& other);  // 0x0099EC30
    ~CCharString();                         // 0x0099EAE0
    CCharString& operator+=(char c);        // 0x0099F180
};

struct CBaseClass { virtual ~CBaseClass(); };
struct CBaseClassNonCopyable : CBaseClass { virtual ~CBaseClassNonCopyable(); };
struct CDataStream : CBaseClassNonCopyable {
    virtual ~CDataStream();
    virtual void SetPosition(unsigned long position) = 0;   // slot 1 (+4)
    virtual unsigned long GetPosition() const = 0;          // slot 2 (+8)
    virtual void Close() = 0;
};
struct CDataInputStream : CDataStream {
    unsigned long StreamPos;                 // +0x04
    long StreamSize;                         // +0x08
    void* CurrentSourcePtr;                  // +0x0c
    unsigned long SourceChunkStreamPos;      // +0x10
    long SourceChunkBytesRemaining;          // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dest, long n)
    {
        __int64 newPosition = (__int64)(StreamPos + n);
        if (!IsValidPosition(newPosition)) return;
        if (SourceChunkBytesRemaining >= n) {
            memcpy(dest, CurrentSourcePtr, n);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + n;
            SourceChunkBytesRemaining -= n;
            StreamPos += n;
        } else ReadWithSrcChunkOverflow((unsigned char*)dest, n);
    }
    __forceinline char ReadChar() { char value; Read(&value, 1); return value; }
    CCharString ReadUntilCharacter(char terminator, bool* found);
};

CCharString CDataInputStream::ReadUntilCharacter(char terminator, bool* found)
{
    CCharString result;
    for (;;) {
        if (StreamPos == (unsigned long)StreamSize) {
            *found = false;
            break;
        }
        char c = ReadChar();
        if (c == terminator) {
            SetPosition(GetPosition() - 1);
            *found = true;
            break;
        }
        result += c;
    }
    return result;
}
