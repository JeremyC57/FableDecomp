// Grade: DIFFER(533v534) -- control flow and calls match; retail keeps size in ebx and the destination in ebp (ours swap them), reuses dead argument slots for the throw-path message temporaries differently, and its second refill calls GetPosition before GetLength (variant probe/cf_d.cpp gets that order but loses the file-pointer caching)
// CCachedFile::Read(void* destination, long size, CASuspendableProcess* process) -- read size bytes at the current
//   position through the cache buffer: a null destination throws CBBBFileException(1), reading past the file's end throws
//   CBBBFileException(3); otherwise copy byte by byte from the buffer, refilling it from the disk file (at the current
//   position, at most the buffer's size or what is left of the file) whenever the position leaves the buffered window
// Leaf: ?Read@CCachedFile@@UAEXPAXJPAVCASuspendableProcess@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5B370, 534 bytes; `ret 0Ch`.
// Callees: CCharString(const char*, long) 0x0099EBF0, operator+(const CCharString&, const char*) 0x0099F600 (__fastcall,
//   hidden result in ecx, left operand in edx), copy ctor 0x0099EC30, operator const char* 0x0099E4C0, ~CCharString
//   0x0099EAE0, _CxxThrowException 0x00BFEB84 (throw info 0x01369380, CBBBFileException, 4 bytes); the disk file's (CAFile)
//   slots 3 Read, 5 SetPosition, 7 GetPosition, 9 GetLength. Messages "Null dst parameter" 0x0129A850, "Cannot read past
//   end of file" 0x0129A830, prefixed with the empty string 0x0122D70E.
// Layout: Ego_r.pdb CCachedFile (44 bytes) with the retail 12-byte DataBuffer: vptr, Position +4, BufferStartPosition +8,
//   DataBuffer {start +0xc, end +0x10, capacity +0x14}, Pathname +0x18, PDiskFile {Data +0x1c, Info +0x20}, Opened +0x24,
//   BufferValid +0x25.
// Shape: the CBBBFileException / by-value ReportFileError model of the landed CDiskFileWin32::Read 0x0099A960.
struct CCharString {
    char* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    operator const char*() const;                      // 0x0099E4C0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
__forceinline void ReportFileError(CCharString message)
{
    (const char*)message;
}
struct CExceptionBase { };
struct CBBBFileException : CExceptionBase {
    long Error;
    __forceinline CBBBFileException(long error, const CCharString& message) : Error(error) { ReportFileError(message); }
};
enum {
    BBB_FILE_ERROR_NULL_DESTINATION = 1,
    BBB_FILE_ERROR_READ_PAST_END    = 3
};
class CASuspendableProcess;
struct CAFile {
    virtual ~CAFile();
    virtual void f01(); virtual void f02();
    virtual void Read(void* destination, long size, CASuspendableProcess* process);   // slot 3 (+0xc)
    virtual void f04();
    virtual void SetPosition(long position);           // slot 5 (+0x14)
    virtual void f06();
    virtual long GetPosition();                        // slot 7 (+0x1c)
    virtual void f08();
    virtual long GetLength();                          // slot 9 (+0x24)
};
struct CArrayUChar {                                   // CArray<unsigned char> (retail 12 bytes)
    unsigned char* Start;
    unsigned char* End;
    unsigned char* Capacity;
    unsigned long size() const { return End - Start; }
};
template <class T> inline const T& Min(const T& a, const T& b) { return a < b ? a : b; }
struct CCachedFile {
    virtual void Read(void* destination, long size, CASuspendableProcess* process);
    long        Position;                              // +0x04
    long        BufferStartPosition;                   // +0x08
    CArrayUChar DataBuffer;                            // +0x0c
    void*       Pathname;                              // +0x18 CWideString
    CAFile*     PDiskFile;                             // +0x1c CCountedPointer<CAFile>::Data
    void*       PDiskFileInfo;                         // +0x20
    bool        Opened;                                // +0x24
    bool        BufferValid;                           // +0x25
};

void CCachedFile::Read(void* destination, long size, CASuspendableProcess* process)
{
    if (destination == 0) {
        CCharString message = CCharString("") + "Null dst parameter";
        throw CBBBFileException(BBB_FILE_ERROR_NULL_DESTINATION, message);
    }
    if (size == 0)
        return;
    if (Position + size > PDiskFile->GetLength()) {
        CCharString message = CCharString("") + "Cannot read past end of file";
        throw CBBBFileException(BBB_FILE_ERROR_READ_PAST_END, message);
    }
    if (!BufferValid) {
        BufferStartPosition = Position;
        PDiskFile->SetPosition(Position);
        long remaining = PDiskFile->GetLength() - PDiskFile->GetPosition();
        long bufferSize = DataBuffer.size();
        PDiskFile->Read(DataBuffer.Start, Min(remaining, bufferSize), process);
        BufferValid = true;
    }
    while (size > 0) {
        while ((unsigned long)(Position - BufferStartPosition) < DataBuffer.size() && Position - BufferStartPosition >= 0 && size > 0) {
            *(unsigned char*)destination = DataBuffer.Start[Position - BufferStartPosition];
            destination = (unsigned char*)destination + 1;
            ++Position;
            --size;
        }
        if (size > 0) {
            BufferStartPosition = Position;
            PDiskFile->SetPosition(Position);
            long remaining = PDiskFile->GetLength() - PDiskFile->GetPosition();
            long bufferSize = DataBuffer.size();
            PDiskFile->Read(DataBuffer.Start, Min(remaining, bufferSize), process);
        }
    }
}
