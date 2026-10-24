// Behaviour test for 0x0083F980 LHNetworkLib::LHHttp2::ResetRequest
#include <stdio.h>
#include <string.h>
struct CRetailString {
    char buf[32];
    CRetailString& Assign(const char* first, const char* last)
    {
        unsigned int n = (unsigned int)(last - first);
        memcpy(buf, first, n); buf[n] = 0; return *this;
    }
    CRetailString& operator=(const char* text) { return Assign(text, text + strlen(text)); }
};
struct LHHttp2 {
    CRetailString PreparedRequest;
    unsigned long RequestBytesSent;
    unsigned long RequestLength;
    unsigned long DocumentEncoding;
    unsigned long DocumentEstimatedLength;
    unsigned long DocumentLength;
    void ResetRequest();
};
void LHHttp2::ResetRequest()
{
    PreparedRequest = "";
    RequestBytesSent = 0;
    RequestLength = 0;
    DocumentEstimatedLength = 0;
    DocumentLength = 0;
}
int main()
{
    int ok = 1;
    LHHttp2 h;
    h.PreparedRequest = "GET / HTTP/1.0";
    h.RequestBytesSent = 3; h.RequestLength = 14; h.DocumentEncoding = 9; h.DocumentEstimatedLength = 100; h.DocumentLength = 50;
    h.ResetRequest();
    if (h.PreparedRequest.buf[0] != 0) ok = 0;
    if (h.RequestBytesSent || h.RequestLength || h.DocumentEstimatedLength || h.DocumentLength) ok = 0;
    if (h.DocumentEncoding != 9) ok = 0;
    printf(ok ? "0083f980_TEST PASS\n" : "0083f980_TEST FAIL\n");
    return ok ? 0 : 1;
}