// Behaviour test for 0x0055F710 CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest: a copy of the row's statements over
// stand-ins of its out-of-line callees (as the landed same-TU tests 0x0055F090 / 0x0055FB80 do). The row object cannot be linked by a
// test: it calls the anonymous-namespace CryptoMatic 0x0055E2C0, whose ?A0x<hash> name depends on the compile path.
// Checked: the old connection is deleted and replaced, proxy ip / port and server / port reach the connection, a failed
// SetConnectionDetails returns 1001 before any header work, the user-name header always and the login header only for login packets,
// the encryption gets the body, its size and the key into a body+32 byte zeroed buffer, the prepared request is a POST of the
// scoreboard page with the headers, the whole body+32 buffer as the document and the encrypted length, TaskState 102, success 1000,
// and every connection / string / buffer allocated is released.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

static int g_fail = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; std::printf("check failed at line %d\n", line); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)

static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }

// --- stand-ins ------------------------------------------------------------------------------------------------------------
class CCharString { public:
    std::string S;
    CCharString(const char* s = "") : S(s) { }
    const char* GetAsCharArray() const { return S.c_str(); }
};
class CWideString { public:
    std::string S;                                       // ASCII payload is enough here
    CCharString ToCharString() const { return CCharString(S.c_str()); }
};
class CUserProfileManager { public:
    CWideString ScoreBoardUserName, ScoreBoardProxyIp;
    long ScoreBoardProxyPort, ScoreBoardServerPort;
    static CUserProfileManager& Get();
    CWideString GetScoreboardUserName() { return ScoreBoardUserName; }
    CWideString GetScoreboardProxyIp() { return ScoreBoardProxyIp; }
    long GetScoreboardProxyPort() { return ScoreBoardProxyPort; }
    long GetScoreboardServerPort() { return ScoreBoardServerPort; }
};
static CUserProfileManager g_profile;
CUserProfileManager& CUserProfileManager::Get() { return g_profile; }

static int g_liveHttp = 0, g_deletedHttp = 0, g_failConnect = 0;
namespace LHNetworkLib {
enum LH_RETURN { LH_OK = 0, LH_ERROR = 1 };
enum HTTP_REQUEST_TYPE { GET = 1, POST = 2 };
class LHHttpHeaders { public:
    std::vector<std::string> Lines;
    void AddHeader(const std::string& name, const std::string& value) { Lines.push_back(name + ": " + value); }
};
class LHHttp2 { public:
    int Id;
    std::string ProxyIp, Server, Page, Document;
    unsigned short ProxyPort, ServerPort;
    std::vector<std::string> Headers;
    unsigned long Length;
    int Type;
    LHHttp2() : Id(++g_liveHttp + 100 * g_deletedHttp), ProxyPort(0), ServerPort(0), Length(0), Type(0) { }
    ~LHHttp2() { --g_liveHttp; ++g_deletedHttp; }
    void SetProxySettings(const std::string& ip, unsigned short port) { ProxyIp = ip; ProxyPort = port; Log("proxy"); }
    LH_RETURN SetConnectionDetails(const std::string& server, unsigned short port)
    {
        Server = server; ServerPort = port; Log("connect");
        return g_failConnect ? LH_ERROR : LH_OK;
    }
    int PrepareRequest(HTTP_REQUEST_TYPE type, const std::string& page, LHHttpHeaders* headers, const std::string& doc, unsigned long length)
    {
        Type = type; Page = page; Headers = headers->Lines; Document = doc; Length = length; Log("prepare");
        return 0;
    }
};
}
struct scoped_ptr {                                      // boost::scoped_ptr<LHHttp2>
    LHNetworkLib::LHHttp2* ptr;
    void reset(LHNetworkLib::LHHttp2* p) { LHNetworkLib::LHHttp2* old = ptr; ptr = p; delete old; }
    LHNetworkLib::LHHttp2* operator->() const { return ptr; }
};

static CCharString ConsoleVarScoreboardServerIPAddress("scores.lionhead.test");   // 0x013B8AF0
static CCharString ConsoleVarServerScoresAgent("/fable/scores.php");             // 0x013B8AF4

static int g_cryptCalls = 0, g_cryptLength = -1, g_bufferZero = 0;
static std::string g_cryptKey, g_cryptSource;
static int CryptoMatic(const char* source, char* destination, int length, const char* key, char decrypt)   // 0x0055E2C0 stand-in
{
    ++g_cryptCalls; g_cryptLength = length; g_cryptKey = key; g_cryptSource.assign(source, length);
    g_bufferZero = 1;
    for (int i = 0; i < length + 32; ++i) if (destination[i] != 0) g_bufferZero = 0;
    CHECK(decrypt == 0);
    int k = (int)std::strlen(key);
    for (int i = 0; i < length; ++i) destination[i] = (char)(source[i] ^ key[i % k]);
    return length + 5;                                   // the encrypted length differs from the buffer size
}
static int EncryptBuffer(const char* source, char* destination, int length, const char* key) { return CryptoMatic(source, destination, length, key, 0); }

class CTCHeroOnlineScoreboard { public:
    enum ENetworkSuccess { ENS_SUCCESS = 1000, ENS_FAILURE = 1001, ENS_PENDING = 1002 };
    scoped_ptr PHttp;
    long TaskState;
    CCharString GetSignedInUserName() { return CUserProfileManager::Get().GetScoreboardUserName().ToCharString(); }
    ENetworkSuccess ResetAndOpenHTTPAndPrepareRequest(const std::string& body_text, const CCharString& enc_key, bool login_packet);
};

