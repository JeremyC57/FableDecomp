// CFileInformation::CFileInformation() -- default-constructs the three CDateAndTime members (creation, last access, last write)
// Leaf: ??0CFileInformation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099A540, 28 bytes (+4 int3 padding).
// Callee: 0x009E9AB0 CDateAndTime::CDateAndTime() (zeroes its seven fields).
//
// The manifest label (a CPersistContextReadSection constructor) is propagated and wrong: the body
// constructs three 0x1C-byte members at +0x00, +0x1C, +0x38, which is the PDB layout of
// CFileInformation (CreationTime, LastAccessTime, LastWriteTime, then bool ReadOnly at +0x54; 88
// bytes). It sits right before CDiskFileWin32::GetFileInformation (0x0099A5A0), which returns a
// CFileInformation by value. ReadOnly has no initialiser (no store), and `this` stays in esi for the
// return.
struct CDateAndTime {
    long          Year;
    unsigned long Month;
    unsigned long Day;
    unsigned long Hour;
    unsigned long Minute;
    unsigned long Second;
    unsigned long Millisecond;
    CDateAndTime();                        // 0x009E9AB0
};

struct CFileInformation {
    CDateAndTime CreationTime;             // +0x00
    CDateAndTime LastAccessTime;           // +0x1C
    CDateAndTime LastWriteTime;            // +0x38
    bool         ReadOnly;                 // +0x54
    CFileInformation();
};

CFileInformation::CFileInformation()
{
}
