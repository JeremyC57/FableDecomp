// CUserProfileManager::GetAutoSaveRegionLeavingPathName() const -- the profile directory followed by the RegionLeaving auto-save file name (L"AutoSave.rl")
// Leaf: ?GetAutoSaveRegionLeavingPathName@CUserProfileManager@@QBE?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004080B0, 102 bytes, thiscall, hidden CWideString result, `ret 4`.
// Identity (proven): it inlines GetAutoSaveRegionLeavingFileName (the L".rl" literal 0x0122DEC0; the out-of-line copy is 0x00406730) after
//   GetDirectoryNameFromProfileName 0x004071C0; Ego_r.pdb/FableWin.pdb ?GetAutoSaveRegionLeavingPathName@CUserProfileManager@@QBE?AVCWideString@@XZ (the catalog binding);
//   callers 004A0180, 004A21F0, 004A2D70. The manifest label GetAutoSaveCheckPointPathName is propagated across the three byte twins.
// Callees: CUserProfileManager::GetDirectoryNameFromProfileName 0x004071C0, CWideString::CWideString(const unsigned short*)
//   0x0099B6B0, operator+(const CWideString&, const unsigned short*) 0x0099BF30, operator+(const CWideString&, const
//   CWideString&) 0x0099BE70 (both __fastcall, hidden result in ecx), CWideString::~CWideString 0x0099B510.
// Shape (family M006, representative 0x00407FD0): the directory is a NAMED local (evaluated first and passed by `lea edx`;
//   as an unnamed left operand VC7.1 evaluates the file name first and passes the directory in eax); the file-name member is
//   defined above in the same TU, so VC7.1 inlines it (its named L"AutoSave" local, then the sum temporary).
class CWideString {
public:
    CWideString(const unsigned short* text);              // 0x0099B6B0
    ~CWideString();                                       // 0x0099B510
    void* PStringData;
};
CWideString __fastcall operator+(const CWideString& left, const unsigned short* right);   // 0x0099BF30
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);      // 0x0099BE70
class CUserProfileManager {
public:
    CWideString GetDirectoryNameFromProfileName() const;  // 0x004071C0
    CWideString GetAutoSaveRegionLeavingFileName() const;    // 0x00406730 (inlined here)
    CWideString GetAutoSaveRegionLeavingPathName() const;
};
CWideString CUserProfileManager::GetAutoSaveRegionLeavingFileName() const
{
    CWideString name((const unsigned short*)L"AutoSave");
    return name + (const unsigned short*)L".rl";
}
CWideString CUserProfileManager::GetAutoSaveRegionLeavingPathName() const
{
    CWideString directory = GetDirectoryNameFromProfileName();
    return directory + GetAutoSaveRegionLeavingFileName();
}