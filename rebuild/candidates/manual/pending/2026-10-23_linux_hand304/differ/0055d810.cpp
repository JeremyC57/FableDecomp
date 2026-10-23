// Grade: DIFFER(944v998) -- 336 difflines: the front (const CWideString& caption with the date string living in the dead argument slot, three forced-inline strstr ReplaceAll passes, the if-form GetString) is close (this kept in ebp in retail, in the frame here); the path tail is unresolved: retail builds seven wide temporaries through 0x0099E480 (name to wide), 0x00406350 (save dir + L"\\" + name, fastcall 4 args), 0x0041A600 (+ L".tga") and a 0x0099BE70/0x0099BF30 pair applied twice to [esp+20h] before the ScreenshotPath assignment; the model has a plain operator+ chain.
// CTCPhotojournalCapture::OnCaptionEntered(const CWideString& caption) -- keep the caption, build a file-safe name from the current local date and time ("\" and "/" to "-", ":" to "."), make the screenshot path from the profile's save directory and that name with a ".tga" extension, then close the capture
// Leaf: OnCaptionEntered
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055D810, 998 bytes, `ret 4`.
#include <string.h>
struct _SYSTEMTIME { unsigned short w[8]; };
extern "C" __declspec(dllimport) void __stdcall GetLocalTime(_SYSTEMTIME* time);   // [0x0143FE18]
struct CWideString {
    void* PStringData;
    ~CWideString();                                   // 0x0099B510
    CWideString& operator=(const CWideString& other); // 0x0099B7D0
};
struct CCharStringData { char* PData; };
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                    // 0x0099E4B0
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
    CCharString& operator=(const CCharString& other); // 0x0099EFB0
    CCharString& operator+=(const CCharString& other);   // 0x0099F0A0
    const char* GetString() const
    {
        const char* text = "";
        if (PStringData)
            text = PStringData->PData;
        return text;
    }
    CWideString GetWideString() const;                // 0x0099E480
};
struct CDateAndTime {
    unsigned char _data[0x20];
    CDateAndTime(const _SYSTEMTIME& time);            // 0x009E9A30
    CCharString FormatDateTimeString() const;         // 0x009E9E30
};
CWideString __fastcall operator+(const CWideString& a, const wchar_t* b);   // 0x00406350
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0041A600
struct CProfileManager {
    unsigned char _pad00[8];
    CWideString   SaveDir;                            // +0x08
    static CProfileManager* __fastcall Get();         // 0x0040D2A0
};

__forceinline void ReplaceAll(CCharString& text, const CCharString& find, const CCharString& replace)
{
    CCharString result;
    char* start = (char*)text.GetString();
    const char* key = find.GetString();
    for (char* hit = strstr(start, key); hit; hit = strstr(start, key)) {
        *hit = 0;
        hit += strlen(key);
        result += CCharString(start);
        result += replace;
        start = hit;
    }
    result += CCharString(start);
    text = result;
}

struct CTCPhotojournalCapture {
    virtual ~CTCPhotojournalCapture();
    unsigned char _pad04[0x0c - 0x04];
    CWideString   Caption;                            // +0x0c
    unsigned char _pad10[0x28 - 0x10];
    CWideString   ScreenshotPath;                     // +0x28
    void ClosePhotojournalCapture();                  // 0x0055D770
    void OnCaptionEntered(const CWideString& caption);
};

void CTCPhotojournalCapture::OnCaptionEntered(const CWideString& caption)
{
    Caption = caption;
    _SYSTEMTIME now;
    GetLocalTime(&now);
    CDateAndTime dateTime(now);
    CCharString name = dateTime.FormatDateTimeString();
    ReplaceAll(name, "\\", "-");
    ReplaceAll(name, "/", "-");
    ReplaceAll(name, ":", ".");
    CProfileManager* profiles = CProfileManager::Get();
    ScreenshotPath = profiles->SaveDir + L"\\" + name.GetWideString() + L".tga";
    ClosePhotojournalCapture();
}
