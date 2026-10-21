// CFrontendGameComponent::CreateNewProfile() -- name a new profile "NewProfile<n>" after the existing ones, create its folder, select it and save it
// Leaf: ?CreateNewProfile@CFrontendGameComponent@@QAE?AVCWideString@@XZ (Ego_r.pdb AAE, private; modelled public)
// Retail: Fable.exe @ 0x004945B0, 199 bytes, `ret 4` (hidden return buffer).
// Body: number = ProfileNames.size() + 1; a CWideString (default ctor 0x0099AED0) Format(L"%d", number) (0x0099BA70,
// cdecl varargs member, literal 0x0122DE44) returned by value from an inline helper (copy ctor 0x0099B720, ~CWideString
// 0x0099B510); name = L"NewProfile" + that (operator+(const wchar_t*, const CWideString&) 0x0099BFF0, __fastcall with the
// hidden result in ecx, literal 0x012384F0); CTBaseSingleton<CUserProfileManager>::Get() (0x0040D2A0)
// .GetProfileDir(name, true) (0x004082D0, result discarded); Get().SetProfileName(name, false) (0x0040D400);
// Get().SetDefaultValues() (0x0040C790); return name.
// Names: GetProfileDir / SetProfileName are Ego_r.pdb symbols matched by signature; 0x0040C790 is SetDefaultValues by its body
// (it stores the 1.0f / flag defaults; first named Save here, corrected at 0x0059697A); the number-to-string helper is unnamed.
// Layout: Ego_r.pdb CFrontendGameComponent ProfileNames +0x1e0c (retail STLport vector, as the matched 0x00495780).
// Opt: /O2 (16-aligned entry).
#include <stddef.h>
class CWideString {
public:
    void* PStringData;
    CWideString();                                       // 0x0099AED0
    CWideString(const CWideString& other);               // 0x0099B720
    ~CWideString();                                      // 0x0099B510
    void __cdecl Format(const wchar_t* format, ...);     // 0x0099BA70
};
CWideString __fastcall operator+(const wchar_t* left, const CWideString& right);   // 0x0099BFF0
inline CWideString NumberToWideString(long number)
{
    CWideString text;
    text.Format(L"%d", number);
    return text;
}
class CUserProfileManager {
public:
    CWideString GetProfileDir(const CWideString& name, bool create);   // 0x004082D0
    void SetProfileName(const CWideString& name, bool load);           // 0x0040D400
    void SetDefaultValues();                                           // 0x0040C790 (stores the defaults; see 0x0059697A)
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
};
}
class CFrontendGameComponent {
public:
    unsigned char            _pad0000[0x1e0c];
    std::vector<CWideString> ProfileNames;               // +0x1e0c
    CWideString CreateNewProfile();
};
CWideString CFrontendGameComponent::CreateNewProfile()
{
    CWideString name = L"NewProfile" + NumberToWideString(ProfileNames.size() + 1);
    CTBaseSingleton<CUserProfileManager>::Get().GetProfileDir(name, true);
    CTBaseSingleton<CUserProfileManager>::Get().SetProfileName(name, false);
    CTBaseSingleton<CUserProfileManager>::Get().SetDefaultValues();
    return name;
}