// CV_ChapelOfEvilScript::GetVictimIsGuard(CCharString) -- true when the victim's def name contains BS_SHERIFF or BS_GUARD (and the script thread is not terminating)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E6E050, length 202 bytes (`ret 4`; 208 with padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, strstr @ 0x00BFEB3A
// (`__cdecl`, the CRT import thunk), CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed). Globals: the
// CCharString empty text @ 0x0129AAF4.
// Identity: the catalog label stands (the caller is 0x00E6DB07 in the same quest's code; no member is touched).
// Shape: the two pattern tests are a `__forceinline` helper with a NAMED pattern local (a temporary in an `||`
// condition adds a conditional-destruction flag; a plain inline helper is out-lined at its second call). The name is a by-value CCharString destroyed on both exits; each pattern is a CCharString temporary whose
// text is read through the null-case-first GetPString accessor, the strstr result is tested and survives the
// temporary's destructor in bl.
// Shape: the two pattern tests are a `__forceinline` helper with a NAMED pattern local, their `||` in a NAMED bool (`found`);
// the by-value name has THREE destructor sites (`return true`, the terminating `return false` inside the found block, the
// not-found `return false`): sites 1 and 2 share the hoisted `lea ecx,[esp+10h]` above the terminating `jne`, and site 2
// cross-jumps into site 3 past its own lea (the two-site `if (!term) return true; } return false;` form puts the lea after the jne).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharStringData { public: const char* m_pText; unsigned long m_length; };
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->m_pText; }   // "" = 0x0129AAF4
};
extern "C" char* __cdecl strstr(const char* text, const char* pattern);   // 0x00BFEB3A

class CScriptBase { public:                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};

__forceinline bool NameContains(const CCharString& name, const char* text)
{
    CCharString pattern(text, -1);
    return strstr(name.GetPString(), pattern.GetPString()) ? true : false;
}

namespace NScript {
class CV_ChapelOfEvilScript : public CScriptBase {
public:
    bool GetVictimIsGuard(CCharString name);
};
} // namespace NScript
using NScript::CV_ChapelOfEvilScript;

bool NScript::CV_ChapelOfEvilScript::GetVictimIsGuard(CCharString name)
{
    bool found = NameContains(name, "BS_SHERIFF") || NameContains(name, "BS_GUARD");
    if (found) {
        if (!IsActiveThreadTerminating())
            return true;
        return false;
    }
    return false;
}