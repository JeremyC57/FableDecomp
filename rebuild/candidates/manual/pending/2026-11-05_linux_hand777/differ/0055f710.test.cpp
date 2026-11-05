// Behaviour test for CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest (0x0055F710): a model of the row -- the HTTP object
// is recreated, the proxy and server come from the user profile, a refused connection returns NETWORK_BUSY before any header is
// built, otherwise the user-name header (and the login header when asked) is added, the request is encrypted into a buffer of
// size()+32 and posted to the scoreboard page, and the task moves to TASK_SEND_REQUEST.
#include <stdio.h>
#include <string>
#include <vector>
#include <string.h>
struct Http {
    std::string Proxy, Server, Page, Body; unsigned short ProxyPort, ServerPort; bool Refuse;
    std::vector<std::string> Headers;
    Http() : ProxyPort(0), ServerPort(0), Refuse(false) { }
};
static int Encrypt(const char* source, char* destination, int length, const char* key)
{
    int k = 0, n = (int)strlen(key);
    for (int i = 0; i < length; ++i) destination[i] = (char)(source[i] ^ key[k++ % n]);
    return length;
}
struct Scoreboard {
    enum { NETWORK_SUCCESS = 1000, NETWORK_BUSY = 1001 };
    enum { TASK_IDLE = 100, TASK_SEND_REQUEST = 102 };
    Http* PHttp; long TaskState; bool RefuseNext; int Created;
    Scoreboard() : PHttp(0), TaskState(TASK_IDLE), RefuseNext(false), Created(0) { }
    ~Scoreboard() { delete PHttp; }
    int ResetAndOpenHTTPAndPrepareRequest(const std::string& request, const std::string& key, bool login)
    {
        delete PHttp; PHttp = new Http; ++Created;
        PHttp->Proxy = "proxy"; PHttp->ProxyPort = 8080;
        if (RefuseNext) return NETWORK_BUSY;
        PHttp->Server = "scores"; PHttp->ServerPort = 80;
        PHttp->Headers.push_back("X-LH-Username: hero");
        if (login) PHttp->Headers.push_back("X-LH-Login: Login");
        unsigned int buffer_size = request.size() + 32;
        std::vector<char> buffer(buffer_size);
        Encrypt(request.c_str(), &buffer[0], (int)request.size(), key.c_str());
        PHttp->Body = std::string(&buffer[0], &buffer[0] + buffer_size);
        PHttp->Page = "page";
        TaskState = TASK_SEND_REQUEST;
        return NETWORK_SUCCESS;
    }
};
int main()
{
    Scoreboard s;
    int ok = s.ResetAndOpenHTTPAndPrepareRequest("abc", "k", true) == Scoreboard::NETWORK_SUCCESS && s.PHttp->Headers.size() == 2 &&
             s.PHttp->Body.size() == 35 && s.PHttp->Body[0] == ('a' ^ 'k') && s.TaskState == Scoreboard::TASK_SEND_REQUEST;
    s.TaskState = Scoreboard::TASK_IDLE; s.RefuseNext = true;
    ok = ok && s.ResetAndOpenHTTPAndPrepareRequest("abc", "k", false) == Scoreboard::NETWORK_BUSY && s.PHttp->Headers.empty() &&
         s.TaskState == Scoreboard::TASK_IDLE && s.Created == 2;
    printf(ok ? "0055f710_TEST PASS\n" : "0055f710_TEST FAIL\n");
    return ok ? 0 : 1;
}
