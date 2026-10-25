// CDiskFileWin32::~CDiskFileWin32() -- close the Win32 file, destroy the path name, then the CBaseClassNonCopyable root
// Leaf: ??1CDiskFileWin32@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00401630, 30 bytes, thiscall.
// Identity: the first instruction after the prologue stores vtable 0x0122D06C, which rtti-vtables.tsv gives to
//   CDiskFileWin32 (slots 9/12/13 = 0x401670/0x401650/0x401660), before any member teardown, so this is that class's own
//   destructor (an inlined base destructor would store its vtable after the derived body). It then calls
//   CDiskFileWin32::Close() 0x0099A920 (landed, ?Close@CDiskFileWin32@@UAEXXZ; a direct call, as virtual calls in a
//   destructor are), destroys the CWideString at +0x18 (Ego_r.pdb CDiskFileWin32::Pathname, ~CWideString 0x0099B510) and
//   tail-jumps to ~CBaseClassNonCopyable 0x0099A300. Ego_r.pdb name set: ??1CDiskFileWin32@@UAE@XZ, ??1CAFile@@UAE@XZ.
// Label: landed as CAIStateGroup_CreatureUseCoverBase::~CAIStateGroup_CreatureUseCoverBase (the landed comment already
//   doubted it); the catalog already says CDiskFileWin32::~CDiskFileWin32. CAFile adds no vtable store of its own, so it
//   is modelled `__declspec(novtable)` (as the landed def-base models do).
class CWideString {
public:
    ~CWideString();                                   // 0x0099B510
    void* PStringData;
};

class CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                 // 0x0099A300
};

enum FileAccessType { };

class __declspec(novtable) CAFile : public CBaseClassNonCopyable {
public:
    virtual ~CAFile() { }
    virtual void Close() = 0;
};

class CDiskFileWin32 : public CAFile {
public:
    virtual ~CDiskFileWin32();
    virtual void Close();                             // 0x0099A920

    FileAccessType AccessType;                        // +0x04
    long           Position;                          // +0x08
    long           Length;                            // +0x0C
    void*          WinFileHandle;                     // +0x10
    bool           Opened;                            // +0x14
    CWideString    Pathname;                          // +0x18
};

CDiskFileWin32::~CDiskFileWin32()
{
    Close();
}
