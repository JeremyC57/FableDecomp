// CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest(const std::string& body_text, const CCharString& enc_key, bool login_packet) -- recreate the scoreboard HTTP connection (proxy, server; failure 1001 if the server cannot be set), add the user-name and (login packets) login headers, encrypt the body into a body+32 byte buffer and prepare it as a POST to the scoreboard page, then wait for the send (TaskState 102) and return success 1000
// Inline-budget: FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE (shared header contract) -- retail calls the vector<char>::resize COMDAT 0x004122D0 (0x0055FA5E) instead of expanding it; without it DIFFER(1122v1122), 7 hunks.
// Leaf: ?ResetAndOpenHTTPAndPrepareRequest@CTCHeroOnlineScoreboard@@AAE?AW4ENetworkSuccess@1@ABV?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@ABVCCharString@@_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055F710, 1,122 bytes, thiscall `ret 0Ch`, no EH frame (the cleanups are inline).
// Identity: Ego_r.pdb ?ResetAndOpenHTTPAndPrepareRequest@CTCHeroOnlineScoreboard@@AAE?AW4ENetworkSuccess@1@ABV?$basic_string@...@std@@ABVCCharString@@_N@Z
//   (private), declared in the shared engine/CTCHeroOnlineScoreboard.h under FABLE_RETAIL_CTCHEROONLINESCOREBOARD_NETWORK. Parameter and
//   local names from Ego_d.pdb (body_text, enc_key, login_packet; hdr, encrypted_buffer_size, encrypted_body_text, encrypted_data_size,
//   encrypted_string). Return values are the header's PDB enumerators ENS_SUCCESS 1000 / ENS_FAILURE 1001. TaskState's PDB type is the
//   class's ETaskState, whose enumerator names are in no PDB, so 102 stays a literal as in the landed siblings (TaskState 100).
// Order: Ego_d's line table puts the vector declaration (line 615, 12 B) before `encrypted_buffer_size = body_text.size() + 32` (616,
//   14 B: size call, add 20h, store); that order is also what retail's register choice needs (the other order: DIFFER(1121v1122)).
// Callees: operator new 0x00BFEA1A / delete 0x00BFE9BC, LHHttp2::LHHttp2 0x00840F50 / ~LHHttp2 0x008410D0 (scoped_ptr reset inline),
//   CUserProfileManager::Get 0x0040D2A0 (scoreboard accessors inline), CWideString copy 0x0099B720 / ToCharString 0x0099B2C0 / dtor
//   0x0099B510, ~CCharString 0x0099EAE0, LHHttp2::SetProxySettings 0x0083F870 / SetConnectionDetails 0x0083FB20 / PrepareRequest 0x008400B0,
//   LHHttpHeaders::AddHeader(name,value) 0x0083FC00 / Reset 0x0083FBC0, vector<char>::resize 0x004122D0, CryptoMatic 0x0055E2C0
//   (EncryptBuffer 0x0055E4F0 expanded), basic_string::_M_range_initialize<const char*> 0x005672D0 / <char*> 0x00567D00.
//   Globals ConsoleVarScoreboardServerIPAddress 0x013B8AF0 / ConsoleVarServerScoresAgent 0x013B8AF4 (CCharString; Ego_r + FableWin + Anniv360 names, as the landed dynamic initializers 0x0121A750 / 0x0121A770).
// Callers: the five landed same-TU callers declare it with a local LHString stand-in (`const LHString&`, `bool flag`): NAME_FIXES.tsv.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_PASS22_CCHARSTRING_GETASCHARARRAY_ORIGINAL_IFELSE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_NETWORK
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_SCOREBOARD
#include "engine/CTCHeroOnlineScoreboard.h"

extern CCharString ConsoleVarScoreboardServerIPAddress;                  // 0x013B8AF0
extern CCharString ConsoleVarServerScoresAgent;                    // 0x013B8AF4

namespace {
int __fastcall CryptoMatic(const char* source, char* destination, int length, const char* key, char decrypt);   // 0x0055E2C0
int __fastcall EncryptBuffer(const char* source, char* destination, int length, const char* key)   // 0x0055E4F0
{
    return CryptoMatic(source, destination, length, key, 0);
}
}

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