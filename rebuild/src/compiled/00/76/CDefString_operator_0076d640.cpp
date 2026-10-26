// CDefString::operator!=(const CCharString& text) const -- whether the table string is not equal to a CCharString (the strcmp intrinsic; a null string compares as "")
// Leaf: ??9CDefString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076D640, length 107 bytes.
// Callees: CDefStringTable::GetStringAsCharArray(long) @ 0x009D49F0 on the global table @ 0x013CA828.
// Shape: the landed const char* overload (0x005B3440) with the argument's characters read first through the
// null-case-first CCharString accessor ("" @ 0x0129AAF4; VC evaluates strcmp's right-hand argument first); the
// two-bytes-per-iteration inline strcmp, then `!= 0` as a bool through a full register (`xor ecx,ecx ; setne cl ; mov al,cl`).
// PDB CDefString: long TablePos at +0.
#include <string.h>

class CCharStringData { public:
    const char*   m_pChars;                         // +0x00
    unsigned long m_length;                         // +0x04
};
extern const char g_emptyLiteral_0129aaf4[];        // 0x0129AAF4: ""
class CCharString { public:
    CCharStringData* m_pStringData;
    const char* GetChars() const { return m_pStringData == 0 ? g_emptyLiteral_0129aaf4 : m_pStringData->m_pChars; }
};

struct CDefStringTable {
    const char* GetStringAsCharArray(long id);      // 0x009D49F0
};

extern CDefStringTable g_DefStringTable;            // 0x013CA828

class CDefString { public:
    long TablePos;                                  // +0
    bool operator!=(const CCharString& text) const;
};

bool CDefString::operator!=(const CCharString& text) const
{
    return strcmp(g_DefStringTable.GetStringAsCharArray(TablePos), text.GetChars()) != 0;
}