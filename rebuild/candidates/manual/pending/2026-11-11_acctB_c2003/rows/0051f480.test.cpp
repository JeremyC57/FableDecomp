// Behaviour test for NThingManager::CEntityRuntimePersistence::ClearPersistenceDataForSectionInEntityScript (0x0051F480): a host model
// of the two halves (statement for statement as src/0051f480.cpp, with the zlib round trip modelled as a copy):
//  - text: find "XXXSectionStart", read the name up to ';', and when it is the section cut [start of the marker, past
//    "XXXSectionEnd;") out of the script;
//  - binary: {count, then per entry: name NUL, long size, size bytes}; the matching entry is erased, count - 1 is written back,
//    UncompressedSize becomes the new length.
#include <cstdio>
#include <cstring>

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)

struct Info {
    char script[512];
    unsigned char data[512];   // "compressed" save data (identity compression in the model)
    long data_size;
    long uncompressed_size;
};

// --- parser model (SkipPastString / SkipWhitespace / ReadAsStringUntilString / ReadAsStringUntilPastString) ---
struct Parser {
    const char* text; unsigned long pos;
    bool SkipPastString(const char* s)
    {
        const char* f = std::strstr(text + pos, s);
        if (!f) return false;
        pos = (unsigned long)(f - text) + (unsigned long)std::strlen(s);
        return true;
    }
    void SkipWhitespace() { while (text[pos] == ' ' || text[pos] == '\t' || text[pos] == '\r' || text[pos] == '\n') ++pos; }
    void ReadAsStringUntilString(const char* s, char* out)
    {
        const char* f = std::strstr(text + pos, s);
        unsigned long end = f ? (unsigned long)(f - text) : (unsigned long)std::strlen(text);
        std::memcpy(out, text + pos, end - pos); out[end - pos] = 0;
        pos = end;
    }
    void ReadAsStringUntilPastString(const char* s)
    {
        const char* f = std::strstr(text + pos, s);
        pos = f ? (unsigned long)(f - text) + (unsigned long)std::strlen(s) : (unsigned long)std::strlen(text);
    }
};

static void ClearSection(const char* section, Info& info)
{
    Parser parser; parser.text = info.script; parser.pos = 0;
    while (parser.SkipPastString("XXXSectionStart")) {
        long section_start = (long)parser.pos - (long)std::strlen("XXXSectionStart");
        parser.SkipWhitespace();
        char name[128];
        parser.ReadAsStringUntilString(";", name);
        if (std::strcmp(name, section) == 0) {
            parser.ReadAsStringUntilPastString("XXXSectionEnd;");
            char result[512];
            std::memcpy(result, info.script, section_start);
            std::strcpy(result + section_start, info.script + parser.pos);
            std::strcpy(info.script, result);
            break;
        }
    }
    if (info.uncompressed_size > 0) {
        unsigned char decompressed[512];
        long size_all = info.uncompressed_size;
        std::memcpy(decompressed, info.data, size_all);                   // InternalDecompress
        unsigned long pos = 0;
        unsigned long count; std::memcpy(&count, decompressed, 4); pos = 4;   // ReadULONG
        for (unsigned long i = 0; i < count; ++i) {
            unsigned long entry_start = pos;
            const char* name = (const char*)decompressed + pos;               // ReadNullTerminatedString
            pos += (unsigned long)std::strlen(name) + 1;
            long size; std::memcpy(&size, decompressed + pos, 4); pos += 4;    // ReadSLONG
            if (size > 0) pos += size;                                         // ReadConstData
            unsigned long entry_end = pos;
            if (std::strcmp(name, section) == 0) {
                unsigned long new_count = count - 1;
                std::memcpy(decompressed, &new_count, 4);
                std::memmove(decompressed + entry_start, decompressed + entry_end, size_all - entry_end);   // erase
                size_all -= (long)(entry_end - entry_start);
                info.uncompressed_size = size_all;
                std::memcpy(info.data, decompressed, size_all); info.data_size = size_all;                  // InternalCompress
                break;
            }
        }
    }
}

static long Put(unsigned char* buf, long at, const char* name, const char* payload)
{
    long n = (long)std::strlen(name) + 1, sz = (long)std::strlen(payload);
    std::memcpy(buf + at, name, n); at += n;
    std::memcpy(buf + at, &sz, 4); at += 4;
    std::memcpy(buf + at, payload, sz); at += sz;
    return at;
}

int main()
{
    Info info;
    std::memset(&info, 0, sizeof info);
    std::strcpy(info.script, "Head;\nXXXSectionStart A;\nx=1;\nXXXSectionEnd;\nXXXSectionStart B;\ny=2;\nXXXSectionEnd;\nTail;");
    unsigned long count = 3;
    std::memcpy(info.data, &count, 4);
    long at = 4;
    at = Put(info.data, at, "A", "aaaa");
    at = Put(info.data, at, "B", "bbbbbb");
    at = Put(info.data, at, "C", "cc");
    info.uncompressed_size = at; info.data_size = at;

    ClearSection("B", info);
    CHECK(std::strcmp(info.script, "Head;\nXXXSectionStart A;\nx=1;\nXXXSectionEnd;\n\nTail;") == 0);
    unsigned long c2; std::memcpy(&c2, info.data, 4);
    CHECK(c2 == 2);
    CHECK(info.uncompressed_size == at - (2 + 4 + 6));
    {
        unsigned char expect[512]; unsigned long c = 2; std::memcpy(expect, &c, 4);
        long e = 4; e = Put(expect, e, "A", "aaaa"); e = Put(expect, e, "C", "cc");
        CHECK(e == info.uncompressed_size && std::memcmp(expect, info.data, e) == 0);
    }

    // an absent section changes nothing
    Info before = info;
    ClearSection("Z", info);
    CHECK(std::memcmp(&before, &info, sizeof info) == 0);

    // no binary data: only the text is cut
    std::memset(&info, 0, sizeof info);
    std::strcpy(info.script, "XXXSectionStart  A;q;XXXSectionEnd;rest");
    ClearSection("A", info);
    CHECK(std::strcmp(info.script, "rest") == 0 && info.uncompressed_size == 0);

    if (g_fail) { std::printf("0051f480_TEST FAIL\n"); return 1; }
    std::printf("0051f480_TEST PASS\n");
    return 0;
}
