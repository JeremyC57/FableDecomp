// NUISystem::CFrontEndManager::DoPressStart() -- "press start": route to profile creation, straight into the only profile, or to the profile menu
// Retail 0x00599D5C, 227 bytes, plain `ret` (Ego_r.pdb ?DoPressStart@CFrontEndManager@NUISystem@@IAEXXZ).
// Body: profiles = GetProfileNames() (0x005955AB, into a local vector<CWideString>); no profiles ->
// GotoNewProfileScreen() (0x00595845). One profile whose save signature checks out (VerifySignatureCached
// 0x005957D9 on CTBaseSingleton<CUserProfileManager>::Get() (0x0040D2A0).GetProfileDir(profiles[0], false) (0x004082D0, a CWideString
// temporary destroyed only when built) -> LoadProfile(profiles[0]) (0x0059899A) and GotoNextScreen(m_mapScreens[0],
// true) (map::operator[] 0x0059B5D7, GotoNextScreen 0x00596763). Otherwise GotoProfileMenu() (0x00597B20) and the
// profile manager re-applies its values (UseNewProfileValues +0x0C = true, then 0x0040BC80). Every path ends by
// handing the frontend's last active joystick (g_pNewFrontendGameComponent 0x013B871C, +0x140) to
// CGameJoystickManager::Get() (0x0049BB90).SetMainPlayerJoystickDeviceNumber (0x0049BB30); ~vector<CWideString>
// (0x00412130) closes the function.
// Shape: frame-pointer size TU (`optimize("s")` + `("y",off)`, as the landed GotoProfileMenu); the single-profile
// test is a named bool, so the GetProfileDir temporary's construction flag sits in a frame word ([ebp-8], zeroed at
// entry) and the result in a frame byte ([ebp-1]); the screen key temporary reuses the dead string slot.
// Layout: m_mapScreens +0x54 as the landed CFrontEndManager rows; CUserProfileManager UseNewProfileValues +0x0C
// (Ego_r.pdb); CNewFrontendGameComponent LastActiveJoystickDeviceNumber at retail +0x140 (Ego_r.pdb +0x148).
// Leaf: ?DoPressStart@CFrontEndManager@NUISystem
#pragma optimize("s", on)
#pragma optimize("y", off)

class CWideString
{
public:
    void* PData;
    ~CWideString();                                       // 0x0099B510
};

namespace std {
template <class T> class vector
{
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector();                                            // 0x00412130 for CWideString (same-TU body below)
    T* begin() const { return _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};
template <class K, class V> class map
{
public:
    void* _body[3];
    V& operator[](const K& key);                          // 0x0059B5D7 for <FRONTEND_SCREEN, CComponent*>
};
}

extern "C" void __cdecl free(void* block);

#pragma auto_inline(off)
// Same-TU body of the vector<CWideString> destructor COMDAT (0x00412130): destroy the names, free the block (a
// declared-only destructor would also take the vector's address).
template <> __declspec(noinline) std::vector<CWideString>::~vector()
{
    for (CWideString* name = _M_start; name != _M_finish; ++name)
        name->~CWideString();
    if (_M_start)
        free(_M_start);
}
#pragma auto_inline(on)

class CUserProfileManager
{
public:
    unsigned char _pad00[0x0c];
    bool UseNewProfileValues;                             // +0x0C
    CWideString GetProfileDir(const CWideString& name, bool create);   // 0x004082D0
    void Apply();                                         // 0x0040BC80
public:
    static CUserProfileManager& __fastcall Get();   // 0x0040D2A0
};

template <class T> class CTBaseSingleton
{
public:
    static T& __fastcall Get();                           // 0x0040D2A0 for CUserProfileManager
};

class CGameJoystickManager
{
public:
    static CGameJoystickManager& __fastcall Get();        // 0x0049BB90
    void SetMainPlayerJoystickDeviceNumber(long device);  // 0x0049BB30
};

class CNewFrontendGameComponent
{ public:
    unsigned char _pad[0x140];
    long LastActiveJoystickDeviceNumber;                  // +0x140
};
extern CNewFrontendGameComponent* g_pNewFrontendGameComponent;   // 0x013B871C

enum FRONTEND_SCREEN { FRONTEND_SCREEN_0 = 0 };

namespace NUISystem {
class CComponent;
class CFrontEndManager
{
public:
    unsigned char                          _pad00[0x54];
    std::map<FRONTEND_SCREEN, CComponent*> m_mapScreens;  // +0x54
    void GetProfileNames(std::vector<CWideString>& names);    // 0x005955AB (same-TU stand-in below)
    void GotoNewProfileScreen();                          // 0x00595845
    bool VerifySignatureCached(const CWideString& filename);   // 0x005957D9
    void LoadProfile(const CWideString& name);            // 0x0059899A
    void GotoNextScreen(CComponent* screen, bool clearStack);  // 0x00596763
    void GotoProfileMenu();                               // 0x00597B20
    void DoPressStart();
};

extern std::vector<CWideString> g_standInProfileScan;   // stand-in data only (no retail counterpart)

#pragma auto_inline(off)
// Same-TU stand-in for GetProfileNames (0x005955AB, same TU in retail; the real body scans the save directory and
// push_backs each profile name): it rewrites the caller's vector words and keeps no pointer to it, so VC7.1 knows the
// vector does not escape and keeps its begin pointer in edi across the later calls, as retail does. Declared only,
// `profiles` escapes and every use re-reads [ebp-18h] (228 v 227).
__declspec(noinline) void CFrontEndManager::GetProfileNames(std::vector<CWideString>& names)
{
    names._M_start = g_standInProfileScan._M_start;
    names._M_finish = g_standInProfileScan._M_finish;
    names._M_end_of_storage = g_standInProfileScan._M_end_of_storage;
}
#pragma auto_inline(on)

void CFrontEndManager::DoPressStart()
{
    std::vector<CWideString> profiles;
    GetProfileNames(profiles);
    if (profiles.empty())
    {
        GotoNewProfileScreen();
    }
    else
    {
        bool loadDirectly = profiles.size() == 1
            && VerifySignatureCached(CUserProfileManager::Get().GetProfileDir(profiles[0], false));
        if (loadDirectly)
        {
            LoadProfile(profiles[0]);
            GotoNextScreen(m_mapScreens[FRONTEND_SCREEN_0], true);
        }
        else
        {
            GotoProfileMenu();
            CUserProfileManager::Get().UseNewProfileValues = true;
            CUserProfileManager::Get().Apply();
        }
    }
    CGameJoystickManager::Get().SetMainPlayerJoystickDeviceNumber(g_pNewFrontendGameComponent->LastActiveJoystickDeviceNumber);
}
}
