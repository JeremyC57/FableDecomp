// CBankFileManager::ReadQuotedString(CCharString* out, CDataInputStream* stream) -- parse a "quoted" token from an ini stream: skip to the opening quote and past it, read up to the closing quote into *out and skip that too; false when no opening quote is found
// Leaf: ?ReadQuotedString@CBankFileManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A77D0, length 255 bytes (`ret 8`; `this` is not used -- the callers in
//          CBankFileManager::OpenIniFile pass the manager in ecx).
// Callees: CDataInputStream::ReadUntilCharacter(char, bool*) @ 0x00994DC0 (by value, x2), ~CCharString @ 0x0099EAE0,
//          CCharString::operator= @ 0x0099EFB0, CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (x2, overflow path).
// Layout: the landed ReadUntilCharacter (0x00994DC0) CDataInputStream model (engine/CDataInputStream.h members with the
//          virtual base chain).
// Shape: the first token is read and discarded (temporary destroyed at once); `found` is homed in the dead `stream`
//          argument slot; each quote is skipped by the inlined one-byte ReadChar whose dead character copy VC drops
//          (pointer/count/position updates only); the success path is nested so the `return false` exit is placed last.
#include <string.h>
#pragma intrinsic(memcpy)

class CCharString { public:
    void* PStringData;
    CCharString();                          // 0x0099E4B0
    CCharString(const CCharString& other);  // 0x0099EC30
    ~CCharString();                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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
    CCharString ReadUntilCharacter(char terminator, bool* found);   // 0x00994DC0
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};

class CBankFileManager {
public:
    bool ReadQuotedString(CCharString* out, CDataInputStream* stream);
};

bool CBankFileManager::ReadQuotedString(CCharString* out, CDataInputStream* stream)
{
    bool found;
    stream->ReadUntilCharacter('"', &found);
    if (found) {
        stream->ReadChar();
        *out = stream->ReadUntilCharacter('"', &found);
        if (found)
            stream->ReadChar();
        return true;
    }
    return false;
}