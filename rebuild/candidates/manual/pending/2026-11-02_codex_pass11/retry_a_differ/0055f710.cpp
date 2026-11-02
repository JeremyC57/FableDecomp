// CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,CCharString const &,bool) -- prepare an encrypted scoreboard request.
// Leaf: ?ResetAndOpenHTTPAndPrepareRequest@CTCHeroOnlineScoreboard@@AAE?AW4ENetworkSuccess@1@ABV?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@ABVCCharString@@_N@Z
// Grade: DIFFER(1122v1122) -- retail loads PHttp before the proxy-port argument and zero-extends the port with xor edx,edx / mov dx (this build: movzx after the string push); scratch-register choices in the string and crypto sections
// Context: grades against the Codex row context $S/codex11_dl/x/rows/0055f710/selected_context/hdr (Codex per-feature macros; NOT rebased on rebuild/include); source = Codex selected; 0055f710 probes in probe/0055f710.log
#define FABLE_RETAIL_SCOREBOARD
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_LHHTTP2
#define FABLE_STLPORT_HTTP
#define FABLE_RETAIL_CWIDESTRING_COPY
#include "engine/CTCHeroOnlineScoreboard.h"
#include "engine/CUserProfileManager.h"
#pragma auto_inline(off)
namespace std {
template<class T,class A> __declspec(noinline) void vector<T,A>::resize(size_type n,const T& x)
{
    if(n<size()) erase(begin()+n,end()); else _M_fill_insert(end(),n-size(),x);
}
}
#pragma auto_inline(on)
extern CCharString ConsoleVarScoreboardServerIPAddress;
extern CCharString ConsoleVarServerScoresAgent;
namespace {
int __fastcall CryptoMatic(const char* data,char* enc,int dlen,const char* key,char mode);
int __fastcall EncryptBuffer(const char* data,char* enc,int data_len,const char* key)
{
    return CryptoMatic(data,enc,data_len,key,0);
}
}
inline CCharString CTCHeroOnlineScoreboard::GetSignedInUserName()
{
    return CUserProfileManager::Get().GetScoreboardUserName().ToCharString();
}
CTCHeroOnlineScoreboard::ENetworkSuccess
CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest(const std::string& body_text,const CCharString& enc_key,bool login_packet)
{
    PHttp.reset(new LHNetworkLib::LHHttp2);
    PHttp->SetProxySettings(CUserProfileManager::Get().GetScoreboardProxyIp().ToCharString().GetAsCharArray(),CUserProfileManager::Get().GetScoreboardProxyPort());
    if(PHttp->Open(ConsoleVarScoreboardServerIPAddress.GetAsCharArray(),CUserProfileManager::Get().GetScoreboardServerPort()) != LHNetworkLib::LH_OK)
        return ENS_FAILURE;
    LHNetworkLib::LHHttpHeaders hdr;
    hdr.AddHeader("X-LH-Username",GetSignedInUserName().GetAsCharArray());
    if(login_packet) hdr.AddHeader("X-LH-Login","Login");
    int encrypted_buffer_size=body_text.size()+32;
    std::vector<char> encrypted_body_text;
    encrypted_body_text.resize(encrypted_buffer_size);
    int encrypted_data_size=EncryptBuffer(body_text.c_str(),&encrypted_body_text[0],(int)body_text.size(),enc_key.GetAsCharArray());
    std::string encrypted_string(encrypted_body_text.begin(),encrypted_body_text.begin()+encrypted_buffer_size);
    PHttp->PrepareRequest(LHNetworkLib::POST,ConsoleVarServerScoresAgent.GetAsCharArray(),&hdr,encrypted_string,(unsigned long)encrypted_data_size);
    TaskState=ETS_SENDING_REQUEST_ASYNC;
    return ENS_SUCCESS;
}
