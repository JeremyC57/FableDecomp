// CMemoryFile::SetPosition(long) -- seek; a position past the end builds an error message and throws error code 11
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E90C0, 128 bytes (padding included).
// Grade: DIFFER(112v118) -- frame: retail reserves two words (sub esp,8: message at [esp], the thrown 11 at [esp+4]) and loads Length into edx before the compare; ours uses one word (push ecx), throws from the dead argument slot and compares against memory. Tried: throw 11 / named long / CFileError struct / error code declared first / inline thrower
// Shape: `position > Length` (+0xc, signed) takes the error path: an explicit CCharString temporary
// (literal 0x0122D70E, 0x0099EBF0; its returned pointer feeds edx) is concatenated with the literal
// @ 0x0129A898 by the __fastcall operator+ (0x0099F600, hidden result in ecx), the temporary dies
// (0x0099EAE0), the message is handed BY VALUE to an inline reporter (copy 0x0099EC30 built in the
// dead `position` slot, its text read through operator const char* 0x0099E4C0, then destroyed) and
// `throw 11` (_CxxThrowException 0x00BFEB84 with the frame word and the int ThrowInfo 0x01369380). No
// /EHsc in this module: the message local is never destroyed on the throw path. Otherwise Position
// (+8) = position. Members per the PDB CMemoryFile (retail packs DataBuffer in 12 bytes, so the
// engine header's +0x20/+0x24 sit 4 bytes low in retail).
struct CCharStringData;

struct CCharString {
    CCharStringData* m_pData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    operator const char*() const;                   // 0x0099E4C0
};

CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600

inline void DebugOutput(const char*) { }            // compiled out in the retail build
inline void ReportError(CCharString message) { DebugOutput(message); }

struct CMemoryFile {
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
        ReportError(message);
        throw 11;
    }
    Position = position;
}
