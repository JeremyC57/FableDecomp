// Behaviour test for 0x009E5C90 CSymbolMap::LoadFromFile: a standalone model of the matched source (the inline
// ReadWholeFile helper reads the file object's size, allocates size+1 zeroed bytes, terminates at [size], reads the
// file, then the caller strips // and /* */ comments, parses, sorts, trims and frees). Prints exactly one line.
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct FakeFile {
    const char* m_text;
    long        m_size;
    long        m_reads;
    void Read(void* buffer, long size, void* /*process*/) { std::memcpy(buffer, m_text, size); ++m_reads; }
};

// Same shape as the row's helper: a reference to the file object, a local buffer, terminator at [size].
inline char* ReadWholeFile(FakeFile& file)
{
    unsigned long length = file.m_size + 1;
    char* buffer = length ? (char*)std::malloc(length) : 0;
    std::memset(buffer, 0, length);
    buffer[file.m_size] = 0;
    file.Read(buffer, file.m_size, 0);
    return buffer;
}

static void RemoveComments(char* buffer, const char* start, const char* end)
{
    for (;;) {
        char* s = std::strstr(buffer, start);
        if (!s) return;
        char* e = std::strstr(s + std::strlen(start), end);
        if (!e) { *s = 0; return; }
        bool lineEnd = (end[0] == '\n' || end[0] == '\r');
        char* resume = lineEnd ? e : e + std::strlen(end);
        std::memmove(s, resume, std::strlen(resume) + 1);
    }
}

static int g_parsed = 0;
static bool ParseBuffer(char* buffer) { g_parsed = (int)std::strlen(buffer); return g_parsed != 0; }

static bool LoadFromFile(FakeFile& file)
{
    bool ok = false;
    char* buffer = ReadWholeFile(file);
    RemoveComments(buffer, "//", "\n");
    RemoveComments(buffer, "//", "\r");
    RemoveComments(buffer, "/*", "*/");
    if (ParseBuffer(buffer))
        ok = true;
    if (buffer)
        std::free(buffer);
    return ok;
}

int main()
{
    int fails = 0;
    const char* text = "A=1 // x\nB=2 /* y */C=3";
    FakeFile file = { text, (long)std::strlen(text), 0 };
    // the helper: size+1 bytes, terminated, read once
    char* b = ReadWholeFile(file);
    if (std::strlen(b) != (size_t)file.m_size || b[file.m_size] != 0 || file.m_reads != 1) ++fails;
    std::free(b);
    // the whole load: comments stripped, parse seen non-empty text
    if (!LoadFromFile(file) || g_parsed != (int)std::strlen("A=1 \nB=2 C=3")) ++fails;
    // an empty file: zero-length buffer still terminated and parse fails -> false
    FakeFile empty = { "", 0, 0 };
    if (LoadFromFile(empty)) ++fails;
    std::printf(fails == 0 ? "009e5c90_TEST PASS\n" : "009e5c90_TEST FAIL\n");
    return fails == 0 ? 0 : 1;
}