// CCRC::CalcFromPathname(const CCharString& pathname) -- CRC-32 of a pathname's lower-cased file name and extension (the directory and drive ignored)
// Leaf: ?CalcFromPathname@CCRC@@SIKABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009914E0, length 186 bytes, plain `ret` (static __fastcall, pathname in ecx).
// PDB (Ego_r donor 0x00AEAA70, FableWin): ?CalcFromPathname@CCRC@@SIKABVCCharString@@@Z (beside CCRC::CalcFromString,
// retail 0x009914B0, the GetCRC twin). No references in the image (an unreferenced out-of-line copy).
// Callees: CCharString::CCharString() @ 0x0099E4B0 (x3), CAFile::SplitPath(const CCharString&, CCharString* drive,
// CCharString* dir, CCharString* name, CCharString* ext) @ 0x009976D0 (__fastcall), operator+(const CCharString&,
// const CCharString&) @ 0x0099F570 (__fastcall, hidden result in ecx), CCharString::operator= @ 0x0099EFB0,
// CCharString::ToLower() const @ 0x0099ED10, CCRC::Calc(seed, data, length) @ 0x004014A0 (__fastcall), ~CCharString
// @ 0x0099EAE0; the shared "" @ 0x0129AAF4.
// Label: the manifest's "CParticleMorphs_HashFilename" is descriptive; the PDB places it on CCRC.
// Shape: three named strings (file name, name, extension -- constructed in that order, destroyed in reverse after the
// lower-cased copy); `file = name + ext` assigns from the sum temporary (destroyed at once, its frame slot reused for the
// ToLower temporary); `return CalcFromString(file.ToLower())` with CalcFromString expanded (GetLength, null arm first,
// evaluated before GetPString, as in CCharString::GetCRC). A named `lower` copy (or a reference-bound one) takes its own
// frame slot (`sub esp,14h`, DIFFER 182v186).
class CCharStringData {
public:
    char*         PData;                        // +0x00 CBasicString<char>::PData
    unsigned long StringLength;                 // +0x04
};

extern const char g_EmptyString[];              // 0x0129AAF4

class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                              // 0x0099E4B0
    ~CCharString();                             // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharString ToLower() const;                // 0x0099ED10
    const char* GetPString() const { return PStringData == 0 ? g_EmptyString : PStringData->PData; }
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
};

CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570

class CAFile {
public:
    static void __fastcall SplitPath(const CCharString& path, CCharString* drive, CCharString* dir,
                                     CCharString* name, CCharString* ext);   // 0x009976D0
};

class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
    static unsigned long __fastcall CalcFromString(const CCharString& string);       // 0x009914B0 (inlined here)
    static unsigned long __fastcall CalcFromPathname(const CCharString& pathname);
};

// Same body as retail's out-of-line CalcFromString @ 0x009914B0; expanded into CalcFromPathname below.
inline unsigned long __fastcall CCRC::CalcFromString(const CCharString& string)
{
    return Calc(0, string.GetPString(), string.GetLength());
}

unsigned long __fastcall CCRC::CalcFromPathname(const CCharString& pathname)
{
    CCharString file;
    CCharString name;
    CCharString ext;
    CAFile::SplitPath(pathname, 0, 0, &name, &ext);
    file = name + ext;
    return CalcFromString(file.ToLower());
}