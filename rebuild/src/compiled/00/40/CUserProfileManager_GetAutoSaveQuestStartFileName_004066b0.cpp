// CUserProfileManager::GetAutoSaveQuestStartFileName() const -- the auto-save file name for a QuestStart save: L"AutoSave" + L".qs"
// Leaf: ?GetAutoSaveQuestStartFileName@CUserProfileManager@@QBE?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004066B0, 52 bytes, thiscall (`this` unused), hidden CWideString result, `ret 4`.
// Identity (proven): it appends L".qs" (0x0122DEB0) to L"AutoSave" (0x0122DE9C); callers CWorld::AutoSaveQuestStart 0x004A0CD0 (call sites in 004A0CD0, 004A5E90); FableWin.pdb/Ego_r.pdb
//   ?GetAutoSaveQuestStartFileName@CUserProfileManager@@QBE?AVCWideString@@XZ (the catalog binding of 0x004066B0); the manifest
//   label GetAutoSaveCheckPointFileName is propagated across the three byte twins.
// Callees: CWideString::CWideString(const unsigned short*) 0x0099B6B0, operator+(const CWideString&, const unsigned short*)
//   0x0099BF30 (__fastcall: hidden result in ecx, the string in edx), CWideString::~CWideString 0x0099B510.
// Shape (family M005, representative 0x004066B0): the L"AutoSave" CWideString is a NAMED local (the `push ecx` word, built
//   before the extension is pushed and passed by `lea edx`; a temporary in the expression is built after the push and passes
//   the ctor's eax); the sum is built straight into the hidden result.
class CWideString {
public:
    CWideString(const unsigned short* text);              // 0x0099B6B0
    ~CWideString();                                       // 0x0099B510
    void* PStringData;
};
CWideString __fastcall operator+(const CWideString& left, const unsigned short* right);   // 0x0099BF30
class CUserProfileManager {
public:
    CWideString GetAutoSaveQuestStartFileName() const;
};
CWideString CUserProfileManager::GetAutoSaveQuestStartFileName() const
{
    CWideString name((const unsigned short*)L"AutoSave");
    return name + (const unsigned short*)L".qs";
}