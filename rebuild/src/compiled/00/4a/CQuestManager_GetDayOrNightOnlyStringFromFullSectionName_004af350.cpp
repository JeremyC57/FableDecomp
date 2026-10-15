// CQuestManager::GetDayOrNightOnlyStringFromFullSectionName(const CCharString& fullName) -- the part of a "<section>%<name>" quest section name after the '%', or "" when there is none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AF350, length 98 bytes (padded to 112), `ret 8` (hidden result + name).
// Callees: CCharString::FindCharPos(char) @ 0x0099E5A0, CCharString::GetSubString(unsigned long, unsigned long) const
// @ 0x0099EC70 (by-value result through the hidden pointer), CCharString::CCharString(const char*, long) @ 0x0099EBF0
// ("" @ 0x0122D70E).
// Layout: CCharString is a handle on CCharStringData (CBasicString<char> {PData +0, StringLength +4, ...}).
// Shape: `pos > -1` gives `cmp eax,-1 ; jle`; the substring length is the inline GetLength() (`PStringData == 0 ? 0 :
// PStringData->StringLength`; the null arm reuses the tested zero register), whose two arms VC7.1 tail-duplicates through the GetSubString call and return;
// the empty result is a direct return of a converted literal (built in the hidden result).
struct CCharStringData {
    char*         PData;                           // +0x00
    unsigned long StringLength;                    // +0x04
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    long FindCharPos(char c) const;                    // 0x0099E5A0
    CCharString GetSubString(unsigned long start, unsigned long count) const;   // 0x0099EC70
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
};
struct CQuestManager {
    CCharString GetDayOrNightOnlyStringFromFullSectionName(const CCharString& fullName);
};
CCharString CQuestManager::GetDayOrNightOnlyStringFromFullSectionName(const CCharString& fullName)
{
    long pos = fullName.FindCharPos('%');
    if (pos > -1)
        return fullName.GetSubString(pos + 1, fullName.GetLength());
    return CCharString("");
}