// CV_ChapelOfEvilScript::GetVictimIsBandit(CCharString victim_def) -- true when the victim's definition name starts with "CREATURE_BANDIT" and the script thread is not terminating
// Leaf: GetVictimIsBandit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E6E120, length 134 bytes (padded to 144), `ret 4` (the string by value).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("CREATURE_BANDIT" @ 0x012E8B00), strncmp
// @ 0x00BFEAF8 (inline prefix test: the prefix's text and length share one null test, "" = 0x0129AAF4),
// ~CCharString @ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating @ 0x00CB7940.
// Shape: the prefix is a named local in its own block (destroyed before the terminating test) with the result in a
// bool declared outside; `strncmp(...) ? false : true` gives retail's 8-bit `neg ; sbb ; inc`.
// Shape: the prefix compare into a NAMED bool, then THREE destructor sites for the by-value parameter (`return true`, the
// terminating `return false` inside the found block, the not-found `return false`): the lea of sites 1/2 is hoisted above the
// terminating `jne` and site 2 cross-jumps into site 3 past its own lea.
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int n);   // 0x00BFEAF8
extern "C" char* __cdecl strstr(const char* text, const char* pattern);          // 0x00BFEB3A
struct CCharStringData {                           // the shared string block
    char* m_pData;                                 // +0x00
    long  m_length;                                // +0x04
};
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                // 0x0099EAE0
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->m_pData; }   // "" = 0x0129AAF4
    long Length() const { return PStringData == 0 ? 0 : PStringData->m_length; }
};
struct CScriptBase {
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating() const;        // 0x00CB7940
};
struct CV_ChapelOfEvilScript : CScriptBase {
    bool GetVictimIsBandit(CCharString victim_def);
};
bool CV_ChapelOfEvilScript::GetVictimIsBandit(CCharString victim_def)
{
    bool is_bandit;
    {
        CCharString prefix("CREATURE_BANDIT");
        is_bandit = strncmp(victim_def.GetPString(), prefix.GetPString(), prefix.Length()) ? false : true;
    }
    if (is_bandit)
    {
        if (!IsActiveThreadTerminating())
            return true;
        return false;
    }
    return false;
}