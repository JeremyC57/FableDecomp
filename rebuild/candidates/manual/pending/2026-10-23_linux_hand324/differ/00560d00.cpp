// CTCHeroOnlineScoreboard::ProcessLogin(CCharString& errorText) -- when idle, sends LOG_ON_TO_REG_SERVER (user name and MD5 of lower-case name + password), with the MD5 of the lower-case name encrypted under the shared key appended; then steps the transaction and, on success, decrypts the reply: a zero status decrypts the session id, any other status stores "0", and the negative login statuses set the GUI error text and report busy
// Leaf: ProcessLogin
// Retail: Fable.exe @ 0x00560D00, 1866 bytes, `ret 4`, jump table at 0x00561420 (status -9..-1).
// Callees: CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0, CWideString(const CWideString&) 0x0099B720 / ~ 0x0099B510,
// ConvertToCharString 0x0099B2C0, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0 / operator=(const char*)
// 0x0099EFE0, ToLower 0x0099ED10, operator+ 0x0099F570 / 0x0099F600 (__fastcall), MD5 0x0055E5F0 (__fastcall),
// std::string range init 0x005672D0 / 0x00567D00, std::string(const char*) 0x00568440, operator+(string, string) 0x00569690
// (cdecl), string assign 0x00569140, Encrypt 0x0055F090 / Decrypt 0x0055F140 (__fastcall, hidden result in ecx),
// malloc 0x00BFEA0E, memmove 0x00BFEAE6, free 0x00BFEA14, ResetAndOpenHTTPAndPrepareRequest 0x0055F710, UpdateTransaction
// 0x0055FB80, GetDecryptedDocument 0x00560BB0, GFCharStringToInt 0x0099E7F0, ~vector<string> 0x00569940,
// LHHttp2::~LHHttp2 0x008410D0, operator delete 0x00BFE9BC; the empty CCharString 0x013B8AF8.
// Layout: as the landed DownloadMemberListForSelectedClan 0x00561DC0 (PHttp +0x0c, SessionID +0x34, TaskState +0x54).
// Grade: DIFFER(1844v1860) -- first draft: this in ebp (retail edi, late ebp push before UpdateTransaction), CCharString-to-std::string strlen path order, temps/frame 0x98 v 0x9c
typedef unsigned short wchar16;
extern "C" void __cdecl free(void* block);
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);
extern "C" unsigned int __cdecl strlen(const char* text);
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strlen)

struct CWideString;
struct CCharStringData { char* m_pString; };
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                   // 0x0099E4B0
    CCharString(const char* text, long length = -1); // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
    CCharString& operator=(const char* text);        // 0x0099EFE0
    CCharString ToLower() const;                     // 0x0099ED10
    const char* c_str() const { return PStringData == 0 ? "" : PStringData->m_pString; }
};
extern const CCharString g_emptyCharString;          // 0x013B8AF8
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
long __fastcall GFCharStringToInt(const CCharString& text);                            // 0x0099E7F0
CCharString __fastcall MD5(const CCharString& text);                                   // 0x0055E5F0
struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);           // 0x0099B720
    ~CWideString();                                  // 0x0099B510
    CCharString ConvertToCharString() const;         // 0x0099B2C0
};
struct CUserProfileManager {
    unsigned char _pad[0x110];
    CWideString   SignedInUserName;                  // +0x110
    CWideString   SignedInPassword;                  // +0x114
};
CUserProfileManager* GetUserProfileManager();        // 0x0040D2A0 (CTBaseSingleton<CUserProfileManager>::Get)

struct CStringAllocator { CStringAllocator() { } };
struct forward_iterator_tag { forward_iterator_tag() { } };
struct LHString {                                    // PDB std::basic_string<char> (12 bytes, malloc-backed)
    char* m_pStart;
    char* m_pFinish;
    char* m_pEndOfStorage;
    explicit LHString(const CCharString& text) : m_pStart(0), m_pFinish(0), m_pEndOfStorage(0)
    {
        const char* s = text.c_str();
        Initialise(s, s + strlen(s), CStringAllocator());
    }
    LHString(const char* text, const CStringAllocator& a);   // 0x00568440
    LHString(const char* first, const char* last, const forward_iterator_tag&)
    {
        unsigned int n = (unsigned int)(last - first);
        m_pStart = (char*)malloc(n + 1);
        m_pEndOfStorage = m_pStart + n + 1;
        m_pFinish = (char*)memmove(m_pStart, first, n) + n;
        *m_pFinish = 0;
    }
    LHString(const LHString& other)
    {
        forward_iterator_tag tag;
        char* first = other.m_pStart;
        char* last = other.m_pFinish;
        m_pStart = 0;
        m_pFinish = 0;
        m_pEndOfStorage = 0;
        InitialiseRange(first, last, tag);
    }
    ~LHString() { if (m_pStart) free(m_pStart); }
    LHString& operator=(const LHString& other)
    {
        if (&other != this)
            AssignRange(other.m_pStart, other.m_pFinish, forward_iterator_tag());
        return *this;
    }
    const char* c_str() const { return m_pStart; }
    void Initialise(const char* first, const char* last, const CStringAllocator& a);   // 0x005672D0
    void InitialiseRange(char* first, char* last, const forward_iterator_tag& tag);    // 0x00567D00
    void AssignRange(char* first, char* last, const forward_iterator_tag& tag);        // 0x00569140
};
LHString operator+(const LHString& left, const LHString& right);   // 0x00569690
LHString __fastcall Encrypt(const LHString& key, const LHString& text);   // 0x0055F090
LHString __fastcall Decrypt(const LHString& text, const LHString& key);   // 0x0055F140

