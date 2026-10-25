// Behaviour test for LHHttp2::SendRequestAsync (0x008405F0): no request -> -104; all sent -> request and response
// header state reset, 2; a failing send -> request reset and header buffer cleared, -100; nothing sent past the
// time-out -> request reset, -102; otherwise the sent count advances (at most 1024 per call), the clock restarts on
// progress, 1.
#include <cstdio>
#include <cstring>
#include <string>

static long g_now = 1000;
static int g_sendResult = 0;          // SendRawData's LH_RETURN
static unsigned long g_sendLimit = 0; // bytes the socket accepts per call

struct Http {
    std::string PreparedRequest;
    unsigned long RequestBytesSent, RequestLength;
    std::string ReceivedHeaderString;
    int ReceivedHeaderLines;
    unsigned long TimeOutValue, ActionStartTime;
    bool ActionStarted;
    unsigned short CurrentAction;
    char CurrentHeaderPart[2048];
    unsigned long CurrentHeaderPos, DocumentEstimatedLength, DocumentLength;
    unsigned long lastSize;

    int SendRawData(const char*, unsigned long size, unsigned long* sent)
    {
        lastSize = size;
        *sent = size < g_sendLimit ? size : g_sendLimit;
        return g_sendResult;
    }
    void ResetRequest()
    {
        PreparedRequest = "";
        RequestBytesSent = RequestLength = DocumentEstimatedLength = DocumentLength = 0;
    }
    int SendRequestAsync()
    {
        if (PreparedRequest == "")
            return -104;
        if (RequestBytesSent >= RequestLength) {
            ResetRequest();
            ReceivedHeaderString = "";
            ReceivedHeaderLines = 0;
            ActionStarted = false;
            CurrentAction = 1;
            return 2;
        }
        unsigned long size = RequestLength - RequestBytesSent;
        if (size >= 1024)
            size = 1024;
        unsigned long bytesSent = 0;
        if (SendRawData(&PreparedRequest[RequestBytesSent], size, &bytesSent) == 2) {
            ResetRequest();
            std::memset(CurrentHeaderPart, 0, sizeof(CurrentHeaderPart));
            CurrentHeaderPos = 0;
            return -100;
        }
        if (bytesSent == 0 && ActionStarted) {
            if ((unsigned long)g_now > ActionStartTime + TimeOutValue) {
                ResetRequest();
                return -102;
            }
        } else {
            ActionStartTime = g_now;
            ActionStarted = true;
        }
        RequestBytesSent += bytesSent;
        return 1;
    }
};

int main()
{
    int fails = 0;
    static Http h;
    h.TimeOutValue = 30;
    if (h.SendRequestAsync() != -104) ++fails;
    h.PreparedRequest = std::string(1500, 'x');
    h.RequestLength = 1500;
    g_sendLimit = 5000;
    if (h.SendRequestAsync() != 1 || h.lastSize != 1024 || h.RequestBytesSent != 1024 || !h.ActionStarted) ++fails;
    if (h.SendRequestAsync() != 1 || h.lastSize != 476 || h.RequestBytesSent != 1500) ++fails;
    h.ReceivedHeaderString = "old"; h.ReceivedHeaderLines = 3;
    if (h.SendRequestAsync() != 2 || h.PreparedRequest != "" || h.RequestLength != 0 || h.ActionStarted ||
        h.CurrentAction != 1 || h.ReceivedHeaderLines != 0 || h.ReceivedHeaderString != "") ++fails;
    // stalled socket: within the time-out -> 1, past it -> -102
    h.PreparedRequest = "GET / HTTP/1.0"; h.RequestLength = 14; h.RequestBytesSent = 0;
    g_sendLimit = 0; h.ActionStarted = true; h.ActionStartTime = 1000; g_now = 1020;
    if (h.SendRequestAsync() != 1 || h.RequestBytesSent != 0) ++fails;
    g_now = 1031;
    if (h.SendRequestAsync() != -102 || h.PreparedRequest != "") ++fails;
    // failing send
    h.PreparedRequest = "abc"; h.RequestLength = 3; h.CurrentHeaderPart[5] = 'z'; h.CurrentHeaderPos = 9;
    g_sendResult = 2;
    if (h.SendRequestAsync() != -100 || h.CurrentHeaderPart[5] != 0 || h.CurrentHeaderPos != 0 || h.RequestLength != 0) ++fails;
    if (fails) { std::printf("008405f0_TEST FAIL\n"); return 1; }
    std::printf("008405f0_TEST PASS\n");
    return 0;
}