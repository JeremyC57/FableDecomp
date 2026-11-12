// CCachedFile::Read(void* destination, long size, CASuspendableProcess* process) -- read size bytes at the current
//   position through the cache buffer: a null destination throws CBBBFileException(1) "Null dst parameter", reading past
//   the disk file's length throws CBBBFileException(3) "Cannot read past end of file"; otherwise (refilling the buffer
//   first if it is not valid) copy byte by byte from the buffer, refilling it from the disk file at the current position
//   (at most the buffer's size or what is left of the file) whenever the position leaves the buffered window.
// Leaf: ?Read@CCachedFile@@UAEXPAXJPAVCASuspendableProcess@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5B370, 534 bytes, `ret 0Ch`; CCachedFile vtable 0x012724D4 slot 3.
// Callees: CCharString(const char*, long) 0x0099EBF0, operator+(const CCharString&, const char*) 0x0099F600 (__fastcall),
//   copy ctor 0x0099EC30, operator const char* 0x0099E4C0, ~CCharString 0x0099EAE0, _CxxThrowException 0x00BFEB84
//   (throw info 0x01369380 = CBBBFileException); the disk file's CAFile slots 3 Read, 5 SetPosition, 7 GetPosition,
//   9 GetLength. Strings "" 0x0122D70E, "Null dst parameter" 0x0129A850, "Cannot read past end of file" 0x0129A830.
// Layout: Ego_r.pdb CCachedFile : CAFile with the retail 12-byte STLport DataBuffer: Position +4, BufferStartPosition +8,
//   DataBuffer +0xC, Pathname +0x18, PDiskFile (CCountedPointer<CAFile>) +0x1C, Opened +0x24, BufferValid +0x25.
// Shape: the exception model of the landed siblings CCachedFile::Write 0x00A5AFB0 / MovePosition 0x00A5B0A0 (Ego_r.pdb
//   CBBBFileException(CCharString by value, EFileExceptionType), no EH frame). The copy loop works on locals (`out`,
//   `bytesLeft`): the destination copy is why retail tests eax and then parks it in ebp, and the size copy is why the
//   outer loop test re-reads the size from its argument slot. The first refill evaluates GetLength before GetPosition
//   (spelled `-GetPosition() + GetLength()`), the second GetPosition first (`GetLength() - GetPosition()`), both with
//   the disk file pointer cached in edi; the window test is spelled on the raw end - begin difference (size() first).
//   Enumerator names of EFileExceptionType are descriptive (no PDB carries them).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#include "engine/CAFile.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
class CBBBFileException {
public:
    enum EFileExceptionType { NullParameter = 1, ReadPastEnd = 3 };
    EFileExceptionType Type;
    CBBBFileException(CCharString message, EFileExceptionType type) : Type(type) { (const char*)message; }
};
template <class T> inline const T& Min(const T& a, const T& b) { return a < b ? a : b; }
class CCachedFile : public CAFile { public:
    long                    Position;            // +0x04
    long                    BufferStartPosition; // +0x08
    CArray<unsigned char>   DataBuffer;          // +0x0C
    CWideString             Pathname;            // +0x18
    CCountedPointer<CAFile> PDiskFile;           // +0x1C
    bool                    Opened;              // +0x24
    bool                    BufferValid;         // +0x25
    virtual void Read(void* destination, long size, CASuspendableProcess* process);
};

void CCachedFile::Read(void* destination, long size, CASuspendableProcess* process)
{
    unsigned char* out = (unsigned char*)destination;
    if (destination == 0) {
        CCharString message = CCharString("", -1) + "Null dst parameter";
        throw CBBBFileException(message, CBBBFileException::NullParameter);
    }
    if (size == 0)
        return;
    if (Position + size > PDiskFile->GetLength()) {
        CCharString message = CCharString("", -1) + "Cannot read past end of file";
        throw CBBBFileException(message, CBBBFileException::ReadPastEnd);
    }
    if (!BufferValid) {
        BufferStartPosition = Position;
        PDiskFile->SetPosition(Position);
        long remaining = -PDiskFile->GetPosition() + PDiskFile->GetLength();
        long bufferSize = DataBuffer.size();
        PDiskFile->Read(&DataBuffer[0], Min(remaining, bufferSize), process);
        BufferValid = true;
    }
    long bytesLeft = size;
    while (bytesLeft > 0) {
        while ((unsigned long)(Position - BufferStartPosition) < (unsigned long)(DataBuffer.end() - DataBuffer.begin()) && Position - BufferStartPosition >= 0 && bytesLeft > 0) {
            *out++ = DataBuffer[Position - BufferStartPosition];
            ++Position;
            --bytesLeft;
        }
        if (bytesLeft > 0) {
            BufferStartPosition = Position;
            PDiskFile->SetPosition(Position);
            long remaining = PDiskFile->GetLength() - PDiskFile->GetPosition();
            long bufferSize = DataBuffer.size();
            PDiskFile->Read(&DataBuffer[0], Min(remaining, bufferSize), process);
        }
    }
}