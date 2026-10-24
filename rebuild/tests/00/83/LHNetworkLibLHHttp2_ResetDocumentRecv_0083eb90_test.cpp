// Behaviour test for 0x0083EB90 LHHttp2::ResetDocumentRecv: clears the header/document cursors, lengths, both 2048-byte
// receive buffers and the chunk-mode flag, then runs ResetDocumentParts; unrelated fields stay untouched.
#include <cstdio>
#include <cstring>

namespace LHNetworkLib {

static int g_partsResets = 0;
struct LHHttp2 {
    unsigned char _pad0[0x70];
    char          CurrentHeaderPart[2048];
    unsigned long CurrentHeaderPos;
    unsigned char _pad874[0x20];
    unsigned long DocumentEstimatedLength;
    unsigned long DocumentLength;
    char          CurrentDocumentPart[2048];
    unsigned long CurrentDocumentPos;
    bool          InChunkMode;
    void ResetDocumentParts() { ++g_partsResets; }
    void ResetDocumentRecv();
};

void LHHttp2::ResetDocumentRecv()
{
    CurrentHeaderPos = 0;
    DocumentEstimatedLength = 0;
    DocumentLength = 0;
    memset(CurrentHeaderPart, 0, sizeof(CurrentHeaderPart));
    memset(CurrentDocumentPart, 0, sizeof(CurrentDocumentPart));
    InChunkMode = false;
    ResetDocumentParts();
}

}

int main()
{
    static LHNetworkLib::LHHttp2 http;
    std::memset(&http, 0x5A, sizeof(http));
    http.InChunkMode = true;
    http.ResetDocumentRecv();
    int ok = LHNetworkLib::g_partsResets == 1 && http.CurrentHeaderPos == 0 && http.DocumentEstimatedLength == 0 &&
             http.DocumentLength == 0 && !http.InChunkMode && http._pad0[0x6f] == 0x5A && http._pad874[0] == 0x5A &&
             http.CurrentDocumentPos != 0;
    for (int i = 0; i < 2048; ++i) ok = ok && http.CurrentHeaderPart[i] == 0 && http.CurrentDocumentPart[i] == 0;
    if (!ok) { std::printf("0083eb90_TEST FAIL\n"); return 1; }
    std::printf("0083eb90_TEST PASS\n");
    return 0;
}