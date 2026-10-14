// CDefString::operator!=(const char*) const -- compare the table string with a C string (the strcmp intrinsic)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FA740, length 90 bytes.
// Callees: CDefStringTable::GetStringAsCharArray(long) @ 0x009D49F0 on the global table @ 0x013CA828.
// Shape: the two-bytes-per-iteration inline `strcmp` expansion (dl/bl, `sbb eax,eax ; sbb eax,-1` for the sign),
// then `!= 0` as a bool through a full register (`xor ecx,ecx ; setne cl ; mov al,cl`). PDB CDefString: long TablePos at +0.
#include <string.h>

struct CDefStringTable {
    const char* GetStringAsCharArray(long id);     // 0x009D49F0
};

extern CDefStringTable g_DefStringTable;           // 0x013CA828

struct CDefString {
    long TablePos;                                 // +0
    bool operator!=(const char* text) const;
};

bool CDefString::operator!=(const char* text) const
{
    return strcmp(g_DefStringTable.GetStringAsCharArray(TablePos), text) != 0;
}
