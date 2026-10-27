// CVtable_012b4f4c::GetSize(unsigned long* size) -- ask the file manager (+4) for the size of this handle's file (key +8)
// Leaf: ?GetSize@CVtable_012b4f4c@@UAE_NPAK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C32F40, 20 bytes, thiscall, `ret 4`, bool result.
// Identity (descriptive): slot 0 of the non-RTTI vtable 0x012B4F4C, stored only by its constructor 0x00C32FC0 (the
//   manager at +4, the file key at +8): the libpk file handle over the default file manager. Its slot 1 is
//   0x00C32F60 (read), slot 3 0x00C32FA0 (close through 0x00C1A870 / 0x00C19FA0). libpk has no PDB or RTTI.
// Callees: CFileManager_00c1a070::GetFileSize(unsigned long key, unsigned long* size) @ 0x00C1A070 (landed; the
//   default file manager's locked Win32 GetFileSize; its header names 0x00C32F4C as its caller).
struct CFileManager_00c1a070 {
    bool GetFileSize(unsigned long key, unsigned long* size);   // 0x00C1A070
};

class CVtable_012b4f4c {                        // vtable 0x012B4F4C
public:
    virtual bool GetSize(unsigned long* size);  // slot 0
    CFileManager_00c1a070* m_pManager;          // +0x04
    unsigned long          m_key;               // +0x08
};

bool CVtable_012b4f4c::GetSize(unsigned long* size)
{
    return m_pManager->GetFileSize(m_key, size);
}