// Behaviour test for CSubtitleRenderer::GetVisibleText (0x0048DE8A): "(HC)" is stripped (result false), the text is cut to the revealed count while revealing, else the reveal completes.
#include <cstdio>
#include <cstring>
struct CCharStringData {
    const char*   Text;                                  // +0x00
    long          Length;                                // +0x04
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->Text; }
};
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                      // 0x0099B510
    CWideString& operator=(const CWideString& other);    // 0x0099B7D0
    CCharString ConvertToCharString() const;             // 0x0099B2C0
    long GetLength() const;                              // 0x0099B220
    CWideString GetSubString(unsigned long start, unsigned long end) const;   // 0x0099BDB0
    void SetLength(long length);                         // 0x0099BC50
};

static inline bool BeginsWith(const CCharString& text, const CCharString& prefix)
{
    const char* prefixText;
    long prefixLength;
    if (prefix.PStringData == 0) {
        prefixLength = 0;
        prefixText = "";
    } else {
        prefixLength = prefix.PStringData->Length;
        prefixText = prefix.PStringData->Text;
    }
    return strncmp(text.GetPString(), prefixText, prefixLength) ? false : true;
}

class CSubtitleRenderer {
public:
    void*         __vftable;                             // +0x00
    unsigned char _pad04[0x24 - 0x04];
    long          TextCharactersShowing;                 // +0x24
    unsigned char _pad28[0xdd - 0x28];
    bool          AllCharactersDisplayed;                // +0xdd
    bool          InCutscene;                            // +0xde
    bool GetVisibleText(const CWideString& text, CWideString& visible);
};

bool CSubtitleRenderer::GetVisibleText(const CWideString& text, CWideString& visible)
{
    visible = text;
    bool hideCursor = BeginsWith(visible.ConvertToCharString(), "(HC)");
    if (hideCursor)
        visible = visible.GetSubString(4, visible.GetLength());
    if (!InCutscene && !AllCharactersDisplayed) {
        long showing = TextCharactersShowing;
        if (showing < visible.GetLength()) {
            visible.SetLength(showing);
            return !hideCursor;
        }
    }
    TextCharactersShowing = visible.GetLength();
    AllCharactersDisplayed = true;
    return !hideCursor;
}
struct WData { char s[64]; };
static WData g_pool[64]; static int g_used = 0;
static WData* NewW(const char* s, int n) { WData* w = &g_pool[g_used++]; std::memset(w->s, 0, sizeof w->s); std::memcpy(w->s, s, n); return w; }
static CCharStringData g_cpool[16]; static int g_cused = 0; static char g_ctext[16][64];
CCharString::CCharString(const char* text, long length)
{
    int i = g_cused++; std::strcpy(g_ctext[i], text);
    g_cpool[i].Text = g_ctext[i]; g_cpool[i].Length = length < 0 ? (long)std::strlen(text) : length; PStringData = &g_cpool[i];
}
CCharString::~CCharString() { }
CWideString::~CWideString() { }
CWideString& CWideString::operator=(const CWideString& o) { WData* w = (WData*)o.PStringData; PStringData = NewW(w->s, (int)std::strlen(w->s)); return *this; }
CCharString CWideString::ConvertToCharString() const { return CCharString(((WData*)PStringData)->s, -1); }
long CWideString::GetLength() const { return (long)std::strlen(((WData*)PStringData)->s); }
CWideString CWideString::GetSubString(unsigned long start, unsigned long end) const
{
    CWideString r; WData* w = (WData*)PStringData; r.PStringData = NewW(w->s + start, (int)(end - start)); return r;
}
void CWideString::SetLength(long length) { ((WData*)PStringData)->s[length] = 0; }
static bool Run(const char* in, long showing, bool done, bool cutscene, const char* wantText, bool wantResult, long wantShowing, bool wantDone)
{
    static CSubtitleRenderer r;
    CWideString text, visible; text.PStringData = NewW(in, (int)std::strlen(in)); visible.PStringData = NewW("", 0);
    r.TextCharactersShowing = showing; r.AllCharactersDisplayed = done; r.InCutscene = cutscene;
    bool result = r.GetVisibleText(text, visible);
    return result == wantResult && !std::strcmp(((WData*)visible.PStringData)->s, wantText)
        && r.TextCharactersShowing == wantShowing && r.AllCharactersDisplayed == wantDone;
}
int main()
{
    bool ok = Run("Hello there", 5, false, false, "Hello", true, 5, false)           /* revealing: cut to 5 */
           && Run("(HC)Hello there", 3, false, false, "Hel", false, 3, false)       /* marker stripped first */
           && Run("Hi", 5, false, false, "Hi", true, 2, true)                       /* all revealed: done */
           && Run("Hello", 1, false, true, "Hello", true, 5, true)                  /* cutscene: whole text */
           && Run("(HC)Hey", 1, true, false, "Hey", false, 3, true);                /* already done */
    if (!ok) { std::printf("0048de8a_TEST FAIL\n"); return 1; }
    std::printf("0048de8a_TEST PASS\n");
    return 0;
}