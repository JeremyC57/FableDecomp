// CV_ChapelOfEvilScript::GetVictimIsGuard(CCharString) -- true when the victim's def name contains BS_SHERIFF or BS_GUARD (and the script thread is not terminating)
// Grade: DIFFER(202v202) -- only the terminating-check exit: retail hoists `lea ecx,[esp+10h]` above `jne` into the shared false tail (jne lands after that tail's lea); ours puts the lea after the jne. The `if (term) return false; return true;` form hoists the lea but moves the false tail before the true one.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E6E050, length 202 bytes (`ret 4`; 208 with padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, strstr @ 0x00BFEB3A
// (`__cdecl`, the CRT import thunk), CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed). Globals: the
// CCharString empty text @ 0x0129AAF4.
// Identity: the catalog label stands (the caller is 0x00E6DB07 in the same quest's code; no member is touched).
// Shape: the two pattern tests are a `__forceinline` helper with a NAMED pattern local (a temporary in an `||`
// condition adds a conditional-destruction flag; a plain inline helper is out-lined at its second call). The name is a by-value CCharString destroyed on both exits; each pattern is a CCharString temporary whose
// text is read through the null-case-first GetPString accessor, the strstr result is tested and survives the
// temporary's destructor in bl.
struct CCharStringData { const char* m_pText; unsigned long m_length; };
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->m_pText; }   // "" = 0x0129AAF4
};
extern "C" char* __cdecl strstr(const char* text, const char* pattern);   // 0x00BFEB3A

struct CScriptBase {                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};

__forceinline bool NameContains(const CCharString& name, const char* text)
{
    CCharString pattern(text, -1);
    return strstr(name.GetPString(), pattern.GetPString()) ? true : false;
}

struct CV_ChapelOfEvilScript : CScriptBase {
    bool GetVictimIsGuard(CCharString name);
};

bool CV_ChapelOfEvilScript::GetVictimIsGuard(CCharString name)
{
    if (NameContains(name, "BS_SHERIFF") || NameContains(name, "BS_GUARD")) {
        if (!IsActiveThreadTerminating())
            return true;
    }
    return false;
}
