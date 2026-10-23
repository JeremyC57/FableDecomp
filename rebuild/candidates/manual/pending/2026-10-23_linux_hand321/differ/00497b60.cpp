// Grade: DIFFER(1060v1052) -- this-pointer spill slot order and register choice around the EndsWith chain; body structure matches
// CProgressDisplay::CalculateNextTextTag() -- advance the loading-screen hint tag: "<tag>_10" -> "_20" -> ... -> "_50" -> next tag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00497B60, length 1052 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0,
// CCharString::GetSubString @ 0x0099EC70, operator+(const CCharString&, const char*) @ 0x0099F600 (__fastcall, hidden result),
// CCharString::operator=(const CCharString&) @ 0x0099EFB0, operator=(const char*) @ 0x0099EFE0, operator!=(const char*) @ 0x0099E960,
// operator+=(const char*) @ 0x0099F100, CCharString::GetCRC @ 0x00404310, NGameText::CDataBank::GetTagFromCRC @ 0x009CC410,
// CDataBank::GetEntryDataType @ 0x009CC210.
// Shape: GetPTextBank (0x00497B30) and an EndsWith(const CCharString&) helper are inline (length difference, then the
// inline strcmp of the text tail; "" = 0x0129AAF4).
#include <string.h>
struct CCharStringData {
    char*         PData;                           // +0x00
    unsigned long StringLength;                    // +0x04
};
extern char g_EmptyCString[];                      // 0x0129AAF4
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString GetSubString(unsigned long start, unsigned long count) const;   // 0x0099EC70
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    CCharString& operator=(const char* text);          // 0x0099EFE0
    bool operator!=(const char* text) const;           // 0x0099E960
    CCharString& operator+=(const char* text);         // 0x0099F100
    unsigned long GetCRC() const;                      // 0x00404310
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetPString() const { return PStringData == 0 ? g_EmptyCString : PStringData->PData; }
    __forceinline bool EndsWith(const CCharString& tail) const
    {
        long offset = GetLength() - tail.GetLength();
        if (offset >= 0)
            return strcmp(GetPString() + offset, tail.GetPString()) == 0;
        return false;
    }
};
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600

class CDataBank {
public:
    unsigned long GetTagFromCRC(unsigned long crc) const;     // 0x009CC410
    unsigned long GetEntryDataType(unsigned long index) const; // 0x009CC210
};
struct CProgressTextBankSlot { unsigned char _pad[0x14]; CDataBank* TextBank; };
struct CGameTextBankSlot { unsigned char _pad[0x60]; CDataBank* TextBank; };
extern CProgressTextBankSlot* g_ProgressTextBankSlot;     // 0x013B86A0
extern CGameTextBankSlot* g_GameTextBankSlot;             // 0x013B871C

struct CCharStringVector {
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    CCharString& operator[](unsigned long i) { return _M_start[i]; }
};

class CProgressDisplay {
public:
    unsigned char _pad00[0x38];
    CCharStringVector TextTags;                    // +0x38 STLport vector<CCharString> (12 bytes in retail)
    unsigned long CurrentTag;                      // +0x44 index into TextTags, -1 = none yet
    void* WideText;                                // +0x48 CWideString
    CCharString TextTag;                           // +0x4C
    unsigned char _pad50[0x7C - 0x50];
    unsigned char DisplayText;                     // +0x7C
    void CalculateNextTextTag();
    CDataBank* GetPTextBank() const
    {
        if (g_ProgressTextBankSlot != 0 && g_ProgressTextBankSlot->TextBank != 0)
            return g_ProgressTextBankSlot->TextBank;
        if (g_GameTextBankSlot != 0 && g_GameTextBankSlot->TextBank != 0)
            return g_GameTextBankSlot->TextBank;
        return 0;
    }
};

void CProgressDisplay::CalculateNextTextTag()
{
    if (!DisplayText)
        return;
    CDataBank* bank = GetPTextBank();
    if (bank == 0)
        return;
    if (CurrentTag != (unsigned long)-1) {
        if (TextTag.EndsWith("_10"))
            TextTag = TextTag.GetSubString(0, TextTag.GetLength() - 3) + "_20";
        else if (TextTag.EndsWith("_20"))
            TextTag = TextTag.GetSubString(0, TextTag.GetLength() - 3) + "_30";
        else if (TextTag.EndsWith("_30"))
            TextTag = TextTag.GetSubString(0, TextTag.GetLength() - 3) + "_40";
        else if (TextTag.EndsWith("_40"))
            TextTag = TextTag.GetSubString(0, TextTag.GetLength() - 3) + "_50";
        else
            TextTag = "";
        if (TextTag != "" && bank->GetTagFromCRC(TextTag.GetCRC()) != 0)
            return;
    }
    if (TextTags.empty())
        return;
    if (++CurrentTag >= TextTags.size())
        CurrentTag = 0;
    TextTag = TextTags[CurrentTag];
    unsigned long tag = bank->GetTagFromCRC(TextTag.GetCRC());
    if (tag > 0 && bank->GetEntryDataType(tag) == 1)
        TextTag += "_10";
}
