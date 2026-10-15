// CSystemManager::DisplayCriticalMessage(const char* message) -- show a fatal-error box: wrap the message in a CCharString, widen it (ToWideString) and pass it to MessageBoxW with no owner window, the caption L"Application Error" and MB_OK
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A4ED0, length 75 bytes.
// Callees: CCharString::CCharString(const char*, long = -1) @ 0x0099EBF0, CCharString::ToWideString @ 0x0099E480
// (hidden CWideString result pushed), CWideString::operator const wchar_t* @ 0x0099AF10, user32!MessageBoxW
// (`call [__imp]` @ 0x014402C8), ~CWideString @ 0x0099B510, ~CCharString @ 0x0099EAE0; caption @ 0x0129AB70.
// Shape: `push ecx` = the CWideString temporary's frame slot; the CCharString is built in the DEAD slot of the
// `message` argument (`lea ecx,[esp+10h]`); the wide temporary dies at the end of the call statement, the
// string at scope end; `ret 4` with `this` never read (a non-static member).
// Model: CCharString / CWideString are declared locally (their members are out of line in retail).
extern "C" __declspec(dllimport) int __stdcall MessageBoxW(void* window, const unsigned short* text,
    const unsigned short* caption, unsigned int type);   // 0x014402C8

class CWideString {
public:
    unsigned short* m_pData;
    ~CWideString();                                // 0x0099B510
    operator const unsigned short*() const;        // 0x0099AF10
};

class CCharString {
public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
    CWideString ToWideString() const;              // 0x0099E480
};

struct CSystemManager {
    void DisplayCriticalMessage(const char* message);   // `this` unused
};

void CSystemManager::DisplayCriticalMessage(const char* message)
{
    CCharString text(message);
    MessageBoxW(0, text.ToWideString(), L"Application Error", 0);   // caption 0x0129AB70, MB_OK
}
