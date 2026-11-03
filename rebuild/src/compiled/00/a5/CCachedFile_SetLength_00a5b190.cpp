// CCachedFile::SetLength(long length) -- a cached file cannot be resized: always throws a CBBBFileException (type 8) carrying "File is not open for writing"
// Retail-span: int3-after-noreturn row -- the 0xCC after the final call _CxxThrowException belongs to the function (GOTCHAS 2026-10-22); graded against the 97-byte retail span
// Leaf: ?SetLength@CCachedFile@@UAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5B190, length 96 bytes (vtable 0x012724D4 slot 8,
// ?SetLength@CCachedFile@@UAEXJ@Z; Ego_r.pdb donor order Close / Write / SetPosition / MovePosition /
// SetLength / GetLength = retail 0x00A5AFA0 / 0x00A5AFB0 / 0x00A5B020 / 0x00A5B0A0 / 0x00A5B190 / 0x00A5B200).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, operator+(const CCharString&, const char*) @ 0x0099F600
// (__fastcall, result by hidden pointer), CCharString::~CCharString @ 0x0099EAE0, CCharString::CCharString(const CCharString&)
// @ 0x0099EC30, CCharString::operator const char* @ 0x0099E4C0, _CxxThrowException @ 0x00BFEB84 with the CBBBFileException
// throw info @ 0x01369380. Literals: "" @ 0x0122D70E, "File is not open for writing" @ 0x0129A878.
// Shape: the PDB constructor ??0CBBBFileException@@QAE@VCCharString@@W4EFileExceptionType@0@@Z takes the message BY VALUE:
// the copy is built in the empty-string temporary's dead slot, converted to `const char*` (the reporting sink compiled out)
// and destroyed by the inlined constructor; the exception object (Type 8, as CDiskFileWin32::Write 0x0099AA90 uses for
// the same text) takes the third frame word. Same body as CCachedFile::Write 0x00A5AFB0 (slot 4). No /EHsc: the message local is never destroyed on the throwing path.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length);   // 0x0099EBF0
    CCharString(const CCharString& other);    // 0x0099EC30
    ~CCharString();                           // 0x0099EAE0
    operator const char*() const;             // 0x0099E4C0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600

class CBBBFileException {
public:
    enum EFileExceptionType {
        NotOpenForWrite = 8                   // descriptive enumerator (the PDB keeps only the type)
    };
    EFileExceptionType Type;                  // +0x00
    CBBBFileException(CCharString message, EFileExceptionType type) : Type(type) { (const char*)message; }
};

class CASuspendableProcess;

class CAFile {
public:
    virtual ~CAFile();
    virtual void Open();
    virtual void Close();
    virtual void Read(void* data, long size, CASuspendableProcess* process);
    virtual void Write(const void* data, long size, CASuspendableProcess* process);
    virtual void SetPosition(long position);
    virtual void MovePosition(long offset);
    virtual long GetPosition() const;
    virtual void SetLength(long length);
};

class CCachedFile : public CAFile {
public:
    virtual void SetLength(long length);
};

void CCachedFile::SetLength(long length)
{
    CCharString message = CCharString("", -1) + "File is not open for writing";
    throw CBBBFileException(message, CBBBFileException::NotOpenForWrite);
}