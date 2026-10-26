// CMemoryFile::SetPosition(long) -- seek; a position past the end builds an error message and throws a CBBBFileException (type 11)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E90C0, 128 bytes (padding included).
// Shape: `position > Length` (+0xc, signed) takes the error path: an explicit CCharString temporary
// (literal 0x0122D70E, 0x0099EBF0; its returned pointer feeds edx) is concatenated with the literal
// @ 0x0129A898 by the __fastcall operator+ (0x0099F600, hidden result in ecx), the temporary dies
// (0x0099EAE0), the message is handed BY VALUE to an inline reporter (copy 0x0099EC30 built in the
// dead `position` slot, its text read through operator const char* 0x0099E4C0, then destroyed) and
// `throw CBBBFileException(11, message)` (_CxxThrowException 0x00BFEB84 with the frame word and ThrowInfo 0x01369380,
// the CBBBFileException throw info -- the same one 0x00A5B020 CCachedFile::SetPosition uses, its type descriptor
// ".?AVCBBBFileException@@"; an earlier note called it the int ThrowInfo). The exception's constructor takes the
// message and reports it, which keeps the throw temporary live across the copy: its own frame word (`sub esp,8`). No
// /EHsc in this module: the message local is never destroyed on the throw path. Otherwise Position
// (+8) = position. Members per the PDB CMemoryFile (retail packs DataBuffer in 12 bytes, so the
// engine header's +0x20/+0x24 sit 4 bytes low in retail).
class CCharStringData;

class CCharString { public:
    CCharStringData* m_pData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    operator const char*() const;                   // 0x0099E4C0
};

CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600

inline void DebugOutput(const char*) { }            // compiled out in the retail build
inline void ReportError(CCharString message) { DebugOutput(message); }

enum EFileExceptionType {
    FILE_EXCEPTION_SEEK_PAST_END = 11
};
class CBBBFileException {
public:
    EFileExceptionType Type;                        // +0x00
    CBBBFileException(EFileExceptionType type, const CCharString& message) : Type(type) { ReportError(message); }
};

class CMemoryFile { public:
    virtual void v00();
    long          AccessType;                       // +0x04
    long          Position;                         // +0x08
    long          Length;                           // +0x0c
    void SetPosition(long position);
};

void CMemoryFile::SetPosition(long position)
{
    if (position > Length) {
        CCharString message = CCharString("", -1) + "CMemoryFile::SetPosition - past end of file";   // literals 0x0122D70E / 0x0129A898 (masked)
        throw CBBBFileException(FILE_EXCEPTION_SEEK_PAST_END, message);
    }
    Position = position;
}