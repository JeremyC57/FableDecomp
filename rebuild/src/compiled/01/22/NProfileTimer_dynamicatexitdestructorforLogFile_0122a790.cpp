// NProfileTimer::`dynamic atexit destructor for 'LogFile''() -- at exit run the inline ~CDiskFileWin32 on the profile timer's log file: its vtable, Close, the Pathname string, then ~CBaseClassNonCopyable
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0122A790, 40 bytes, in `.text$yd`, plain tail `jmp ~CBaseClassNonCopyable`.
// Identity (proven): the only reference is the `push offset 0x0122A790 ; call _atexit` of the landed dynamic initializer
//   0x01224100 (`NProfileTimer::dynamic initializer for 'LogFile''); Ego_r.pdb / FableWin.pdb list
//   "NProfileTimer::`dynamic atexit destructor for 'LogFile''" (Ego_r 0x0101E460, FableWin 0x04012710) for the same object.
//   The object is 0x013CA878 (?LogFile@NProfileTimer@@3VCDiskFileWin32@@A, already bound by the initializer) and the stored
//   vtable 0x0122D06C is RTTI .?AVCDiskFileWin32@@ : CAFile : CBaseClassNonCopyable : CBaseClass. VC7.1 names the pair
//   `$E1` / `$E2` in the TU: this source is the initializer row's own TU (the one definition below emits both).
//   The catalog label `_Dest_val<std::allocator<std::pair<CCharString,CSymbolMap::C::CSymbolInfo>>>` is propagated.
// Callees: CDiskFileWin32::Close 0x0099A920 (?Close@CDiskFileWin32@@UAEXXZ, slot 2, called directly in the dtor),
//   CWideString::~CWideString 0x0099B510 on Pathname (0x013CA890 = +0x18), CBaseClassNonCopyable::~CBaseClassNonCopyable
//   0x0099A300 (MAE, tail).
// Layout: Ego_r.pdb CDiskFileWin32 (0x1c): AccessType +4, Position +8, Length +0xc, WinFileHandle +0x10, Opened +0x14,
//   CWideString Pathname +0x18; CAFile is novtable (no store of its own), as the landed ~CDiskFileWin32 0x00401630.
//   engine/CDiskFileWin32.h flattens the vptr (no CAFile base), so the polymorphic class is local, sized against the header
//   included under a layout name (the chunk 708 layout-rename pattern; header debt in NOTES.md).
// Shape: the landed 0x01224100 model (inline virtual ~CDiskFileWin32() { Close(); }) with the shared engine/CBaseClassNonCopyable.h
//   (: CBaseClass) in place of its local base stand-in; the $E1 initializer graded from this same text stays RELOCATION_MATCH.
#define CDiskFileWin32 CDiskFileWin32Fields
#include "engine/CDiskFileWin32.h"                    // Ego_r offsets / sizeof 0x1c (flattened vptr: layout name only)
#undef CDiskFileWin32
class CWideString {
public:
    CWideString();                                    // 0x0099AED0
    ~CWideString();                                   // 0x0099B510
    void* PStringData;
};

#include "engine/CBaseClassNonCopyable.h"           // : CBaseClass; CBaseClassNonCopyable() 0x0099A2F0, ~ 0x0099A300 (protected virtual)

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

FABLE_STATIC_ASSERT(sizeof(CDiskFileWin32) == sizeof(CDiskFileWin32Fields));

namespace NProfileTimer {
CDiskFileWin32 LogFile;                               // 0x013CA878
}