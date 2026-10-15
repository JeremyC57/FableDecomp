// CDefString::operator==(const CCharString& text) const -- whether the table string is equal to a CCharString (the strcmp intrinsic; a null string compares as "")
// Leaf: ??8CDefString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B7980, length 107 bytes.
// Callees: CDefStringTable::GetStringAsCharArray(long) @ 0x009D49F0 on the global table @ 0x013CA828.
// Shape: the landed const char* overload (0x005B3440) with the argument's characters read first through the
// null-case-first CCharString accessor ("" @ 0x0129AAF4; VC evaluates strcmp's right-hand argument first); the
// two-bytes-per-iteration inline strcmp, then `== 0` as a bool through a full register (`xor ecx,ecx ; sete cl ; mov al,cl`).
// PDB CDefString: long TablePos at +0.
#include <string.h>

struct CCharStringData {
    const char*   m_pChars;                         // +0x00
    unsigned long m_length;                         // +0x04
};
extern const char g_emptyLiteral_0129aaf4[];        // 0x0129AAF4: ""
struct CCharString {
    CCharStringData* m_pStringData;
    const char* GetChars() const { return m_pStringData == 0 ? g_emptyLiteral_0129aaf4 : m_pStringData->m_pChars; }
};

struct CDefStringTable {
    const char* GetStringAsCharArray(long id);      // 0x009D49F0
};

extern CDefStringTable g_DefStringTable;            // 0x013CA828

struct CDefString {
    long TablePos;                                  // +0
    bool operator==(const CCharString& text) const;
};

bool CDefString::operator==(const CCharString& text) const
{
    return strcmp(g_DefStringTable.GetStringAsCharArray(TablePos), text.GetChars()) == 0;
}