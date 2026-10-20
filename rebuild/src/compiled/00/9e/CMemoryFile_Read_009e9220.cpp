// CMemoryFile::Read(void*, long, CASuspendableProcess*) -- copy `size` bytes from the memory file at the current position into `buffer`, advancing the position; throws CBBBFileException "Null dst parameter" (type 1) for a null buffer and "Cannot read past end of file" (type 3) when the read would pass Length
// Leaf: ?Read@CMemoryFile
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E9220, 258 bytes (`ret 0xc`, virtual).
// Callees: CCharString(const char*, long) 0x0099EBF0, __fastcall operator+(const CCharString&, const char*) 0x0099F600
//   (labelled CCharString::AppendCString), CCharString copy ctor 0x0099EC30, operator const char* 0x0099E4C0, ~CCharString
//   0x0099EAE0, _CxxThrowException 0x00BFEB84 with the CBBBFileException ThrowInfo 0x01369380.
// Layout: the landed CMemoryFile::MovePosition (0x009E9140) local model: Position +0x08, Length +0x0c, DataBuffer (CArray,
//   element block first) +0x10 (engine/CMemoryFile.h is 4 bytes off in retail).
// Shape: the MovePosition exception model (no EH frame); the copy loop advances the buffer PARAMETER itself and reads through
//   an inline CArray operator[] -- `DataBuffer[Position++]` evaluates the index (load, +1, store) before the element block and
//   keeps the buffer in edx / the count in esi (`dec ; jne`).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    operator const char*() const;                       // 0x0099E4C0
    friend CCharString __fastcall operator+(const CCharString& a, const char* b);   // 0x0099F600
};
class CExceptionBase {
public:
    CExceptionBase(const char*) { }
};
class CBBBFileException : public CExceptionBase {
public:
    enum EFileExceptionType { NullParameter = 1, NotOpenForWriting = 2, ReadPastEnd = 3, SeekOutOfRange = 11 };
    EFileExceptionType Type;
    CBBBFileException(CCharString text, EFileExceptionType type) : CExceptionBase(static_cast<const char*>(text)), Type(type) { }
};
class CASuspendableProcess;
template <class T> struct CArray {                     // CArray<T>: the element block first
    T*   Data;
    long Count;
    long Capacity;
    long Grow;
    void SetSize(long size);                           // 0x004897F0
    T& operator[](long index) { return Data[index]; }
};
struct CMemoryFile {
    // vptr at +0x00
    long                   AccessType;                  // +0x04 FileAccessType (1: read only)
    long                   Position;                    // +0x08
    long                   Length;                      // +0x0c
    CArray<unsigned char>  DataBuffer;                  // +0x10
    virtual void Read(void* buffer, long size, CASuspendableProcess* process);
    virtual void Write(const void* buffer, long size, CASuspendableProcess* process);
};

void CMemoryFile::Read(void* buffer, long size, CASuspendableProcess* process)
{
    if (!buffer) {
        CCharString message = CCharString("", -1) + "Null dst parameter";
        throw CBBBFileException(message, CBBBFileException::NullParameter);
    }
    if (size) {
        if (Position + size > Length) {
            CCharString message = CCharString("", -1) + "Cannot read past end of file";
            throw CBBBFileException(message, CBBBFileException::ReadPastEnd);
        }
        for (; size > 0; --size) {
            *(unsigned char*)buffer = DataBuffer[Position++];
            buffer = (unsigned char*)buffer + 1;
        }
    }
}