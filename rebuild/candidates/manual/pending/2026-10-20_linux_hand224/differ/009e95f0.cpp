// CMemoryFile::Write(const void*, long, CASuspendableProcess*) -- copy bytes into the memory file at the current position, growing it
// Grade: DIFFER(ours v retail) -- register choice only: `mov ecx,[esi+8] ; add ecx,ebx` for the new end (retail `mov edx,[esi+8] ; lea ecx,[edx+ebx]`) and the not-writable throw rotates eax/ecx/edx around the out-of-line CBBBFileException ctor
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
    enum EFileExceptionType { NullParameter = 1, NotOpenForWriting = 8, ReadPastEnd = 3, SeekOutOfRange = 11 };
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

void CMemoryFile::Write(const void* buffer, long size, CASuspendableProcess* process)
{
    if (!buffer) {
        CCharString message = CCharString("", -1) + "Null src parameter";
        throw CBBBFileException(message, CBBBFileException::NullParameter);
    }
    if (size) {
        long end = size + Position;
        __int64 checked = end;
        if (checked < 0 || checked > 0x7FFFFFFF)
            return;
        if (AccessType == 1) {
            CCharString message = CCharString("", -1) + "File is not open for writing";
            throw CBBBFileException(message, CBBBFileException::NotOpenForWriting);
        }
        if (end > Length) {
            Length = end;
            DataBuffer.SetSize(end);
        }
        for (; size > 0; --size) {
            DataBuffer[Position++] = *(const unsigned char*)buffer;
            buffer = (const unsigned char*)buffer + 1;
        }
    }
}
