// CSingleItemTable_012b504c::GetColumnCount(unsigned long row) -- slot 4 of the one-entry item table: 1 when the entry is set, else 0
// Leaf: ?GetColumnCount@CSingleItemTable_012b504c@@UAEKK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C35120, 13 bytes, thiscall `ret 4` (the row is not read).
// Identity: slot 4 (GetColumnCount(row), CItemTable slot layout of the landed Validate 0x00C348F0) of vtable 0x012B504C.
//   Descriptive (libpk, no PDB/RTTI).
// Shape: `m_pEntry != 0` widened through `xor eax,eax ; test ; setne al`.
struct CTableEntry;

class CSingleItemTable_012b504c {           // vtable 0x012B504C
public:
    virtual unsigned long GetType();                                         // slot 0
    virtual long v01();                                                      // slot 1
    virtual long v02();                                                      // slot 2
    virtual long Insert(unsigned long row, CTableEntry* entry);              // slot 3
    virtual unsigned long GetColumnCount(unsigned long row);                 // slot 4
    CTableEntry* m_pEntry;                  // +4
};

unsigned long CSingleItemTable_012b504c::GetColumnCount(unsigned long row)
{
    return m_pEntry != 0;
}