// Behaviour test for LHNetworkLib::LHHttp2::LHHttp2 (0x00840F50): empty strings, null vectors and socket, the 60 s time-out,
// 10 forwardings and a cleared document buffer.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>

struct String {
    char* start; char* finish; char* end;
    String() : start(0), finish(0), end(0) { start = (char*)std::malloc(8); finish = start; end = start + 8; *finish = 0; }
    String& operator=(const char* t) { std::size_t n = std::strlen(t); if (n < 8) { std::memcpy(start, t, n); finish = start + n; *finish = 0; } return *this; }
    bool empty() const { return finish == start && *start == 0; }
};
template <class T> struct Vector { T* start; T* finish; T* end; Vector() : start(0), finish(0), end(0) { } };
struct Headers { Vector<String> Hdr; String HeaderString; };
struct Http {
    void* Host; String Hostname; unsigned short Port; String PreparedRequest; unsigned long RequestBytesSent, RequestLength;
    Headers MyHeaders, ReceivedHeaders;
    unsigned long TimeOutValue, ActionStartTime; bool ActionStarted; unsigned short CurrentAction;
    unsigned long CurrDownSize, Forwardings, CurrForwarding; char CurrentHeaderPart[2048]; unsigned long CurrentHeaderPos;
    String ProxyHost; unsigned long ProxyPort; Vector<void*> DocumentParts;
    unsigned long DocumentEncoding, DocumentEstimatedLength, DocumentLength; char CurrentDocumentPart[2048];
    unsigned long CurrentDocumentPos; bool InChunkMode; void* CurrentDocumentChunk;
    Http()
    {
        Host = 0; PreparedRequest = ""; Port = 0; Hostname = ""; TimeOutValue = 60; ActionStartTime = 0; CurrentAction = 0;
        CurrentHeaderPos = 0; CurrDownSize = 0; ActionStarted = false; DocumentLength = 0; Forwardings = 10; CurrForwarding = 0;
        DocumentEstimatedLength = 0; std::memset(CurrentDocumentPart, 0, sizeof(CurrentDocumentPart)); CurrentDocumentPos = 0;
        InChunkMode = false; CurrentDocumentChunk = 0; ProxyHost = ""; ProxyPort = 0;
    }
};

int main()
{
    void* raw = std::malloc(sizeof(Http));
    std::memset(raw, 0xcd, sizeof(Http));
    Http* h = new (raw) Http;
    bool ok = h->Host == 0 && h->Hostname.empty() && h->PreparedRequest.empty() && h->ProxyHost.empty()
        && h->MyHeaders.HeaderString.empty() && h->ReceivedHeaders.HeaderString.empty() && h->MyHeaders.Hdr.start == 0
        && h->DocumentParts.start == 0 && h->DocumentParts.end == 0 && h->Port == 0 && h->TimeOutValue == 60
        && h->Forwardings == 10 && h->CurrForwarding == 0 && h->ActionStarted == false && h->CurrentAction == 0
        && h->ProxyPort == 0 && h->DocumentLength == 0 && h->CurrentDocumentPos == 0 && !h->InChunkMode
        && h->CurrentDocumentChunk == 0 && h->CurrentDocumentPart[0] == 0 && h->CurrentDocumentPart[2047] == 0;
    if (!ok) { std::printf("00840f50_TEST FAIL\n"); return 1; }
    std::printf("00840f50_TEST PASS\n");
    return 0;
}