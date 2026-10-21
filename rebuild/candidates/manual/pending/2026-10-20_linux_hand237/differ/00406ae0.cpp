// Grade: DIFFER(342v314) -- frame packing: our build overlaps the GetTextBySymbol / operator+ temporaries with the dead number slots (0x0c frame) where retail keeps five separate words (0x14), so every later offset and the shared destructor tail differ
// CUserProfileManager::GetManualSaveDisplayName(long slot) -- the localised "manual save <n>" name for a save slot, from the main game component's text bank, else the front end's, else the file name
// Leaf: GetManualSaveDisplayName
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00406AE0, length 314 bytes (+ int3 padding), `ret 8` (CWideString result).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CWideString() @ 0x0099AED0,
// CWideString::SetFromFormat(const wchar_t*, ...) @ 0x0099BA70 (cdecl), CWideString(const CWideString&) @ 0x0099B720,
// ~CWideString @ 0x0099B510, CDataBank::GetTextBySymbol(const CCharString&) @ 0x009C95E0, operator+(const CWideString&,
// const wchar_t*) @ 0x0099BF30 and operator+(const CWideString&, const CWideString&) @ 0x0099BE70 (`__fastcall`), the
// int-to-wide-string helper @ 0x00405F00 (out of line in the second arm, inlined in the first),
// CUserProfileManager::GetManualSaveFileName(long) @ 0x00406610.
// Globals: g_pMainGameComponent @ 0x013B86A0 (TextBank +0x14), the front-end component @ 0x013B871C (its text bank
// +0x60); strings 0x0122DF9C "TEXT_GUI_SAVE_MANUAL_SAVE_NAME", 0x0122DE44 L"%d", 0x0122D820 L" ".
// Shape: the number conversion is inlined once (format into a local, copied out: VC7.1 has no NRVO) and called out of
// line in the second arm; both arms build `text + L" " + number` straight into the result and share the destructor tail.
typedef unsigned short wchar_t;                                // VC7.1 without /Zc:wchar_t
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
    CWideString();                                             // 0x0099AED0
    CWideString(const CWideString& other);                     // 0x0099B720
    ~CWideString();                                            // 0x0099B510
};
void __cdecl SetFromFormat(CWideString* out, const wchar_t* format, ...);   // 0x0099BA70
CWideString __fastcall operator+(const CWideString& left, const wchar_t* right);      // 0x0099BF30
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);  // 0x0099BE70
CWideString __fastcall GFIntToWideString(long value);         // 0x00405F00
__forceinline CWideString IntToWideStringInline(long value)
{
    CWideString result;
    SetFromFormat(&result, L"%d", value);
    return result;
}
class CDataBank {
public:
    CWideString GetTextBySymbol(const CCharString& symbol);    // 0x009C95E0
};
struct CMainGameComponent { unsigned char _pad[0x14]; CDataBank* TextBank; };   // +0x14
struct CFrontEndComponent { unsigned char _pad[0x60]; CDataBank* TextBank; };   // +0x60
extern CMainGameComponent* g_pMainGameComponent;               // 0x013B86A0
extern CFrontEndComponent* g_pFrontEndComponent;               // 0x013B871C

class CUserProfileManager {
public:
    CWideString GetManualSaveFileName(long slot);              // 0x00406610
    CWideString GetManualSaveDisplayName(long slot);
};

CWideString CUserProfileManager::GetManualSaveDisplayName(long slot)
{
    if (CMainGameComponent* game = g_pMainGameComponent) {
        CCharString symbol("TEXT_GUI_SAVE_MANUAL_SAVE_NAME");
        CWideString number = IntToWideStringInline(slot);
        return game->TextBank->GetTextBySymbol(symbol) + L" " + number;
    }
    if (CFrontEndComponent* frontEnd = g_pFrontEndComponent) {
        CCharString symbol("TEXT_GUI_SAVE_MANUAL_SAVE_NAME");
        return frontEnd->TextBank->GetTextBySymbol(symbol) + L" " + GFIntToWideString(slot);
    }
    return GetManualSaveFileName(slot);
}
