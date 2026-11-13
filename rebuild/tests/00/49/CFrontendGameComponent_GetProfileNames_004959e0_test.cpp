// Behaviour test for CFrontendGameComponent::GetProfileNames (0x004959E0): a host model of the row, statement for statement as
// src/004959e0.cpp, over an in-memory save directory:
//  - clear the list; scan "<save dir>*": skip entries whose name ends in "." ("." / "..") and stop collecting when the pattern
//    itself names a file; for every other entry scan "<entry>\*.bin";
//  - each profile file that opens: a "FablePro" file is {8-byte signature, long version, long end offset, text up to the end
//    offset}, anything else is the whole file as text; "ProfileName <name>" transferred out of the text (default "") is added;
//  - finally the names are sorted.
#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)

struct ProfileFile { const char* name; bool opens; unsigned char data[128]; long size; };
struct Entry { const char* name; ProfileFile files[3]; long fileCount; };

struct Names {
    char items[16][32]; long count;
    void clear() { count = 0; }
    void push_back(const char* s) { std::strcpy(items[count++], s); }
    void sort()
    {
        for (long i = 1; i < count; ++i)
            for (long j = i; j > 0 && std::strcmp(items[j - 1], items[j]) > 0; --j) {
                char t[32]; std::strcpy(t, items[j]); std::strcpy(items[j], items[j - 1]); std::strcpy(items[j - 1], t);
            }
    }
};

static bool EndsWith(const char* s, const char* suffix)
{
    long offset = (long)std::strlen(s) - (long)std::strlen(suffix);
    if (offset >= 0) return std::strcmp(s + offset, suffix) == 0;
    return false;
}

// "ProfileName <word>" -> name (the transfer default is the empty string)
static void TransferProfileName(const char* text, char* out)
{
    out[0] = 0;
    const char* f = std::strstr(text, "ProfileName");
    if (!f) return;
    f += std::strlen("ProfileName");
    while (*f == ' ') ++f;
    long n = 0;
    while (f[n] && f[n] != ';' && f[n] != ' ' && f[n] != '\n') { out[n] = f[n]; ++n; }
    out[n] = 0;
}

static void GetProfileNames(Names& names, const Entry* entries, long entryCount, bool patternIsFile)
{
    names.clear();
    for (long e = 0; e < entryCount; ++e) {
        bool dots = EndsWith(entries[e].name, ".");
        if (!dots && !patternIsFile) {
            for (long f = 0; f < entries[e].fileCount; ++f) {
                const ProfileFile& file = entries[e].files[f];
                if (file.opens) {
                    char text[128];
                    if (file.size >= 8 && std::memcmp(file.data, "FablePro", 8) == 0) {
                        long pos = 8;
                        pos += 4;                                          // version, unused
                        int length; std::memcpy(&length, file.data + pos, 4); pos += 4;
                        long n = length - pos;                             // ReadString(length - GetPosition())
                        std::memcpy(text, file.data + pos, n); text[n] = 0;
                    } else {
                        std::memcpy(text, file.data, file.size); text[file.size] = 0;
                    }
                    char profileName[32];
                    TransferProfileName(text, profileName);
                    names.push_back(profileName);
                }
            }
        }
    }
    names.sort();
}

static void MakeFablePro(ProfileFile& f, const char* name, const char* text)
{
    f.name = name; f.opens = true;
    std::memcpy(f.data, "FablePro", 8);
    int version = 3; std::memcpy(f.data + 8, &version, 4);
    int length = 16 + (int)std::strlen(text); std::memcpy(f.data + 12, &length, 4);
    std::memcpy(f.data + 16, text, std::strlen(text));
    f.size = length;
}

static void MakeText(ProfileFile& f, const char* name, const char* text, bool opens)
{
    f.name = name; f.opens = opens;
    f.size = (long)std::strlen(text); std::memcpy(f.data, text, f.size);
}

int main()
{
    Entry entries[4];
    entries[0].name = "."; entries[0].fileCount = 1; MakeFablePro(entries[0].files[0], "x.bin", "ProfileName Dot;");
    entries[1].name = "Zed"; entries[1].fileCount = 2;
    MakeFablePro(entries[1].files[0], "a.bin", "ProfileName Zed;\nOther 1;");
    MakeText(entries[1].files[1], "b.bin", "ProfileName Locked;", false);
    entries[2].name = "Amy"; entries[2].fileCount = 1; MakeText(entries[2].files[0], "p.bin", "Gamma 1.0;\nProfileName Amy;", true);
    entries[3].name = ".."; entries[3].fileCount = 0;

    Names names; names.count = 5;
    GetProfileNames(names, entries, 4, false);
    CHECK(names.count == 2);
    CHECK(std::strcmp(names.items[0], "Amy") == 0);
    CHECK(std::strcmp(names.items[1], "Zed") == 0);

    // a FablePro file whose text has no ProfileName contributes the empty default, sorted first
    Entry more[1]; more[0].name = "Bob"; more[0].fileCount = 1; MakeFablePro(more[0].files[0], "q.bin", "Gamma 1.0;");
    GetProfileNames(names, more, 1, false);
    CHECK(names.count == 1 && names.items[0][0] == 0);

    // the pattern naming a file stops every entry
    GetProfileNames(names, entries, 4, true);
    CHECK(names.count == 0);

    if (g_fail) { std::printf("004959e0_TEST FAIL\n"); return 1; }
    std::printf("004959e0_TEST PASS\n");
    return 0;
}