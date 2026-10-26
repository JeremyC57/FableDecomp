// CDiskFileWin32::GetPathname() const -- copy of the path this file was opened with
// Retail: Fable.exe (TLC, VC7.1) @ 0x00401690, 22 bytes. Callees: CWideString copy constructor.
//
// Returns the stored `Pathname` by value. The file keeps the full path it was opened with so error
// messages and the installer's progress display can name the file; this hands the caller its own
// CWideString copy, built in the caller-supplied return slot.
//
// Matching: the return value is constructed in place (hidden return pointer in [esp+4]), so the body
// is one copy-constructor call with `&Pathname` (this+0x18) as the source; CWideString is modelled as a
// single pointer so the constructor takes one argument.
namespace N_0099b720 { struct CWideString {
    char* p;                        // string storage pointer
    CWideString(const char* s);     // copy-construct from existing storage
}; } using N_0099b720::CWideString;

class CDiskFileWin32 { public:
    char pad[0x18];                 // access type, position, length, handle, opened flag
    char name[4];                   // Pathname (CWideString) at +0x18
    CWideString GetPathname() const;
};

CWideString CDiskFileWin32::GetPathname() const {
    return CWideString(this->name);
}
