// Behaviour test for LHNetworkLib::LHHttp2::~LHHttp2 (0x008410D0): the socket is closed and deleted once, Reset runs
// twice (Close's and the destructor's), each header set is reset (header string emptied, lines cleared) before its
// members are destroyed, and every owned block is released.
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int g_socketClosed = 0, g_socketDeleted = 0, g_resets = 0, g_assigns = 0, g_clears = 0, g_vectorDtors = 0;
static int g_frees = 0;

static void Release(void* p) { if (p != 0) { std::free(p); ++g_frees; } }

struct Socket {
    bool Close() { ++g_socketClosed; return true; }
    ~Socket() { ++g_socketDeleted; }
};

struct Str {
    char* start;
    ~Str() { Release(start); }
    void AssignEmpty() { ++g_assigns; }
};

struct StrVector {
    Str* start;
    ~StrVector() { ++g_vectorDtors; }
    void Clear() { ++g_clears; }
};

struct PartVector {
    void** start;
    ~PartVector() { Release(start); }
};

struct Headers {
    StrVector lines;
    Str text;
    ~Headers() { Reset(); }
    void Reset() { text.AssignEmpty(); lines.Clear(); }
};

struct Http {
    Socket* Host;
    Str Hostname;
    Str PreparedRequest;
    Headers MyHeaders;
    Headers ReceivedHeaders;
    Str ProxyHost;
    PartVector DocumentParts;
    void Reset() { ++g_resets; }
    long Close()
    {
        if (Host) { Host->Close(); delete Host; Host = 0; }
        Reset();
        return 0;
    }
    ~Http() { Close(); Reset(); }
};

static char* Block() { return (char*)std::malloc(8); }

int main()
{
    int bad = 0;
    {
        Http* h = new Http;
        h->Host = new Socket;
        h->Hostname.start = Block();
        h->PreparedRequest.start = Block();
        h->MyHeaders.lines.start = 0; h->MyHeaders.text.start = Block();
        h->ReceivedHeaders.lines.start = 0; h->ReceivedHeaders.text.start = 0;
        h->ProxyHost.start = Block();
        h->DocumentParts.start = (void**)std::malloc(16);
        delete h;
    }
    if (g_socketClosed != 1 || g_socketDeleted != 1) bad |= 1;
    if (g_resets != 2) bad |= 2;
    if (g_assigns != 2 || g_clears != 2 || g_vectorDtors != 2) bad |= 4;
    if (g_frees != 5) bad |= 8;
    {
        Http* h = new Http;                       // no socket: nothing to close, still two resets
        h->Host = 0;
        h->Hostname.start = 0; h->PreparedRequest.start = 0;
        h->MyHeaders.lines.start = 0; h->MyHeaders.text.start = 0;
        h->ReceivedHeaders.lines.start = 0; h->ReceivedHeaders.text.start = 0;
        h->ProxyHost.start = 0; h->DocumentParts.start = 0;
        delete h;
    }
    if (g_socketClosed != 1 || g_resets != 4 || g_frees != 5) bad |= 16;
    if (bad) { std::printf("008410d0_TEST FAIL\n"); return 1; }
    std::printf("008410d0_TEST PASS\n");
    return 0;
}