// CCachedFile::SetPosition(long position) -- move the cached file's read position; seeking past the end of the underlying disk file builds the error text and throws a CBBBFileException
// Leaf: ?SetPosition@CCachedFile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5B020, length 128 bytes (ret 4; the virtual ?SetPosition@CCachedFile@@UAEXJ@Z).
// Callees: the disk file's virtual slot 9 (+0x24, its length), CCharString::CCharString(const char*, long)
// @ 0x0099EBF0, operator+(const CCharString&, const char*) @ 0x0099F600 (__fastcall, the result by hidden pointer),
// CCharString::~CCharString @ 0x0099EAE0, CCharString::CCharString(const CCharString&) @ 0x0099EC30,
// CCharString::operator const char* @ 0x0099E4C0, _CxxThrowException @ 0x00BFEB84 with the CBBBFileException throw
// info @ 0x01369380 (its type descriptor reads ".?AVCBBBFileException@@"). Literals: "" @ 0x0122D70E and
// "Cannot seek past end of file" @ 0x0129A898.
// Layout (Ego_r.pdb CCachedFile with the retail 12-byte CArray, as the landed CCachedFile::OpenFromFile 0x00A5B210):
// Position +0x04, PDiskFile +0x1c (the CCountedPointer<CAFile>: object first).
// Shape: `position > length` (signed, `jle` skips) guards the error block; the message is
// `CCharString("") + "Cannot seek past end of file"` in a frame local, the empty-string temporary and the by-value
// copy handed to the inline error reporter (which only converts it to `const char*`, its sink compiled out) are both
// homed in the dead `position` argument slot; the exception (Type 11) is built in the second frame word because its
// constructor takes the message and reports it (`CBBBFileException(type, const CCharString&) : Type(type) {
// ReportFileError(message); }`), so the throw temporary is live across the copy (reported in the caller, the lane homes
// it in the dead `position` slot too). No EH
// frame: the module is built without /EHsc (the message is never destroyed on the throwing path).
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length);   // 0x0099EBF0
    CCharString(const CCharString& other);    // 0x0099EC30
    ~CCharString();                           // 0x0099EAE0
    operator const char*() const;             // 0x0099E4C0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600

inline void ReportFileError(CCharString message)  // the release build keeps only the conversion
{
    (const char*)message;
}

enum EFileExceptionType {
    FILE_EXCEPTION_SEEK_PAST_END = 11
};
class CBBBFileException {
public:
    EFileExceptionType Type;                  // +0x00
    CBBBFileException(EFileExceptionType type) : Type(type) { }
    CBBBFileException(EFileExceptionType type, const CCharString& message) : Type(type) { ReportFileError(message); }
};

struct CAFile {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual long GetSize();                   // slot 9 (+0x24)
};
struct CCountedPointer_CAFile {
    CAFile* m_pObject;                        // +0x00
    void*   m_pCount;                         // +0x04
};

class CCachedFile {
public:
    long                   Position;          // +0x04
    long                   BufferStartPosition;   // +0x08
    unsigned char          DataBuffer[0x0c];  // +0x0c CArray<unsigned char>
    void*                  Pathname;          // +0x18 CWideString
    CCountedPointer_CAFile PDiskFile;         // +0x1c
    bool                   Opened;            // +0x24
    bool                   BufferValid;       // +0x25
    virtual void SetPosition(long position);
};

void CCachedFile::SetPosition(long position)
{
    if (position > PDiskFile.m_pObject->GetSize()) {
        CCharString message = CCharString("", -1) + "Cannot seek past end of file";
        throw CBBBFileException(FILE_EXCEPTION_SEEK_PAST_END, message);
    }
    Position = position;
}