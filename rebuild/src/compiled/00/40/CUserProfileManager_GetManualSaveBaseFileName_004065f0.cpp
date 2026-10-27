// CUserProfileManager::GetManualSaveBaseFileName() const -- the base file name of a manual save, L"Save"
// Leaf: ?GetManualSaveBaseFileName@CUserProfileManager@@QBE?AVCWideString@@XZ
// Retail: Fable.exe @ 0x004065F0, 23 bytes, __fastcall `ret 4` (hidden CWideString result, this unused).
// Identity (probable): sits between the CUserProfileManager save-name getters (0x004065D0 returns L"Default",
// 0x00406690 L"AutoSave", 0x00406610 GetManualSaveFileName formats with L"%d"); of the PDB's no-argument
// CUserProfileManager getters returning CWideString, GetManualSaveBaseFileName is the one that yields L"Save".
// Callee: CWideString::CWideString(const unsigned short*) @ 0x0099B6B0 (bound ??0CWideString@@QAE@PBG@Z).
// Literal: L"Save" @ 0x0122DE90.
class CWideString {
public:
    void* PStringData;
    CWideString(const unsigned short* text);    // 0x0099B6B0
    CWideString(const CWideString& other);      // 0x0099B720
    ~CWideString();                             // 0x0099B510
};

class CUserProfileManager {
public:
    CWideString GetManualSaveBaseFileName() const;
};

CWideString CUserProfileManager::GetManualSaveBaseFileName() const
{
    return CWideString((const unsigned short*)L"Save");
}