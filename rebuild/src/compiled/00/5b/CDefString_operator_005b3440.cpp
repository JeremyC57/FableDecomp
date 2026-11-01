// CDefString::operator==(const char*) const -- compare the table string with a C string (the strcmp intrinsic)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B3440, length 90 bytes.
// Callees: CDefStringTable::GetStringAsCharArray(long) @ 0x009D49F0 on the global table @ 0x013CA828.
// Shape: the two-bytes-per-iteration inline `strcmp` expansion (dl/bl, `sbb eax,eax ; sbb eax,-1` for the sign),
// then `== 0` as a bool through a full register (`xor ecx,ecx ; sete cl ; mov al,cl`). PDB CDefString: long TablePos at +0.
#include <string.h>

class CDefStringTable { public:
    const char* GetStringAsCharArray(long id);     // 0x009D49F0
};

extern CDefStringTable GDefStringTable;   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)           // 0x013CA828

class CDefString { public:
    long TablePos;                                 // +0
    bool operator==(const char* text) const;
};

bool CDefString::operator==(const char* text) const
{
    return strcmp(GDefStringTable.GetStringAsCharArray(TablePos), text) == 0;
}