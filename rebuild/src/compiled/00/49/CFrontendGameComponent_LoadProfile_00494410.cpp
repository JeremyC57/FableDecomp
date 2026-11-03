// CFrontendGameComponent::LoadProfile(const CWideString& profileName, bool loadProfile) -- hand the chosen profile name to the user profile manager (load it, or save under it)
// Leaf: ?LoadProfile@CFrontendGameComponent@@AAEXABVCWideString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00494410, 25 bytes, `ret 8`; `this` is not read.
// Identity (naming wave 34): Ego_r.pdb 0x0049D030 / FableWin.pdb 0x01A43680 ?LoadProfile@CFrontendGameComponent@@AAEXABVCWideString@@_N@Z
// (private); Ego_r donor order ChangeSelection 0x0049CFA0, LoadProfile 0x0049D030, Update ... = retail ChangeSelection 0x00494380,
// 0x00494410. The body forwards both arguments: was landed as the descriptive DispatchFrontendInputAction stand-in.
// Callees: CUserProfileManager::Get @ 0x0040D2A0 (static __fastcall, returns the manager by reference; Ego_r / FableWin
// ?Get@CUserProfileManager@@SIAAV1@XZ), CUserProfileManager::SetProfileName(const CWideString&, bool) @ 0x0040D400.
// Retail bytes:  mov eax,[esp+8] ; mov ecx,[esp+4] ; push eax ; push ecx ; call Get ; mov ecx,eax ; call SetProfileName ; ret 8
// (the arguments are pushed before the object expression is evaluated).
class CWideString;
class CUserProfileManager { public:
    static CUserProfileManager& __fastcall Get();                           // 0x0040D2A0
    void SetProfileName(const CWideString& profileName, bool loadProfile);  // 0x0040D400
};
class CFrontendGameComponent {
private:
    void LoadProfile(const CWideString& profileName, bool loadProfile);
public:
    void TestLoadProfile(const CWideString& profileName, bool loadProfile) { LoadProfile(profileName, loadProfile); }
};

void CFrontendGameComponent::LoadProfile(const CWideString& profileName, bool loadProfile)
{
    CUserProfileManager::Get().SetProfileName(profileName, loadProfile);
}
