// CAFile::GetFileInformation() -- virtual: returns a default CFileInformation (three cleared CDateAndTime stamps)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00412090, length 34 bytes.
// Callees: CDateAndTime::CDateAndTime @ 0x009E9AB0 (x3: zeroes the 0x1c-byte record).
// Mangled name from the manifest: ?GetFileInformation@CAFile@@UAE?AVCFileInformation@@XZ --
// a thiscall virtual returning CFileInformation by value. `this` (ecx) is never read; the hidden
// result pointer ([esp+8]) is default-constructed in place -- three CDateAndTime members at +0,
// +0x1c and +0x38 (created / modified / accessed) -- and returned in eax (`ret 4`).
struct CDateAndTime {
    unsigned char _body[0x1c];
    CDateAndTime();                         // 0x009E9AB0
};

struct CFileInformation {
    CDateAndTime m_created;                 // +0x00
    CDateAndTime m_modified;                // +0x1c
    CDateAndTime m_accessed;                // +0x38
    CFileInformation() : m_created(), m_modified(), m_accessed() { }
};

struct CAFile {
    virtual void v00();
    virtual CFileInformation GetFileInformation();
};

CFileInformation CAFile::GetFileInformation()
{
    return CFileInformation();
}
