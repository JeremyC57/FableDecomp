// NUISystem::CFrontEndManager::CreateNewProfile(CWideString&) -- create and switch to a new profile: needs free space, makes its folder, resets it to defaults, keeps the new-profile camera inversion, saves, and resets the main menu
// Leaf: ?CreateNewProfile@CFrontEndManager@NUISystem@@QAE_NAAVCWideString@@@Z (Ego_r.pdb IAE @ 0x00602875, protected; modelled public)
// Retail: Fable.exe @ 0x0059697A, 238 bytes, `ret 4`.
// Body: ok = true; if !Get().IsEnoughSpaceForProfile() (0x004067C0) return false; Get().UseNewProfileValues = false;
// if Get().GetProfileDir(name, true) (0x004082D0, CWideString temporary) has GetLength() (0x0099B220) zero, ok = false
// (the bool is formed with neg/sbb/inc before ~CWideString 0x0099B510); Get().SetProfileName(name, false) (0x0040D400);
// Get().SetDefaultValues() (0x0040C790); Get().ApplyNewProfileValues() (0x0040CCA0, copies the *NewProfile mirror
// +0xcd.. over the live values); Get().SetCameraYZInverted(Get().IsCameraYZInverted(), true) (0x0040D2F0; the inline
// getter reads CameraYZInvertedNewProfile +0xd0 when UseNewProfileValues, else CameraYZInverted +0x10);
// Get().Save() (0x0040A740); SetMainMenuDefName(CCharString("UI_FRONTEND_MAIN_MENU_NO_LIVEAWARE_NO_CONTINUE"))
// (a named local in its own block, ctor 0x0099EBF0 with length -1, 0x00595A06, ~CCharString 0x0099EAE0);
// AddProfileNameToScreens(name) (0x00595B24); return ok. Get() is CTBaseSingleton<CUserProfileManager>::Get (0x0040D2A0).
// Names: IsEnoughSpaceForProfile / SetCameraYZInverted are Ego_r.pdb signature matches; SetDefaultValues (0x0040C790:
// stores the 1.0f / flag defaults), ApplyNewProfileValues (0x0040CCA0: copies +0xcd.. to +0xd..) and Save (0x0040A740:
// GetProfileDir then writes) are named from their bodies. 0x004945B0's src calls 0x0040C790 "Save" -- that name is wrong,
// it is the defaults reset.
// Layout: Ego_r.pdb CUserProfileManager UseNewProfileValues +0xc, CameraYZInverted +0x10; retail
// CameraYZInvertedNewProfile +0xd0 (Ego_r +0xd4, 4 lower).
// Shape: the CCharString must be a named local (a temporary pushes the ctor's eax) scoped to its own block: it dies before
// AddProfileNameToScreens and shares [ebp-8] with the GetProfileDir temporary (function scope gives a 0xc frame).
// Opt: size TU (unaligned entry) with a frame pointer (`leave`).
#pragma optimize("s", on)
#pragma optimize("y", off)
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                      // 0x0099B510
    long GetLength() const;                              // 0x0099B220
};
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CUserProfileManager {
public:
    unsigned char _pad00[0x0c];
    bool          UseNewProfileValues;                   // +0x0c
    unsigned char _pad0d[0x10 - 0x0d];
    bool          CameraYZInverted;                      // +0x10
    unsigned char _pad11[0xd0 - 0x11];
    bool          CameraYZInvertedNewProfile;            // +0xd0
    bool IsEnoughSpaceForProfile() const;                // 0x004067C0
    CWideString GetProfileDir(const CWideString& name, bool create);   // 0x004082D0
    void SetProfileName(const CWideString& name, bool load);           // 0x0040D400
    void SetDefaultValues();                             // 0x0040C790
    void ApplyNewProfileValues();                        // 0x0040CCA0
    bool IsCameraYZInverted() const { return UseNewProfileValues ? CameraYZInvertedNewProfile : CameraYZInverted; }
    void SetCameraYZInverted(bool inverted, bool apply); // 0x0040D2F0
    void Save();                                         // 0x0040A740
public:
    static CUserProfileManager& __fastcall Get();   // 0x0040D2A0
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                          // 0x0040D2A0 for CUserProfileManager
};
namespace NUISystem {
class CFrontEndManager {
public:
    bool CreateNewProfile(CWideString& name);
protected:
    void AddProfileNameToScreens(const CWideString&);   // 0x00595B24
    void SetMainMenuDefName(const CCharString&);   // 0x00595A06
};
bool CFrontEndManager::CreateNewProfile(CWideString& name)
{
    bool ok = true;
    if (!CUserProfileManager::Get().IsEnoughSpaceForProfile())
        return false;
    CUserProfileManager::Get().UseNewProfileValues = false;
    if (CUserProfileManager::Get().GetProfileDir(name, true).GetLength() == 0)
        ok = false;
    CUserProfileManager::Get().SetProfileName(name, false);
    CUserProfileManager::Get().SetDefaultValues();
    CUserProfileManager::Get().ApplyNewProfileValues();
    CUserProfileManager::Get().SetCameraYZInverted(CUserProfileManager::Get().IsCameraYZInverted(), true);
    CUserProfileManager::Get().Save();
    {
        CCharString def("UI_FRONTEND_MAIN_MENU_NO_LIVEAWARE_NO_CONTINUE");
        SetMainMenuDefName(def);
    }
    AddProfileNameToScreens(name);
    return ok;
}
}