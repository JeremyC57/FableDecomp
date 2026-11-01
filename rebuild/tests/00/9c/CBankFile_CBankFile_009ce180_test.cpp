// behaviour test: CBankFile::~CBankFile -- the user body calls Close() (which closes the entries) before any member
// is destroyed; the members then go in reverse order (checksum cache last declared, so first).
#include <cstdio>
static char g_log[16]; static int g_n = 0;
struct Member { char tag; explicit Member(char t) : tag(t) { } ~Member() { g_log[g_n++] = tag; } };
class CBankFile { public:
    bool open; Member Entries; Member NameMap; Member ChecksumCache;
    CBankFile() : open(true), Entries('E'), NameMap('N'), ChecksumCache('C') { }
    virtual ~CBankFile() { Close(); }
    void Close() { CloseEntries(); }
    void CloseEntries() { if (open) { open = false; g_log[g_n++] = 'x'; } }
};
int main()
{
    CBankFile* b = new CBankFile;
    delete b;
    g_log[g_n] = 0;
    const char* want = "xCNE";
    for (int i = 0; want[i] || g_log[i]; ++i)
        if (want[i] != g_log[i]) { std::printf("009ce180_TEST FAIL\n"); return 1; }
    std::printf("009ce180_TEST PASS\n");
    return 0;
}