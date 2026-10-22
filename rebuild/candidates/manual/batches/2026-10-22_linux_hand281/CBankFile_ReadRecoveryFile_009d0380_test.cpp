#include <cstdio>
#include <cstring>
// behaviour test: CBankFile::ReadRecoveryFile -- missing file, bad signature, wrong validation word, and the good path
static int Fail(const char* what) { std::printf("009d0380_TEST FAIL %s\n", what); return 1; }
#define CHECK(cond) do { if (!(cond)) return Fail(#cond); } while (0)
static bool g_exists = true;
static unsigned char g_file[64];
static unsigned int g_fileLen = 0;
static int g_alive = 0, g_closed = 0;
static unsigned int g_entryA = 0, g_entryB = 0, g_entryCalls = 0;
struct CDiskFile {
    bool Opened;
    CDiskFile() : Opened(false) { ++g_alive; }
    ~CDiskFile() { Close(); --g_alive; }
    void Open() { Opened = true; }
    void Close() { if (Opened) ++g_closed; Opened = false; }
};
struct CStream {
    unsigned int Pos;
    CStream(CDiskFile*, int) : Pos(0) { ++g_alive; }
    ~CStream() { --g_alive; }
    void Read(void* dst, unsigned int n) { if (Pos + n <= g_fileLen) std::memcpy(dst, g_file + Pos, n); else std::memset(dst, 0, n); Pos += n; }
};
struct CBankFile {
    bool ReadEntryList(CStream&, unsigned int a, unsigned int b) { g_entryA = a; g_entryB = b; ++g_entryCalls; return true; }
    bool ReadRecoveryFile(unsigned int validation);
};
bool CBankFile::ReadRecoveryFile(unsigned int validation)
{
    if (g_exists) {
        CDiskFile file;
        file.Open();
        CStream stream(&file, 0x4000);
        unsigned int header;
        {
            char signature[] = "BREC";
            stream.Read(&header, 4);
            if (std::memcmp(&header, signature, 4) != 0)
                return false;
        }
        {
            unsigned int value;
            stream.Read(&value, 4);
            if (value != validation)
                return false;
        }
        {
            unsigned int first;
            stream.Read(&first, 4);
            unsigned int second;
            stream.Read(&second, 4);
            ReadEntryList(stream, first, second);
        }
        return true;
    }
    return false;
}
static void Put(unsigned int at, unsigned int v) { std::memcpy(g_file + at, &v, 4); }
int main()
{
    CBankFile bank;
    std::memcpy(g_file, "BREC", 4); Put(4, 0x1234u); Put(8, 7u); Put(12, 9u); g_fileLen = 16;
    CHECK(bank.ReadRecoveryFile(0x1234u) && g_entryCalls == 1 && g_entryA == 7u && g_entryB == 9u && g_alive == 0 && g_closed == 1);
    CHECK(!bank.ReadRecoveryFile(0x1235u) && g_entryCalls == 1 && g_alive == 0 && g_closed == 2);
    g_file[0] = 'X';
    CHECK(!bank.ReadRecoveryFile(0x1234u) && g_entryCalls == 1 && g_alive == 0 && g_closed == 3);
    g_file[0] = 'B'; g_exists = false;
    CHECK(!bank.ReadRecoveryFile(0x1234u) && g_entryCalls == 1 && g_closed == 3);
    std::printf("009d0380_TEST PASS\n");
    return 0;
}
