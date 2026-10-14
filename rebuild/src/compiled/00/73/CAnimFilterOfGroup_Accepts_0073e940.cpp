// CAnimFilterOfGroup::Accepts(const CAnimInfo&) -- virtual slot 0: the anim is accepted when the CRC of its
// group-name string (global def string table) equals the stored group CRC.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073E940, length 33 bytes.
// Callees: CDefStringTable::GetStringCRC 0x009D4A20 (on the global table @ 0x013CA828).
// Layout (PDB CAnimFilterOfGroup, 0xc bytes): vptr +0, Group +4, GroupCRC +8.
// The equality comes back as a long through `sub ; neg ; sbb ; inc` (an int result, not a bool `sete al`).
struct CDefStringTable {
    unsigned long GetStringCRC(long stringIndex);      // 0x009D4A20
};
extern CDefStringTable g_defStringTable;                // 0x013CA828

struct CCharString { void* PStringData; };

struct CAnimInfo {
    unsigned char _pad[8];
    long GroupName;                                      // +8: def string index of the anim's group
};

struct CAnimFilterOfGroup {
    virtual long Accepts(const CAnimInfo& anim);
    CCharString Group;                                   // +4
    unsigned long GroupCRC;                              // +8
};

long CAnimFilterOfGroup::Accepts(const CAnimInfo& anim)
{
    return g_defStringTable.GetStringCRC(anim.GroupName) == GroupCRC;
}