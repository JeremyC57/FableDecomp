// Behaviour test for CTCHeroOnlineScoreboard::UpdateTransaction (retail 0x0055FB80): model copy + stub callees.
#include <cstdio>

static int g_responses = 0;
namespace LHNetworkLib {
struct CHttpResponse {
    long StatusCode;
    CHttpResponse() : StatusCode(0) { ++g_responses; }
    ~CHttpResponse() { --g_responses; }
};
class LHHttp2 { public:
    long send, headers, code, document; int parsed;
    long SendRequest() { return send; }
    long ReceiveResponseHeaders() { return headers; }
    long ParseResponseLine(CHttpResponse* r) { r->StatusCode = code; ++parsed; return 0; }
    long ReceiveDocumentAsync() { return document; }
};
}
class CTCHeroOnlineScoreboard { public:
    enum ENetworkSuccess { NETWORK_SUCCESS = 1000, NETWORK_BUSY = 1001, NETWORK_FAILED = 1002 };
    enum ETaskState { TASK_IDLE = 100, TASK_BEGIN = 101, TASK_SEND_REQUEST = 102, TASK_RECEIVE_HEADERS = 103, TASK_RECEIVE_DOCUMENT = 104 };
    LHNetworkLib::LHHttp2* PHttp;
    long TaskState;
    ENetworkSuccess UpdateTransaction();
};
CTCHeroOnlineScoreboard::ENetworkSuccess CTCHeroOnlineScoreboard::UpdateTransaction()
{
    long status;
    switch (TaskState) {
    case TASK_IDLE:
        return NETWORK_BUSY;
    case TASK_SEND_REQUEST: {
        LHNetworkLib::CHttpResponse response;
        status = PHttp->SendRequest();
        if (status == 2) {
            TaskState = TASK_RECEIVE_HEADERS;
        } else {
            if (status == -0x68 || status == -0x64)
                return NETWORK_BUSY;
            return NETWORK_FAILED;
        }
    }
    case TASK_RECEIVE_HEADERS: {
        LHNetworkLib::CHttpResponse response;
        status = PHttp->ReceiveResponseHeaders();
        if (status == -0x65 || status == -0x66)
            return NETWORK_BUSY;
        if (status != 5)
            return NETWORK_FAILED;
        PHttp->ParseResponseLine(&response);
        if (response.StatusCode != 200)
            return NETWORK_BUSY;
        TaskState = TASK_RECEIVE_DOCUMENT;
    }
    case TASK_RECEIVE_DOCUMENT:
        status = PHttp->ReceiveDocumentAsync();
        if (status == 7) {
            TaskState = TASK_IDLE;
            return NETWORK_SUCCESS;
        }
        if (status == -0x65 || status == -0x66)
            return NETWORK_BUSY;
        return NETWORK_FAILED;
    }
    return NETWORK_BUSY;
}
int main()
{
    int fail = 0;
    LHNetworkLib::LHHttp2 http = { 2, 5, 200, 7, 0 };
    CTCHeroOnlineScoreboard s; s.PHttp = &http; s.TaskState = 102;
    if (s.UpdateTransaction() != 1000 || s.TaskState != 100 || http.parsed != 1 || g_responses != 0) fail = 1;   // whole chain in one call
    if (s.UpdateTransaction() != 1001 || s.TaskState != 100) fail = 2;      // idle
    s.TaskState = 101; if (s.UpdateTransaction() != 1001) fail = 3;           // default
    http.send = -0x68; s.TaskState = 102; if (s.UpdateTransaction() != 1001 || s.TaskState != 102) fail = 4;
    http.send = 9; if (s.UpdateTransaction() != 1002) fail = 5;
    http.send = 2; http.headers = -0x66; if (s.UpdateTransaction() != 1001 || s.TaskState != 103) fail = 6;
    http.headers = 4; if (s.UpdateTransaction() != 1002) fail = 7;
    http.headers = 5; http.code = 404; if (s.UpdateTransaction() != 1001 || s.TaskState != 103) fail = 8;
    http.code = 200; http.document = -0x65; if (s.UpdateTransaction() != 1001 || s.TaskState != 104) fail = 9;
    http.document = 3; if (s.UpdateTransaction() != 1002 || s.TaskState != 104) fail = 10;
    if (g_responses != 0) fail = 11;
    if (fail) { std::printf("0055fb80_TEST FAIL\n"); return 1; }
    std::printf("0055fb80_TEST PASS\n");
    return 0;
}