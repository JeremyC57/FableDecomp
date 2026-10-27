// CSingleItemTable_012b504c::Insert(unsigned long row, CTableEntry* entry) -- slot 3 of the one-entry item table: only row 0 exists (-2 otherwise), an occupied slot refuses (-1), else the entry is stored (0)
// Leaf: ?Insert@CSingleItemTable_012b504c@@UAEJKPAUCTableEntry@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C350F0, 41 bytes, thiscall `ret 8`.
// Identity: slot 3 (Insert(row, entry), CItemTable slot layout of the landed Validate 0x00C348F0) of vtable 0x012B504C,
//   whose constructor 0x00C35170 nulls the single entry pointer at +4 (landed under the propagated label
//   CBaseIntelligentPointer::CBaseIntelligentPointer). Descriptive (libpk, no PDB/RTTI).
// Shape: `row > 0` on an unsigned row is `test ; jbe`; three exits with their own `ret 8`.
struct CTableEntry;

class CSingleItemTable_012b504c {           // vtable 0x012B504C
public:
    virtual unsigned long GetType();                                         // slot 0
    virtual long v01();                                                      // slot 1
    virtual long v02();                                                      // slot 2
    virtual long Insert(unsigned long row, CTableEntry* entry);              // slot 3
    CTableEntry* m_pEntry;                  // +4
};

long CSingleItemTable_012b504c::Insert(unsigned long row, CTableEntry* entry)
{
    if (row > 0)
        return -2;
    if (m_pEntry)
        return -1;
    m_pEntry = entry;
    return 0;
}