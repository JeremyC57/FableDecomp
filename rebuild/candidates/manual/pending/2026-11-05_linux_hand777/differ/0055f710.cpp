// CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest(const std::string& request, const CCharString& key, bool login) -- recreate the scoreboard HTTP connection (proxy, server), add the user-name / login headers and prepare an encrypted POST of the request
// Grade: DIFFER(1121v1122) -- dl=46 (try g, 19 grades): register choice only -- the dynamic string temporaries' `add eax,esi` / `lea edx,[esi+eax]` operand roles at the proxy / server / page sites are swapped, `lea edx` v `lea eax` for &server string and the literal ctors' `lea edx/ecx,[eax+n]`, AddHeader argument scheduling, the resize tag in ecx v edx, the key in edx v eax, and after CryptoMatic retail `mov ebp,eax ; lea eax,[tag] ; add esi,edi` v ours `lea ecx ; lea edx,[edi+esi] ; mov ebp,eax` later (one byte short)
// Leaf: ?ResetAndOpenHTTPAndPrepareRequest@CTCHeroOnlineScoreboard@@AAE?AW4ENetworkSuccess@1@ABV?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@ABVCCharString@@_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055F710, 1,122 bytes, thiscall `ret 0Ch`, EH frame (scoped_ptr reset, string / vector / header locals).
// Identity: Ego_r.pdb ?ResetAndOpenHTTPAndPrepareRequest@CTCHeroOnlineScoreboard@@AAE?AW4ENetworkSuccess@1@ABV?$basic_string@...@std@@ABVCCharString@@_N@Z
//   (private; std::string request, CCharString key, bool login); the landed callers (UploadPlayerScores 0x00561450 and four more) spell
//   it with a `QAE` / `ABULHString@@` signature (NAME_FIXES.tsv). Same TU as the landed StdEncryptString 0x0055F090 / UpdateTransaction
//   0x0055FB80.
// Callees: operator new 0x00BFEA1A, LHHttp2::LHHttp2 0x00840F50 / ~LHHttp2 0x008410D0 (boost::scoped_ptr reset + checked_delete inline),
//   CUserProfileManager::Get, the scoreboard CWideString accessors (inline, members +0x110..+0x120), CWideString::ToCharString,
//   LHHttp2::SetProxySettings 0x0083F870, SetConnectionDetails 0x0083FB20, LHHttpHeaders::AddHeader, LHHttpHeaders::Reset 0x0083FBC0,
//   vector<char>::resize (out of line, the STLport _M_fill_insert body visible), CryptoMatic 0x0055E2C0 (EncryptBuffer 0x0055E4F0
//   expanded), basic_string::_M_range_initialize<char*> 0x00567D00 / <const char*> 0x005672D0, LHHttp2::PrepareRequest.
//   Globals g_ScoreboardServer 0x013B8AF0 / g_ScoreboardPage 0x013B8AF4 (CCharString).
// Shape: implicit `const char*` -> std::string conversions build the proxy / server temporaries before the port argument (VC7.1
//   constructs conversion temporaries first; an explicit std::string(...) is evaluated right to left, port first); the user name is a
//   block-scope CCharString; a named buffer_size (request.size() + 32) feeds both resize and the encrypted string's end.
// Next idea: model the anonymous-namespace CryptoMatic / EncryptBuffer and the string range constructor with same-TU noinline bodies as
//   StdEncryptString does (retail's whole-TU view), or the LHString-based argument types the landed callers use, to move the
//   esi/eax operand roles at the three dynamic string sites.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_SCOREBOARD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR
#include "fable_string.h"
#include "engine/LHHttp2.h"
#include "boost/scoped_ptr.h"
#include "engine/CTCBase.h"
#include "engine/CUserProfileManager.h"

extern CCharString g_ScoreboardServer;                  // 0x013B8AF0
extern CCharString g_ScoreboardPage;                    // 0x013B8AF4

namespace {
int __fastcall CryptoMatic(const char* source, char* destination, int length, const char* key, char decrypt);   // 0x0055E2C0
int __fastcall EncryptBuffer(const char* source, char* destination, int length, const char* key)   // 0x0055E4F0
{
    return CryptoMatic(source, destination, length, key, 0);
}
}

class CTCHeroOnlineScoreboard : public CTCBase {
public:
    enum ENetworkSuccess { NETWORK_SUCCESS = 1000, NETWORK_BUSY = 1001, NETWORK_FAILED = 1002 };
    enum ETaskState { TASK_IDLE = 100, TASK_BEGIN = 101, TASK_SEND_REQUEST = 102, TASK_RECEIVE_HEADERS = 103, TASK_RECEIVE_DOCUMENT = 104 };
    boost::scoped_ptr<LHNetworkLib::LHHttp2> PHttp;     // +0x0c
    unsigned char _pad_10[0x54 - 0x10];
    long TaskState;                                     // +0x54
private:
    ENetworkSuccess ResetAndOpenHTTPAndPrepareRequest(const std::string& request, const CCharString& key, bool login);
};

CTCHeroOnlineScoreboard::ENetworkSuccess CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest(const std::string& request, const CCharString& key, bool login)
{
    PHttp.reset(new LHNetworkLib::LHHttp2);
    PHttp->SetProxySettings(CUserProfileManager::Get().GetScoreboardProxyIp().ToCharString().GetAsCharArray(),
                            (unsigned short)CUserProfileManager::Get().GetScoreboardProxyPort());
    if (PHttp->SetConnectionDetails(g_ScoreboardServer.GetAsCharArray(), (unsigned short)CUserProfileManager::Get().GetScoreboardServerPort()) != LHNetworkLib::LH_OK)
        return NETWORK_BUSY;
    LHNetworkLib::LHHttpHeaders headers;
    {
        CCharString user_name = CUserProfileManager::Get().GetScoreboardUserName().ToCharString();
        headers.AddHeader("X-LH-Username", user_name.GetAsCharArray());
    }
    if (login)
        headers.AddHeader("X-LH-Login", "Login");
    unsigned int buffer_size = request.size() + 32;
    std::vector<char> buffer;
    buffer.resize(buffer_size);
    int length = EncryptBuffer(request.c_str(), &buffer[0], request.size(), key.GetAsCharArray());
    std::string encrypted(&buffer[0], &buffer[0] + buffer_size);
    PHttp->PrepareRequest(LHNetworkLib::POST, g_ScoreboardPage.GetAsCharArray(), &headers, encrypted, length);
    TaskState = TASK_SEND_REQUEST;
    return NETWORK_SUCCESS;
}
