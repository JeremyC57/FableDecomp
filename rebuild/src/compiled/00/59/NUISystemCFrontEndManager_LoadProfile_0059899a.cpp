// NUISystem::CFrontEndManager::LoadProfile(const CWideString& name) -- switch the profile manager to a named profile, pick the main-menu def by whether it has saves, and show the profile name
// Leaf: ?LoadProfile@CFrontEndManager@NUISystem@@IAEXABVCWideString@@@Z
// Retail: Fable.exe @ 0x0059899A, 130 bytes, `ret 4`; Ego_r.pdb / FableWin.pdb same symbol (protected).
// Body: CTBaseSingleton<CUserProfileManager>::Get() (0x0040D2A0, builds a 0x140-byte object; labelled
// CTBaseSingleton<CCodeSectionManager>::Get by folding).UseNewProfileValues (+0xc) = false; Get().SetProfileName(name, true)
// (0x0040D400, Ego_r.pdb ?SetProfileName@CUserProfileManager@@QAEXABVCWideString@@_N@Z; labelled
// CRenderManager2D::AddLineToDrawBuffer by folding); vector<CWideString> saves; GetSavedGameNames(saves) (0x00597DF7);
// SetMainMenuDefName(a CCharString of saves.empty() ? "UI_FRONTEND_MAIN_MENU_NO_LIVEAWARE_NO_CONTINUE" (0x01252374) :
// "UI_FRONTEND_MAIN_MENU_NO_LIVEAWARE" (0x012524E4))) (0x00595A06; CCharString(const char*, long = -1) 0x0099EBF0,
// ~CCharString 0x0099EAE0); AddProfileNameToScreens(name) (0x00595B24); ~vector<CWideString> 0x00412130 (labelled
// CCombatWheel::ResetRings by folding).
// Layout: Ego_r.pdb CUserProfileManager UseNewProfileValues +0xc (bool).
// Opt: size TU with frame pointer (`push ebp / mov ebp,esp`, `leave`, unaligned entry; `and [ebp-x],0` zero stores).
// Shape: a named CCharString in each branch of an if/else, tails cross-jumped (`push -1; jne; push A; jmp; push B; call`).
// A temporary reuses the ctor's eax for the reference (127v130), and a `const char*` ternary materialises the
// pointer in eax (126v130).
#pragma optimize("s", on)
#pragma optimize("y", off)
class CWideString {
public:
    void* PStringData;
};
class CCharString {
public:
    void* PStringData;
    CCharString(const char* s, long length = -1);        // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                          // 0x0040D2A0 for CUserProfileManager
};
class CUserProfileManager {
public:
    unsigned char _pad00[0xc];
    bool          UseNewProfileValues;                   // +0x0c
    void SetProfileName(const CWideString& name, bool load);   // 0x0040D400
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector();                                           // 0x00412130 for CWideString
    bool empty() const { return _M_start == _M_finish; }
};
}
namespace NUISystem {
class CFrontEndManager {
protected:
    void GetSavedGameNames(std::vector<CWideString>& names);   // 0x00597DF7
    void SetMainMenuDefName(const CCharString& name);          // 0x00595A06
    void AddProfileNameToScreens(const CWideString& name);     // 0x00595B24
    void LoadProfile(const CWideString& name);
    friend struct CFrontEndManager_Test;                 // behaviour-test hook
};
void CFrontEndManager::LoadProfile(const CWideString& name)
{
    CTBaseSingleton<CUserProfileManager>::Get().UseNewProfileValues = false;
    CTBaseSingleton<CUserProfileManager>::Get().SetProfileName(name, true);
    std::vector<CWideString> saves;
    GetSavedGameNames(saves);
    if (saves.empty())
    {
        CCharString def_name("UI_FRONTEND_MAIN_MENU_NO_LIVEAWARE_NO_CONTINUE");
        SetMainMenuDefName(def_name);
    }
    else
    {
        CCharString def_name("UI_FRONTEND_MAIN_MENU_NO_LIVEAWARE");
        SetMainMenuDefName(def_name);
    }
    AddProfileNameToScreens(name);
}
}