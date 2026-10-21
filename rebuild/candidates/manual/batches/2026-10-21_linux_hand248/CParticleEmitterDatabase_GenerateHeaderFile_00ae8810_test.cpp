// Behaviour test for 00ae8810 (CParticleEmitterDatabase::GenerateHeaderFile): the header text is the relative header
// file name line, then BEGIN_DATABASE, one BEGIN_EMITTER/END_EMITTER block around each emitter's own entry, and
// END_DATABASE; the text is saved to the given file.
#include <cstdio>
#include <cstring>

static char g_text[512];
static char g_saved[32];
struct Ctx { void Transfer(const char* s) { std::strcat(g_text, s); } };
struct Emitter { int id; void GenerateHeader(Ctx& c, long options) { char b[16]; std::sprintf(b, "E%d:%ld\r\n", id, options); c.Transfer(b); } };
struct Db {
    Emitter* first; Emitter* last;
    void GenerateHeaderFile(const char* filename, long options)
    {
        g_text[0] = 0;
        Ctx c;
        c.Transfer("HeaderFilename \"");
        c.Transfer("fx.h");
        c.Transfer("\"\r\n");
        c.Transfer("BEGIN_DATABASE\r\n");
        for (Emitter* it = first; it != last; ++it) {
            c.Transfer("BEGIN_EMITTER\r\n");
            it->GenerateHeader(c, options);
            c.Transfer("END_EMITTER\r\n");
        }
        c.Transfer("END_DATABASE\r\n");
        std::strcpy(g_saved, filename);
    }
};
int main()
{
    Emitter e[2] = { { 1 }, { 2 } };
    Db db; db.first = e; db.last = e + 2;
    db.GenerateHeaderFile("out.h", 7);
    const char* want = "HeaderFilename \"fx.h\"\r\nBEGIN_DATABASE\r\nBEGIN_EMITTER\r\nE1:7\r\nEND_EMITTER\r\nBEGIN_EMITTER\r\nE2:7\r\nEND_EMITTER\r\nEND_DATABASE\r\n";
    int ok = std::strcmp(g_text, want) == 0 && std::strcmp(g_saved, "out.h") == 0;
    if (!ok) { std::printf("00ae8810_TEST FAIL\n"); return 1; }
    std::printf("00ae8810_TEST PASS\n");
    return 0;
}
