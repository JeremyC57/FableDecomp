// GFGetBuildNumber2() -- load the full build version string from full_build_version.txt, if the file exists
// Leaf: ?GFGetBuildNumber2@@YAXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00401F30, 162 bytes.
// Callees: CWideString::CWideString(const wchar_t*) @ 0x0099B6B0, CWideString::PathExists() const @ 0x00999230,
//   CWideString::~CWideString @ 0x0099B510, CDiskFileWin32::CDiskFileWin32(const CWideString&, FileAccessType, unsigned long)
//   @ 0x0099AD80, CCharString::CCharString(CAFile*, long) @ 0x0099FA50, CCharString::operator= @ 0x0099EFB0,
//   CCharString::~CCharString @ 0x0099EAE0, CDiskFileWin32::Close @ 0x0099A920 (virtual in the PDB, called directly from
//   the inlined destructor), CBaseClassNonCopyable::~CBaseClassNonCopyable @ 0x0099A300 (the CAFile base chain).
//
// Called once from GFInitialise. The build machine drops a `full_build_version.txt` next to the
// executable; when it is present its whole contents become the global FableFullBuildVersion string that
// the title bar and bug reports show. A retail install usually has no such file, in which case nothing
// happens and the compiled-in build number stands.
//
// The path is a CWideString built twice (the existence check and the file open each take their
// own); the disk file is opened read-only (access 1, share mode 2), read in one go into a CCharString
// that is assigned to the global, and the file's inline destructor runs at the end of the block:
// vtable reset (0x0122D06C = ??_7CDiskFileWin32@@6B@), Close, the Pathname member (+0x18, a CWideString), then the
// base destructor. The 0x0099B510 / 0x0099EAE0 pair are the two string classes' destructors (wide: NoRefs at data+0xc
// and free(); char: release 0x0099E9B0 with NoRefs at data+0xd as the PDB CCharStringData has it).
// Naming wave 18: the CDiskFileWin32 model is polymorphic as the PDB has it (Ego_r ?Close@CDiskFileWin32@@UAEXXZ, the
//   protected virtual ??1CBaseClassNonCopyable@@MAE@XZ, the ctor's (const CWideString&, FileAccessType, unsigned long)
//   pair); the inlined ~CDiskFileWin32 calls Close directly (the destructor knows the dynamic type) and the vtable store is
//   the compiler's ??_7CDiskFileWin32@@6B@ -- name-only (the bytes are unchanged).
#include <stddef.h>                                  // wchar_t (unsigned short: no /Zc:wchar_t in VC7.1)
class CAFile;

class CWideString
{
public:
    CWideString(const wchar_t* text);               // 0x0099B6B0
    ~CWideString();                                 // 0x0099B510
    bool PathExists() const;                        // 0x00999230
    void* PStringData;
};

class CCharString
{
public:
    CCharString(CAFile* file, long length);         // 0x0099FA50
    ~CCharString();                                 // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    void* PStringData;
};

class CBaseClassNonCopyable
{
protected:
    virtual ~CBaseClassNonCopyable();               // 0x0099A300 ??1CBaseClassNonCopyable@@MAE@XZ
};

class CAFile : public CBaseClassNonCopyable
{
public:
    virtual void Close() = 0;
};

enum FileAccessType { };                            // the enumerators are not in the name sets: the retail values are cast

class CDiskFileWin32 : public CAFile                 // vtable 0x0122D06C; PDB layout 0x1c: AccessType +4 .. Pathname +0x18
{
public:
    CDiskFileWin32(const CWideString& path, FileAccessType accessType, unsigned long shareMode);   // 0x0099AD80
    ~CDiskFileWin32()
    {
        Close();
    }
    virtual void Close();                           // 0x0099A920 ?Close@CDiskFileWin32@@UAEXXZ
    long        AccessType;                         // +0x04
    long        Position;                           // +0x08
    long        Length;                             // +0x0c
    void*       WinFileHandle;                      // +0x10
    bool        Opened;                             // +0x14
    CWideString Pathname;                           // +0x18
};

extern CCharString FableFullBuildVersion;           // 0x013B85E0

// Matching: retail builds and destroys the three objects in fixed frame slots in exactly this order
// (the wide path is rebuilt in the same slot and destroyed straight after the file opens it), so each
// slot is raw storage brought to life with placement new and ended with an explicit destructor call.
inline void* operator new(size_t, void* where) { return where; }
template <class T, size_t N> struct CSlot
{
    unsigned char Raw[N];
    T& Get() { return *reinterpret_cast<T*>(Raw); }
};

void __cdecl GFGetBuildNumber2()
{
    CSlot<CWideString, 4> pathText;
    CSlot<CCharString, 4> fileContents;
    CSlot<CDiskFileWin32, 0x1c> diskFile;

    // Does the version file exist at all?
    new (pathText.Raw) CWideString(L"full_build_version.txt");
    const bool pathExists = pathText.Get().PathExists();
    pathText.Get().~CWideString();

    if (pathExists)
    {
        // Open it read-only and slurp the whole file into the global version string.
        new (pathText.Raw) CWideString(L"full_build_version.txt");
        new (diskFile.Raw) CDiskFileWin32(pathText.Get(), (FileAccessType)1, 2);
        pathText.Get().~CWideString();

        new (fileContents.Raw) CCharString(&diskFile.Get(), -1);
        FableFullBuildVersion = fileContents.Get();
        fileContents.Get().~CCharString();

        diskFile.Get().CDiskFileWin32::~CDiskFileWin32();     // qualified: the explicit call of the virtual destructor stays direct, as the scope-end call retail compiled
    }
}
