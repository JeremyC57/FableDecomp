// CUserProfileManager::GetProfileDir(const CWideString& name, bool create) -- path of another profile's Profile.bin: temporarily swap ProfileName to `name`, resolve through GetPathNameFromFileName, restore the name
// Leaf: ?GetProfileDir@CUserProfileManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004082D0, 110 bytes (+2 int3 pad). Callees: CWideString
// copy ctor 0x0099B720, CWideString::operator= 0x0099B7D0, CUserProfileManager::GetPathNameFromFileName
// 0x00408120 (landed; const, hidden result, `ret 0xc`), ~CWideString 0x0099B510; the static
// CWideString L"Profile.bin" at 0x013B7D54 (landed dynamic initializer 0x01219EC0).
//
// Retail bytes:  sub esp,8 ; push esi ; push edi ; mov edi,ecx ; lea esi,[edi+8] ; push esi ;
//   lea ecx,[saved] ; call CWideString(copy) ; push name ; mov ecx,esi ; call operator= ;
//   push create ; push &g_ProfileFileName ; lea edx,[path] ; push edx ; mov ecx,edi ;
//   call GetPathNameFromFileName ; lea eax,[saved] ; push eax ; mov ecx,esi ; call operator= ;
//   mov esi,[result] ; push &path ; mov ecx,esi ; call CWideString(copy) ; ~path ; ~saved ;
//   pop edi ; mov eax,esi ; pop esi ; add esp,8 ; ret 0xc
// Identity: FableWin.pdb `?GetProfileDir@CUserProfileManager@@QAE?AVCWideString@@ABV2@_N@Z`; the
// manifest label GetDefaultVal<CSpecialEffectsStringMap> is propagated. Ego_r.pdb
// CUserProfileManager: ProfileName (CWideString) +0x08, addressed once into esi (a member
// reference). No NRVO in VC7.1: the named `path` is copied into the hidden result, then the two
// locals die in reverse order. `create` is forwarded as the full argument dword.
class CWideStringData;

class CWideString {
public:
    CWideString(const CWideString& other);        // 0x0099B720
    ~CWideString();                               // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
private:
    CWideStringData* PStringData;
};

extern CWideString g_ProfileFileName;             // 0x013B7D54, L"Profile.bin"

class CUserProfileManager {
public:
    virtual void v00();
    long _unknown04;                              // +0x04
    CWideString ProfileName;                      // +0x08
    CWideString GetPathNameFromFileName(const CWideString& fileName, bool create) const;   // 0x00408120
    CWideString GetProfileDir(const CWideString& name, bool create);
};

CWideString CUserProfileManager::GetProfileDir(const CWideString& name, bool create)
{
    CWideString savedName = ProfileName;
    ProfileName = name;
    CWideString path = GetPathNameFromFileName(g_ProfileFileName, create);
    ProfileName = savedName;
    return path;
}