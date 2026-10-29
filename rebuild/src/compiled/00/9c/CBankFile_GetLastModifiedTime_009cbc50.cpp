// CBankFile::GetLastModifiedTime(const CCharString& path) -- open the file read-only on a local CDiskFileWin32, read its file information and return the last-write time
// Leaf: ?GetLastModifiedTime@CBankFile@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CBC50, length 188 bytes (+4 int3), ret 8 (hidden CDateAndTime result, path;
// `this` unused). Ego_r.pdb ?GetLastModifiedTime@CBankFile@@MAE?AVCDateAndTime@@ABVCCharString@@@Z.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CWideString::CWideString() @ 0x0099AED0, CCharString::ToWideString() @ 0x0099E480,
// CDiskFileWin32::Open(const CWideString&, FileAccessType, ulong) @ 0x0099A6A0 (read, 2), CWideString::~CWideString
// @ 0x0099B510, CDiskFileWin32::GetFileInformation() @ 0x0099A5A0 (by-value CFileInformation, 0x58 bytes),
// CDiskFileWin32::Close() @ 0x0099A920, CBaseClassNonCopyable::~CBaseClassNonCopyable @ 0x0099A300. Vtable 0x0122D06C = CDiskFileWin32.
// Shape: the local file's inline ctor (vtable, Pathname, then Opened = false in the BODY: an initialiser stores it before Pathname) and inline dtor (vtable, Close, ~Pathname, ~CBase);
// the virtual Open/GetFileInformation/Close are direct calls on the local. LastWriteTime (+0x38) is copied into a named
// CDateAndTime local (`rep movsd` x7), then into the result.
// Layout: PDB CDiskFileWin32 (0x1c: Opened +0x14, Pathname +0x18), CFileInformation (Creation +0, LastAccess +0x1c,
// LastWrite +0x38, ReadOnly +0x54), CDateAndTime (7 dwords).
class CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable();                                       // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                                      // 0x0099A300
};
class CWideString { public:
    void* PStringData;
    CWideString();                                 // 0x0099AED0
    ~CWideString();                                // 0x0099B510
};
class CCharString { public:
    void* m_pData;
    CWideString ToWideString() const;              // 0x0099E480
};
class CDateAndTime { public:
    long Year;
    unsigned long Month, Day, Hour, Minute, Second, Millisecond;
};
class CFileInformation { public:
    CDateAndTime CreationTime;                     // +0x00
    CDateAndTime LastAccessTime;                   // +0x1c
    CDateAndTime LastWriteTime;                    // +0x38
    bool ReadOnly;                                 // +0x54
};
enum FileAccessType { FILE_ACCESS_READ = 1 };

class CDiskFileWin32 : public CBaseClassNonCopyable { public:                    // vtable 0x0122D06C
    long        AccessType;                        // +0x04
    long        Position;                          // +0x08
    long        Length;                            // +0x0c
    void*       WinFileHandle;                     // +0x10
    bool        Opened;                            // +0x14
    CWideString Pathname;                          // +0x18
    CDiskFileWin32() { Opened = false; }
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099A6A0
    virtual CFileInformation GetFileInformation(); // 0x0099A5A0
    virtual void Close();                          // 0x0099A920
};

class CBankFile {
public:
    CDateAndTime GetLastModifiedTime(const CCharString& path);
};

CDateAndTime CBankFile::GetLastModifiedTime(const CCharString& path)
{
    CDiskFileWin32 file;
    file.Open(path.ToWideString(), FILE_ACCESS_READ, 2);
    CDateAndTime time = file.GetFileInformation().LastWriteTime;
    file.Close();
    return time;
}