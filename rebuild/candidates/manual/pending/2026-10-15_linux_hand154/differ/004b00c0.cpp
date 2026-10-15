// CQuestManager::IsQuestRegistered(const CCharString& name) -- true for "NULL" or when the quest name (the part before any '%') is in RegisteredQuestNames
// Grade: DIFFER(159v154) -- lane gap: retail keeps the quest-name local in the dead argument slot (no push-ecx frame word) and passes the empty random_access_iterator_tag of __find at that same live slot; ours opens a frame word. Also the inline operator==(const char*) arms are laid out differently (retail tail-merges the strcmp result into the true exit). Tried a ternary-initialised local (extra temporary + copy) and an inline by-value GetQuestNameFromSectionName helper (constructs in place)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B00C0, length 154 bytes (padded to 160), `ret 4`.
// Callees: CCharString::FindCharPos(char) @ 0x0099E5A0, CCharString::GetSubString(unsigned long, unsigned long) const
// @ 0x0099EC70, CCharString::CCharString(const CCharString&) @ 0x0099EC30, CBasicString<char>::operator==(const char*)
// @ 0x004115A0 ("NULL" @ 0x012393E4), STLport `__find(first, last, val, random_access_iterator_tag)` over CCharString
// @ 0x004B8FF0 (`__fastcall`: first in ecx, last in edx), CCharString::~CCharString @ 0x0099EAE0.
// Layout: retail RegisteredQuestNames (STLport vector<CCharString>) at +0x2c/+0x30 (landed GetRegisteredQuestFromIndex).
// Shape: the quest name comes from an inline by-value helper returning either GetSubString(0, pos) or a copy, both
// constructed straight into the local, which retail keeps in
// the dead argument slot; the inline CCharString::operator==(const char*) compares an unset string as "" with the
// strcmp intrinsic; `find(...) != end` gives `setne bl`.
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)
struct CBasicStringChar {
    bool operator==(const char* text) const;       // 0x004115A0
};
class CCharString {
public:
    CBasicStringChar* PStringData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    long FindCharPos(char c) const;                // 0x0099E5A0
    CCharString GetSubString(unsigned long start, unsigned long count) const;   // 0x0099EC70
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return *PStringData == text;
    }
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };
CCharString* __fastcall __find(CCharString* first, CCharString* last, const CCharString& val, const random_access_iterator_tag&);   // 0x004B8FF0
inline CCharString* find(CCharString* first, CCharString* last, const CCharString& val)
{
    return __find(first, last, val, random_access_iterator_tag());
}
inline CCharString GetQuestNameFromSectionName(const CCharString& name)   // the part before any '%'
{
    long pos = name.FindCharPos('%');
    if (pos > -1)
        return name.GetSubString(0, pos);
    return name;
}
struct CQuestManager {
    unsigned char _pad_0x00[0x2c];
    CCharString*  RegisteredQuestNames_start;      // +0x2c
    CCharString*  RegisteredQuestNames_finish;     // +0x30
    bool IsQuestRegistered(const CCharString& name);
};
bool CQuestManager::IsQuestRegistered(const CCharString& name)
{
    CCharString questName = GetQuestNameFromSectionName(name);
    if (questName == "NULL")
        return true;
    return find(RegisteredQuestNames_start, RegisteredQuestNames_finish, questName) != RegisteredQuestNames_finish;
}
