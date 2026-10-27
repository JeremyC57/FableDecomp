// NProfileTimer::$E_LogFile() -- dynamic initializer of the profile timer's log file: an inline CDiskFileWin32 default constructor (base, empty pathname, vtable, not opened), then its destructor is registered with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224100, 49 bytes, in `.text$yc`, reached from the .CRT$XCU table, plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb `NProfileTimer::dynamic initializer for 'LogFile'` follows
//   `NProfileTimer::dynamic initializer for 'ProfilingInfo'`; retail's previous initializer 0x012240C0 builds the tree at
//   0x013CA86C and this one the object right after it at 0x013CA878 (0x86C + 0xC), whose vtable 0x0122D06C is CDiskFileWin32
//   (rtti-vtables.tsv). The registered thunk 0x0122A790 is the inline ~CDiskFileWin32 (vtable, Close 0x0099A920,
//   ~CWideString 0x0099B510, ~CBaseClassNonCopyable 0x0099A300), as the landed ~CDiskFileWin32 0x00401630.
// Label: the manifest's "sub_01224100" is a gap-scan placeholder.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable() 0x0099A2F0, CWideString::CWideString() 0x0099AED0, _atexit 0x004012BC.
// Layout: Ego_r.pdb CDiskFileWin32 (0x1c): AccessType +4, Position +8, Length +0xc, WinFileHandle +0x10, Opened +0x14,
//   CWideString Pathname +0x18; CAFile adds no vtable store of its own (novtable, as the landed 0x00401630).
// Shape: the landed CDiskFileWin32() 0x00415C30 inline: Opened is a body assignment after the pathname constructor.
class CWideString {
public:
    CWideString();                                    // 0x0099AED0
    ~CWideString();                                   // 0x0099B510
    void* PStringData;
};

class CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable();                          // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                 // 0x0099A300
};

enum FileAccessType { };

class __declspec(novtable) CAFile : public CBaseClassNonCopyable {
public:
    CAFile() { }
    virtual ~CAFile() { }
    virtual void Close() = 0;
};

class CDiskFileWin32 : public CAFile {               // vtable 0x0122D06C
public:
    CDiskFileWin32() { Opened = false; }
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Close();                             // 0x0099A920

    FileAccessType AccessType;                        // +0x04
    long           Position;                          // +0x08
    long           Length;                            // +0x0C
    void*          WinFileHandle;                     // +0x10
    bool           Opened;                            // +0x14
    CWideString    Pathname;                          // +0x18
};

namespace NProfileTimer {
CDiskFileWin32 LogFile;                               // 0x013CA878
}