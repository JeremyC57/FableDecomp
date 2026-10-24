// Behaviour test for LHHttp2::Reset (0x0083FF80): socket destroyed and nulled, strings emptied, headers cleared,
// document parts reset, transfer state zeroed; Port / RequestLength / TimeOutValue / CurrentHeaderPart untouched.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
static int g_socketsDestroyed = 0;
static int g_partsReset = 0;
struct LSocket { ~LSocket() { ++g_socketsDestroyed; } };
struct LHHttpHeaders {
    std::vector<std::string> HdrDocumentHeader;
    std::string HeaderString;
    void Reset() { HeaderString = ""; HdrDocumentHeader.clear(); }
};
struct LHHttp2 {
    LSocket* Host;
    std::string Hostname;
    unsigned short Port;
    std::string PreparedRequest;
    unsigned int RequestBytesSent, RequestLength;
    LHHttpHeaders MyHeaders, ReceivedHeaders;
    unsigned int TimeOutValue, ActionStartTime;
    bool ActionStarted;
    unsigned short CurrentAction;
    unsigned int CurrDownSize;
    char CurrentHeaderPart[2048];
    unsigned int CurrentHeaderPos;
    std::string ProxyHost;
    unsigned int ProxyPort;
    unsigned int DocumentEstimatedLength, DocumentLength;
    char CurrentDocumentPart[2048];
    unsigned int CurrentDocumentPos;
    bool InChunkMode;
    void ResetDocumentParts() { ++g_partsReset; }
    void Reset()
    {
        if (Host) {
            delete Host;
            Host = 0;
        }
        PreparedRequest = "";
        MyHeaders.Reset();
        ReceivedHeaders.Reset();
        ResetDocumentParts();
        RequestBytesSent = 0;
        Hostname = "";
        ActionStartTime = 0;
        CurrDownSize = 0;
        CurrentAction = 0;
        ActionStarted = false;
        CurrentHeaderPos = 0;
        DocumentLength = 0;
        DocumentEstimatedLength = 0;
        std::memset(CurrentDocumentPart, 0, sizeof(CurrentDocumentPart));
        CurrentDocumentPos = 0;
        InChunkMode = false;
        ProxyHost = "";
        ProxyPort = 0;
    }
};
int main()
{
    static LHHttp2 h;
    h.Host = new LSocket;
    h.Hostname = "host"; h.Port = 80; h.PreparedRequest = "GET /"; h.RequestBytesSent = 3; h.RequestLength = 9;
    h.MyHeaders.HdrDocumentHeader.push_back("A: b"); h.MyHeaders.HeaderString = "x";
    h.ReceivedHeaders.HdrDocumentHeader.push_back("C: d");
    h.TimeOutValue = 7; h.ActionStartTime = 5; h.ActionStarted = true; h.CurrentAction = 2; h.CurrDownSize = 11;
    h.CurrentHeaderPart[0] = 'H'; h.CurrentHeaderPos = 4; h.ProxyHost = "proxy"; h.ProxyPort = 8080;
    h.DocumentEstimatedLength = 100; h.DocumentLength = 50; h.CurrentDocumentPart[10] = 'D'; h.CurrentDocumentPos = 9;
    h.InChunkMode = true;
    h.Reset();
    bool ok = h.Host == 0 && g_socketsDestroyed == 1 && g_partsReset == 1 && h.Hostname.empty() && h.Port == 80
        && h.PreparedRequest.empty() && h.RequestBytesSent == 0 && h.RequestLength == 9
        && h.MyHeaders.HdrDocumentHeader.empty() && h.MyHeaders.HeaderString.empty() && h.ReceivedHeaders.HdrDocumentHeader.empty()
        && h.TimeOutValue == 7 && h.ActionStartTime == 0 && !h.ActionStarted && h.CurrentAction == 0 && h.CurrDownSize == 0
        && h.CurrentHeaderPart[0] == 'H' && h.CurrentHeaderPos == 0 && h.ProxyHost.empty() && h.ProxyPort == 0
        && h.DocumentEstimatedLength == 0 && h.DocumentLength == 0 && h.CurrentDocumentPart[10] == 0
        && h.CurrentDocumentPos == 0 && !h.InChunkMode;
    h.Reset();                                       // no socket: nothing destroyed
    ok = ok && g_socketsDestroyed == 1 && g_partsReset == 2;
    std::printf(ok ? "0083ff80_TEST PASS\n" : "0083ff80_TEST FAIL\n");
    return ok ? 0 : 1;
}