// --- the row's statements -------------------------------------------------------------------------------------------------
CTCHeroOnlineScoreboard::ENetworkSuccess CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest(const std::string& body_text, const CCharString& enc_key, bool login_packet)
{
    PHttp.reset(new LHNetworkLib::LHHttp2);
    PHttp->SetProxySettings(CUserProfileManager::Get().GetScoreboardProxyIp().ToCharString().GetAsCharArray(),
                            (unsigned short)CUserProfileManager::Get().GetScoreboardProxyPort());
    if (PHttp->SetConnectionDetails(ConsoleVarScoreboardServerIPAddress.GetAsCharArray(), (unsigned short)CUserProfileManager::Get().GetScoreboardServerPort()) != LHNetworkLib::LH_OK)
    {
        return ENS_FAILURE;
    }

    LHNetworkLib::LHHttpHeaders hdr;
    hdr.AddHeader("X-LH-Username", GetSignedInUserName().GetAsCharArray());
    if (login_packet)
    {
        hdr.AddHeader("X-LH-Login", "Login");
    }

    std::vector<char> encrypted_body_text;
    unsigned int encrypted_buffer_size = body_text.size() + 32;
    encrypted_body_text.resize(encrypted_buffer_size);
    int encrypted_data_size = EncryptBuffer(body_text.c_str(), &encrypted_body_text[0], body_text.size(), enc_key.GetAsCharArray());
    std::string encrypted_string(&encrypted_body_text[0], &encrypted_body_text[0] + encrypted_buffer_size);
    PHttp->PrepareRequest(LHNetworkLib::POST, ConsoleVarServerScoresAgent.GetAsCharArray(), &hdr, encrypted_string, encrypted_data_size);

    TaskState = 102;
    return ENS_SUCCESS;
}

// --- reference expectations -----------------------------------------------------------------------------------------------
static void Run(bool login, bool failConnect, const char* body, const char* key, long proxyPort, long serverPort)
{
    g_log[0] = 0; g_cryptCalls = 0; g_failConnect = failConnect;
    g_profile.ScoreBoardUserName.S = "Hero"; g_profile.ScoreBoardProxyIp.S = "10.0.0.7";
    g_profile.ScoreBoardProxyPort = proxyPort; g_profile.ScoreBoardServerPort = serverPort;
    CTCHeroOnlineScoreboard s; s.PHttp.ptr = new LHNetworkLib::LHHttp2; s.TaskState = 100;
    int oldId = s.PHttp.ptr->Id, deleted = g_deletedHttp;
    std::string b(body); CCharString k(key);
    int r = s.ResetAndOpenHTTPAndPrepareRequest(b, k, login);
    LHNetworkLib::LHHttp2* h = s.PHttp.ptr;
    CHECK(g_deletedHttp == deleted + 1 && h != 0 && h->Id != oldId && g_liveHttp == 1);   // old connection deleted, new one kept
    CHECK(h->ProxyIp == "10.0.0.7" && h->ProxyPort == (unsigned short)proxyPort);
    CHECK(h->Server == "scores.lionhead.test" && h->ServerPort == (unsigned short)serverPort);
    if (failConnect) {
        CHECK(r == 1001 && s.TaskState == 100 && g_cryptCalls == 0 && std::strcmp(g_log, "proxy;connect;") == 0);
    } else {
        CHECK(r == 1000 && s.TaskState == 102 && std::strcmp(g_log, "proxy;connect;prepare;") == 0);
        CHECK(h->Headers.size() == (login ? 2u : 1u) && h->Headers[0] == "X-LH-Username: Hero");
        if (login) CHECK(h->Headers.size() == 2u && h->Headers[1] == "X-LH-Login: Login");
        CHECK(g_cryptCalls == 1 && g_cryptSource == body && g_cryptLength == (int)b.size() && g_cryptKey == key && g_bufferZero);
        CHECK(h->Type == LHNetworkLib::POST && h->Page == "/fable/scores.php");
        CHECK(h->Document.size() == b.size() + 32 && h->Length == b.size() + 5);
        int kl = (int)std::strlen(key), same = 1;
        for (unsigned int i = 0; i < b.size(); ++i) if (h->Document[i] != (char)(b[i] ^ key[i % kl])) same = 0;
        for (unsigned int i = b.size(); i < b.size() + 32; ++i) if (h->Document[i] != 0) same = 0;
        CHECK(same);
    }
    s.PHttp.reset(0);
    CHECK(g_liveHttp == 0);
}

extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void*) { std::printf("0055f710_TEST FAIL crash\n"); std::fflush(stdout); ExitProcess(3); return 0; }
int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    Run(false, false, "Murders=12\r\nWealth=3400\r\n", "THIS-ISMY-VALI-DCDK-EYOH-YESI-TISM", 8080, 80);
    Run(true, false, "Session=abc", "K3Y", 3128, 8000);
    Run(true, true, "Session=abc", "K3Y", 3128, 8000);
    Run(false, true, "", "Q", 0x12345, 0x10050);              // ports truncate to unsigned short
    Run(true, false, "", "Q", 0x12345, 0x10050);              // empty body: a 32-byte zero document
    if (g_fail) { std::printf("0055f710_TEST FAIL\n"); return 1; }
    std::printf("0055f710_TEST PASS\n");
    return 0;
}