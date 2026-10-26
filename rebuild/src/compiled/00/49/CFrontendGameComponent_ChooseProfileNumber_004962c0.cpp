// CFrontendGameComponent::ChooseProfileNumber(long) -- load the chosen debug-front-end profile (row 11 = default profile, row 12 = a new profile, else a listed one) and switch mode
// Leaf: ?ChooseProfileNumber@CFrontendGameComponent@@QAEXJ@Z (Ego_r.pdb AAEXJ @ 0x0049F070, private; modelled public)
// Retail: Fable.exe @ 0x004962C0, 219 bytes, `ret 4`.
// Body: index = ProfileNamesOffset + number - 1. Row 11: LoadProfile(Get().GetDefaultProfileName() (0x004065D0), true);
// row 12: LoadProfile(CreateNewProfile() (0x004945B0), false); otherwise if 0 <= index < ProfileNames.size():
// LoadProfile(ProfileNames[index], true); each followed by SetMode(1). LoadProfile (inline here) is
// CTBaseSingleton<CUserProfileManager>::Get() (0x0040D2A0).SetProfileName(name, load) (0x0040D400);
// SetMode (inline) stores Mode, CursorHighlight = 1, ClearString() (String[128] zeroed, StringPos = 0), and if Mode is 10
// calls GetWorldNames() (0x00495890; the manifest's PopulateProfiles / FillInListBox labels are stale -- it fills
// WorldNames +0x1e30, the mode-10 list). CWideString temporaries die via ~CWideString 0x0099B510.
// Names: LoadProfile / SetMode / ClearString / GetWorldNames / GetDefaultProfileName are Ego_r.pdb symbols; their
// inlined bodies here are reconstructed from retail.
// Shape: each branch is { LoadProfile(temp, flag); SetMode(1); } with both helpers forced inline: the CWideString temporary
// dies before SetMode, and the backend cross-jumps the three SetMode copies into one tail (each branch materialises the
// shared constant 1 in edi itself). A single SetMode after the if-chain hoists `mov edi,1` and merges the row-11/12 call
// tails; SetMode inside LoadProfile destroys the temporary after SetMode. ClearString must be memset then StringPos = 0;
// vector::operator[] must be STLport's *(begin() + n) (the begin pointer is copied to ecx); _M_start[n] differs.
// Layout: retail CFrontendGameComponent: Mode +0x1c, String +0x168, StringPos +0x1e8 (Ego_r offsets), ProfileNames
// +0x1e0c, then the STLport vector shrink (0x10 -> 0xc): ProfileNamesOffset +0x1e18 (Ego_r +0x1e1c), CursorHighlight
// +0x1e44 (Ego_r +0x1e50).
// Opt: /O2 (16-aligned entry).
#include <string.h>
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                      // 0x0099B510
};
class CUserProfileManager {
public:
    CWideString GetDefaultProfileName() const;           // 0x004065D0
    void SetProfileName(const CWideString& name, bool load);   // 0x0040D400
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                          // 0x0040D2A0 for CUserProfileManager
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
enum EGameMode { GAME_MODE_1 = 1, GAME_MODE_10 = 10 };
class CFrontendGameComponent {
public:
    unsigned char            _pad0000[0x1c];
    EGameMode                Mode;                       // +0x1c
    unsigned char            _pad0020[0x168 - 0x20];
    char                     String[128];                // +0x168
    long                     StringPos;                  // +0x1e8
    unsigned char            _pad01ec[0x1e0c - 0x1ec];
    std::vector<CWideString> ProfileNames;               // +0x1e0c
    long                     ProfileNamesOffset;         // +0x1e18
    unsigned char            _pad1e1c[0x1e44 - 0x1e1c];
    long                     CursorHighlight;            // +0x1e44
    CWideString CreateNewProfile();                      // 0x004945B0
    void ClearString() { memset(String, 0, sizeof(String)); StringPos = 0; }
    __forceinline void SetMode(EGameMode mode)
    {
        Mode = mode;
        CursorHighlight = 1;
        ClearString();
        if (Mode == GAME_MODE_10)
            GetWorldNames();
    }
    __forceinline void LoadProfile(const CWideString& name, bool load)
    {
        CTBaseSingleton<CUserProfileManager>::Get().SetProfileName(name, load);
    }
    void ChooseProfileNumber(long number);
private:
    void GetWorldNames();   // 0x00495890
};
void CFrontendGameComponent::ChooseProfileNumber(long number)
{
    long index = ProfileNamesOffset + number - 1;
    if (index == 11)
    {
        LoadProfile(CTBaseSingleton<CUserProfileManager>::Get().GetDefaultProfileName(), true);
        SetMode(GAME_MODE_1);
    }
    else if (index == 12)
    {
        LoadProfile(CreateNewProfile(), false);
        SetMode(GAME_MODE_1);
    }
    else if (index >= 0 && index < ProfileNames.size())
    {
        LoadProfile(ProfileNames[index], true);
        SetMode(GAME_MODE_1);
    }
}