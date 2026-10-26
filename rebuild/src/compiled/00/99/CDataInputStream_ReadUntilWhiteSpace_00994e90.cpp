// CDataInputStream::ReadUntilWhiteSpace() -- read characters into a string up to (not including) the next space, tab, CR or LF, which is pushed back; stops at the end of the stream
// Leaf: ReadUntilWhiteSpace
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994E90, length 205 bytes (+ int3 padding), `ret 4` (hidden result).
// Callees: CCharString::CCharString() @ 0x0099E4B0, CCharString::operator+=(char) @ 0x0099F180, copy constructor
// @ 0x0099EC30, ~CCharString @ 0x0099EAE0, CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0, virtual slot 2
// GetPosition (+8) and slot 1 SetPosition (+4) on `this`.
// Layout: engine/CDataInputStream.h (StreamPos +4, StreamSize +8, CurrentSourcePtr +0xc, SourceChunkStreamPos +0x10,
// SourceChunkBytesRemaining +0x14), redeclared below with the virtual base chain so the slots can be called (the
// header models the vptr as a plain word) -- the same model as the landed ReadLine (0x00994B50).
// Shape: the character comes from the inlined ReadChar() (one-byte Read into a frame byte, value kept in bl; a
// failed read leaves the frame byte stale and VC7.1 keeps the previous bl); the four-way whitespace test is
// written inline; the pushed-back character is un-read with SetPosition(GetPosition() - 1); no NRVO in VC7.1.
#define CDataInputStream CDataInputStreamLayout
#include "engine/CDataInputStream.h"
#undef CDataInputStream
#include <string.h>
#pragma intrinsic(memcpy)

class CCharString { public:
    void* PStringData;
    CCharString();                          // 0x0099E4B0
    CCharString(const CCharString& other);  // 0x0099EC30
    ~CCharString();                         // 0x0099EAE0
    CCharString& operator+=(char c);        // 0x0099F180
};

class CBaseClass { public: virtual ~CBaseClass(); };
struct CBaseClassNonCopyable : CBaseClass { virtual ~CBaseClassNonCopyable(); };
struct CDataStream : CBaseClassNonCopyable {
    virtual ~CDataStream();
    virtual void SetPosition(unsigned long position) = 0;   // slot 1 (+4)
    virtual unsigned long GetPosition() const = 0;          // slot 2 (+8)
    virtual void Close() = 0;
};
class CDataInputStream : public CDataStream { public:
    unsigned long StreamPos;                 // +0x04
    long StreamSize;                         // +0x08
    void* CurrentSourcePtr;                  // +0x0c
    unsigned long SourceChunkStreamPos;      // +0x10
    long SourceChunkBytesRemaining;          // +0x14
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
    CCharString ReadUntilWhiteSpace();
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};

CCharString CDataInputStream::ReadUntilWhiteSpace()
{
    CCharString result;
    while (StreamPos != (unsigned long)StreamSize) {
        char c = ReadChar();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            SetPosition(GetPosition() - 1);
            break;
        }
        result += c;
    }
    return result;
}