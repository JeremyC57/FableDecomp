// Standalone fixture with the reconstructed function body and deterministic engine substitutes.


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string>
#include <vector>
struct CCharString {
    std::string value;
    CCharString() {}
    CCharString(const char* s): value(s) {}
    const char* GetAsCharArray() const { return value.c_str(); }
    operator const char*() const { return value.c_str(); }
    CCharString GetAsLowerCase() const {
        CCharString result=*this;
        for (unsigned i=0; i<result.value.size(); ++i)
            result.value[i]=(char)tolower((unsigned char)result.value[i]);
        return result;
    }
};
CCharString operator+(const CCharString& a, const CCharString& b) { return (a.value+b.value).c_str(); }
CCharString operator+(const CCharString& a, const char* b) { return (a.value+b).c_str(); }
long GFCharStringToInt(const CCharString& text) { return atol(text); }
CCharString EASY_KEY="fixture key";
CCharString MD5(const CCharString& s) { return ("hash("+s.value+")").c_str(); }
std::string StdEncryptString(const std::string& data,const std::string& key) { return "encrypted("+data+","+key+")"; }
std::string StdDecryptString(const std::string& data,const std::string& key) { return "decrypted("+data+","+key+")"; }
struct CTCHeroOnlineScoreboard {
    enum ETaskState { ETS_UNSTARTED=100, ETS_PREPARING=101 };
    enum ENetworkSuccess { ENS_SUCCESS=1000, ENS_FAILURE=1001, ENS_PENDING=1002 };
    struct HttpState { int resets; HttpState():resets(0) {} void reset() { ++resets; } } PHttp;
    long TaskState;
    CCharString SessionID;
    ENetworkSuccess setupResult, updateResult;
    std::vector<std::string> reply;
    std::string sent;
    int updates, documentReads;
    CTCHeroOnlineScoreboard(): TaskState(100), SessionID("old"), setupResult(ENS_SUCCESS),
        updateResult(ENS_SUCCESS), updates(0), documentReads(0) {}
    CCharString GetGameIdent() { return "Fable1.5\r\nVersion 1\r\n"; }
    CCharString GetCDKey() { return "test key"; }
    CCharString GetSignedInUserName() { return "PLAYER"; }
    CCharString GetSignedInPassword() { return "SeCRet"; }
    ENetworkSuccess ResetAndOpenHTTPAndPrepareRequest(const std::string& s, const CCharString& key, bool flag) {
        assert(flag && key.value==EASY_KEY.value);
        sent=s;
        return setupResult;
    }
    ENetworkSuccess UpdateTransaction() { ++updates; return updateResult; }
    int GetDecryptedDocument(std::vector<std::string>& lines, const CCharString& key) {
        assert(key.value==EASY_KEY.value);
        ++documentReads;
        lines=reply;
        return (int)lines.size();
    }
    ENetworkSuccess LogOnToRegistrationServer(CCharString&);
};

CTCHeroOnlineScoreboard::ENetworkSuccess CTCHeroOnlineScoreboard::LogOnToRegistrationServer(CCharString& error_message_tag)
{
    if (TaskState == ETS_UNSTARTED || TaskState == ETS_PREPARING) {
        std::string req((GetGameIdent() + "LOG_ON_TO_REG_SERVER\r\n" + GetSignedInUserName() + "\r\n" + MD5(GetSignedInUserName().GetAsLowerCase() + GetSignedInPassword().GetAsLowerCase()) + "\r\n").GetAsCharArray());
        req = req + StdEncryptString(GetCDKey().GetAsCharArray(), MD5(GetSignedInPassword().GetAsLowerCase()).GetAsCharArray()) + std::string("\r\n");
        if (ResetAndOpenHTTPAndPrepareRequest(req, EASY_KEY, true) == ENS_FAILURE) {
            error_message_tag = "TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK";
            return ENS_FAILURE;
        }
    }
    ENetworkSuccess result = UpdateTransaction();
    if (result == ENS_SUCCESS) {
        std::vector<std::string> lines;
        int count = GetDecryptedDocument(lines, EASY_KEY);
        if (count > 0) {
            CCharString user_id_str(lines[0].c_str());
            CCharString return_code_str(lines[1].c_str());
            long return_code = GFCharStringToInt(return_code_str);
            std::string sessionid_encrypted(lines[2]);
            if (return_code == 0) {
                std::string sessionid = StdDecryptString(sessionid_encrypted, GetCDKey().GetAsCharArray());
                SessionID = sessionid.c_str();
            }
            else
                SessionID = "0";
            switch (return_code) {
            case -9:
            case -2:
            case -1:
                error_message_tag = "TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK";
                return ENS_FAILURE;
            case -5:
            case -4:
            case -3:
                error_message_tag = "TEXT_GUI_SCOREBOARD_ERROR_LOGIN_FAILURE";
                return ENS_FAILURE;
            case -7:
                error_message_tag = "TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK";
                return ENS_FAILURE;
            case -8:
                error_message_tag = "TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK";
                return ENS_FAILURE;
            }
        }
        PHttp.reset();
    } else {
        error_message_tag = "TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK";
    }
    return result;
}

int main()
{
    typedef CTCHeroOnlineScoreboard Scoreboard;
    const char* network="TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK";
    const char* login="TEXT_GUI_SCOREBOARD_ERROR_LOGIN_FAILURE";
    Scoreboard rejected;
    CCharString error;
    rejected.setupResult=Scoreboard::ENS_FAILURE;
    assert(rejected.LogOnToRegistrationServer(error)==Scoreboard::ENS_FAILURE);
    assert(error.value==network && rejected.updates==0 && rejected.PHttp.resets==0);
    assert(rejected.sent=="Fable1.5\r\nVersion 1\r\nLOG_ON_TO_REG_SERVER\r\nPLAYER\r\nhash(playersecret)\r\nencrypted(test key,hash(secret))\r\n");
    Scoreboard pending;
    pending.TaskState=101; pending.updateResult=Scoreboard::ENS_PENDING;
    assert(pending.LogOnToRegistrationServer(error)==Scoreboard::ENS_PENDING);
    assert(error.value==network && !pending.sent.empty() && pending.documentReads==0);
    Scoreboard noDocument; noDocument.TaskState=102;
    assert(noDocument.LogOnToRegistrationServer(error)==Scoreboard::ENS_SUCCESS);
    assert(noDocument.sent.empty() && noDocument.PHttp.resets==1 && noDocument.SessionID.value=="old");
    for (int status=-10; status<=1; ++status) {
        Scoreboard scoreboard;
        error="";
        scoreboard.TaskState=102;
        scoreboard.reply.push_back("registration");
        char statusText[16]; sprintf(statusText,"%d",status);
        scoreboard.reply.push_back(statusText);
        scoreboard.reply.push_back("ticket");
        const bool failure=status>=-9 && status<=-1 && status!=-6;
        assert(scoreboard.LogOnToRegistrationServer(error)==(failure ? Scoreboard::ENS_FAILURE : Scoreboard::ENS_SUCCESS));
        assert(scoreboard.PHttp.resets==(failure ? 0 : 1));
        assert(scoreboard.SessionID.value==(status==0 ? "decrypted(ticket,test key)" : "0"));
        if (failure) assert(error.value==(status>=-5 && status<=-3 ? login : network));
        else assert(error.value.empty());
    }
    puts("00560d00_TEST PASS");
    return 0;
}