struct LHStringVector {                              // vector<std::string>
    LHString* m_pStart;
    LHString* m_pFinish;
    LHString* m_pEndOfStorage;
    LHStringVector() : m_pStart(0), m_pFinish(0), m_pEndOfStorage(0) { }
    ~LHStringVector();                               // 0x00569940
    LHString& operator[](int i) { return m_pStart[i]; }
};

namespace LHNetworkLib {
struct LHHttp2 {
    ~LHHttp2();                                      // 0x008410D0
};
}
struct CHttpScopedPtr {
    LHNetworkLib::LHHttp2* px;
    void reset(LHNetworkLib::LHHttp2* p = 0)
    {
        LHNetworkLib::LHHttp2* old = px;
        px = p;
        delete old;
    }
};

class CTCHeroOnlineScoreboard {
public:
    enum ENetworkSuccess { NETWORK_SUCCESS = 1000, NETWORK_BUSY = 1001, NETWORK_FAILED = 1002 };
    void*          __vftable;                        // +0x00
    void*          Thing;                            // +0x04
    long           m_8;                              // +0x08
    CHttpScopedPtr PHttp;                            // +0x0c
    unsigned char  _pad10[0x34 - 0x10];
    CCharString    SessionID;                        // +0x34
    unsigned char  _pad38[0x54 - 0x38];
    long           TaskState;                        // +0x54
    ENetworkSuccess ResetAndOpenHTTPAndPrepareRequest(const LHString& request, const CCharString& key, bool flag);   // 0x0055F710
    ENetworkSuccess UpdateTransaction();             // 0x0055FB80
    int GetDecryptedDocument(LHStringVector& lines, const CCharString& key);   // 0x00560BB0
    CCharString GetSignedInUserName() { CWideString name = GetUserProfileManager()->SignedInUserName; return name.ConvertToCharString(); }
    CCharString GetSignedInPassword() { CWideString name = GetUserProfileManager()->SignedInPassword; return name.ConvertToCharString(); }
    static CCharString GetHeader() { return CCharString("Fable1.5\r\nVersion 1\r\n", -1); }
    ENetworkSuccess ProcessLogin(CCharString& errorText);
};

#pragma auto_inline(off)
__declspec(noinline) int CTCHeroOnlineScoreboard::GetDecryptedDocument(LHStringVector& lines, const CCharString& key)
{
    if (key.PStringData == 0)
        lines.m_pFinish = lines.m_pStart;
    return (int)(lines.m_pFinish - lines.m_pStart);
}
__declspec(noinline) LHStringVector::~LHStringVector()
{
    for (LHString* p = m_pStart; p != m_pFinish; ++p)
        p->~LHString();
    if (m_pStart)
        free(m_pStart);
}
__declspec(noinline) void LHString::Initialise(const char* first, const char* last, const CStringAllocator&)
{
    m_pStart = (char*)first;
    m_pFinish = (char*)last;
    m_pEndOfStorage = (char*)last;
}
#pragma auto_inline(on)

CTCHeroOnlineScoreboard::ENetworkSuccess CTCHeroOnlineScoreboard::ProcessLogin(CCharString& errorText)
{
    if (TaskState == 100 || TaskState == 101) {
        LHString text(GetHeader() + "LOG_ON_TO_REG_SERVER\r\n" + GetSignedInUserName() + "\r\n" + MD5(GetSignedInUserName().ToLower() + GetSignedInPassword().ToLower()) + "\r\n");
        {
            CCharString name = GetSignedInUserName();
            CCharString lower = name.ToLower();
            CCharString hash = MD5(lower);
            LHString hashText(hash);
            CCharString keyText("THIS-ISMY-VALI-DCDK-EYOH-YESI-TISM", -1);
            LHString key(keyText);
            const char* crlfText = "\r\n";
            LHString crlf(crlfText, crlfText + 2, forward_iterator_tag());
            text = text + Encrypt(key, hashText) + crlf;
        }
        if (ResetAndOpenHTTPAndPrepareRequest(text, g_emptyCharString, true) == NETWORK_BUSY) {
            errorText = "TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK";
            return NETWORK_BUSY;
        }
    }
    ENetworkSuccess result = UpdateTransaction();
    if (result == NETWORK_SUCCESS) {
        LHStringVector lines;
        if (GetDecryptedDocument(lines, g_emptyCharString) > 0) {
            CCharString reply(lines[0].c_str());
            CCharString statusText(lines[1].c_str());
            long status = GFCharStringToInt(statusText);
            LHString session(lines[2]);
            if (status == 0) {
                LHString id = Decrypt(session, LHString(CCharString("THIS-ISMY-VALI-DCDK-EYOH-YESI-TISM", -1).c_str(), CStringAllocator()));
                SessionID = id.c_str();
            }
            else
                SessionID = "0";
            switch (status) {
            case -9:
            case -2:
            case -1:
                errorText = "TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK";
                return NETWORK_BUSY;
            case -5:
            case -4:
            case -3:
                errorText = "TEXT_GUI_SCOREBOARD_ERROR_LOGIN_FAILURE";
                return NETWORK_BUSY;
            case -7:
                errorText = "TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK";
                return NETWORK_BUSY;
            case -8:
                errorText = "TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK";
                return NETWORK_BUSY;
            }
        }
        PHttp.reset();
        return result;
    }
    errorText = "TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK";
    return result;
}
