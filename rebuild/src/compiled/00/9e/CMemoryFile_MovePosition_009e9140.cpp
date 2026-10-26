// CMemoryFile::MovePosition(long delta) -- move the read/write position by `delta`, throwing a CBBBFileException (type 11) when it would leave the file
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E9140, length 219 bytes (`ret 4`).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, `__fastcall operator+(const CCharString&, const char*)` @ 0x0099F600
// (hidden result in ecx, labelled CCharString::AppendCString), CCharString copy ctor @ 0x0099EC30, operator const char* @ 0x0099E4C0,
// ~CCharString @ 0x0099EAE0, _CxxThrowException @ 0x00BFEB84 with the CBBBFileException ThrowInfo 0x01369380.
// Layout: CMemoryFile Position +0x08, Length +0x0C in retail (engine/CMemoryFile.h is 4 bytes off in retail -- chunk 147 -- so the class is
// modelled locally with its PDB member names).
// Shape: the exception model of the landed CDiskFileWin32::Write 0x0099AA90 (CBBBFileException(CStringFileArgument by value, type) over an
// empty CExceptionBase(const char*)); the message is `CCharString("", -1) + "..."` with the prefix temporary in the dead `delta` slot; no EH
// frame (built without /EHsc), so the message is never destroyed.
class CCharString { public:
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
    enum EFileExceptionType { SeekOutOfRange = 11 };
    EFileExceptionType Type;
    CBBBFileException(CCharString text, EFileExceptionType type) : CExceptionBase(static_cast<const char*>(text)), Type(type) { }
};
class CMemoryFile { public:
    void*         __vftable;                    // +0x00
    unsigned char* Data;                        // +0x04
    long          Position;                     // +0x08
    long          Length;                       // +0x0c
    void MovePosition(long delta);
};

void CMemoryFile::MovePosition(long delta)
{
    long position = Position + delta;
    if (position < 0) {
        CCharString message = CCharString("", -1) + "Cannot move before beginning of file";
        throw CBBBFileException(message, CBBBFileException::SeekOutOfRange);
    }
    if (position > Length) {
        CCharString message = CCharString("", -1) + "Cannot seek past end of file";
        throw CBBBFileException(message, CBBBFileException::SeekOutOfRange);
    }
    Position = position;
}