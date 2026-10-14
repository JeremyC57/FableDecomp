// CAnimFilterOf2Groups::Accepts(const CAnimInfo&) -- virtual slot 0: the anim is accepted when the CRC of its
// group-name string (looked up in the global def string table) equals either of the two stored group CRCs.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073EA00, length 63 bytes.
// Callees: CDefStringTable::GetStringCRC 0x009D4A20 (on the global table @ 0x013CA828).
// Layout (PDB CAnimFilterOf2Groups, 0x14 bytes): vptr +0, Group1 +4, Group2 +8, Group1CRC +0xc, Group2CRC +0x10.
// The result is a long (`mov eax,1` / `xor eax,eax`), not a bool (that would be `mov al,1`); the anim's
// string index at +8 is re-read before the second lookup because the call may have changed it.
struct CDefStringTable {
    unsigned long GetStringCRC(long stringIndex);      // 0x009D4A20
};
extern CDefStringTable g_defStringTable;                // 0x013CA828

struct CCharString { void* PStringData; };

struct CAnimInfo {
    unsigned char _pad[8];
    long GroupName;                                      // +8: def string index of the anim's group
};

struct CAnimFilterOf2Groups {
    virtual long Accepts(const CAnimInfo& anim);
    CCharString Group1;                                  // +4
    CCharString Group2;                                  // +8
    unsigned long Group1CRC;                             // +0xc
    unsigned long Group2CRC;                             // +0x10
};

long CAnimFilterOf2Groups::Accepts(const CAnimInfo& anim)
{
    if (g_defStringTable.GetStringCRC(anim.GroupName) == Group1CRC ||
        g_defStringTable.GetStringCRC(anim.GroupName) == Group2CRC)
        return 1;
    return 0;
}