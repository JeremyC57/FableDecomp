// Behaviour test for 0x009a8840: CBankFileManager::OpenRetailBank -- magic/version gate, contained-bank table, threaded file
// Host model: files are byte vectors keyed by path; the stream is a bounded little-endian reader with SetPosition.
#include <cstdio>
#include <cstring>
#include <map>
#include <list>
#include <string>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
typedef std::vector<unsigned char> Bytes;
static std::map<std::string, Bytes> g_files;
struct Stream {                                   // CDataInputStream: reads past the end leave the target untouched
    const Bytes* Data; unsigned long Pos;
    explicit Stream(const Bytes* d) : Data(d), Pos(0) { }
    void Read(void* dst, long size) { if (size <= 0 || Pos + size > Data->size()) return; std::memcpy(dst, &(*Data)[Pos], size); Pos += size; }
    unsigned long ReadULONG() { unsigned long v = 0; unsigned int w = 0; Read(&w, 4); v = w; return v; }
    void SetPosition(unsigned long p) { Pos = p; }
    std::string ReadNullTerminatedString() { std::string s; while (Pos < Data->size() && (*Data)[Pos]) s += (char)(*Data)[Pos++]; ++Pos; return s; }
};
struct CContainedBank { unsigned long DataVersion, EntryCount, InfoOffset, InfoSize, Alignment; };
struct CRetailBank {
    const Bytes* DiskFile; std::string ThreadedPath; bool ThreadedOpen; bool UseDMA;
    std::map<std::string, CContainedBank> ContainedBanks; std::string FilePath;
    CRetailBank() : DiskFile(0), ThreadedOpen(false), UseDMA(true) { }
};
struct CBankFileManager {
    std::list<CRetailBank*> RetailBanks; std::list<const Bytes*> Files; unsigned long FileVersion;
    ~CBankFileManager() { for (std::list<CRetailBank*>::iterator i = RetailBanks.begin(); i != RetailBanks.end(); ++i) delete *i; for (std::list<const Bytes*>::iterator f = Files.begin(); f != Files.end(); ++f) delete *f; }
    void OpenRetailBank(const std::string& pathname, bool)
    {
        if (!g_files.count(pathname)) return;
        const Bytes* disk_file = new Bytes(g_files[pathname]);
        Files.push_back(disk_file);                   // the counted disk file outlives a rejected open only in this host model
        Stream stream(disk_file);
        char magic[4] = { 0, 0, 0, 0 };
        stream.Read(magic, 4);
        if (std::memcmp(magic, "BIGB", 4) == 0) {
            if (stream.ReadULONG() != FileVersion) return;
            CRetailBank* bank = new CRetailBank;
            RetailBanks.push_back(bank);
            bank->FilePath = pathname;
            bank->DiskFile = disk_file;
            unsigned long header_offset = stream.ReadULONG();
            unsigned long header_size = stream.ReadULONG();
            if (header_size > 0) {
                Bytes header(header_size);
                stream.SetPosition(header_offset);
                stream.Read(&header[0], header_size);
                Stream header_stream(&header);
                unsigned long no_banks = header_stream.ReadULONG();
                for (unsigned long i = 0; i < no_banks; ++i) {
                    std::string name = header_stream.ReadNullTerminatedString();
                    CContainedBank c;
                    c.DataVersion = header_stream.ReadULONG(); c.EntryCount = header_stream.ReadULONG();
                    c.InfoOffset = header_stream.ReadULONG(); c.InfoSize = header_stream.ReadULONG(); c.Alignment = header_stream.ReadULONG();
                    bank->ContainedBanks[name] = c;
                }
            }
            bank->ThreadedPath = pathname; bank->UseDMA = false; bank->ThreadedOpen = true;
        }
    }
};
static void PutU32(Bytes& b, unsigned int v) { for (int i = 0; i < 4; ++i) b.push_back((unsigned char)(v >> (8 * i))); }
static Bytes MakeBank(const char* magic, unsigned int version)
{
    Bytes header; PutU32(header, 2);
    const char* names[2] = { "GBANK_GUI", "GBANK_MAIN" };
    for (int b = 0; b < 2; ++b) { for (const char* p = names[b]; *p; ++p) header.push_back(*p); header.push_back(0); for (int k = 0; k < 5; ++k) PutU32(header, 100 * (b + 1) + k); }
    Bytes f; for (int i = 0; i < 4; ++i) f.push_back(magic[i]); PutU32(f, version); PutU32(f, 32); PutU32(f, (unsigned int)header.size());
    while (f.size() < 32) f.push_back(0xCD);
    f.insert(f.end(), header.begin(), header.end());
    return f;
}
int main()
{
    CBankFileManager m; m.FileVersion = 100;
    m.OpenRetailBank("missing.big", true);
    CHECK(m.RetailBanks.empty());
    g_files["bad.big"] = MakeBank("BIGA", 100);
    m.OpenRetailBank("bad.big", true);
    CHECK(m.RetailBanks.empty());
    g_files["old.big"] = MakeBank("BIGB", 99);
    m.OpenRetailBank("old.big", true);
    CHECK(m.RetailBanks.empty());
    g_files["frontend.big"] = MakeBank("BIGB", 100);
    m.OpenRetailBank("frontend.big", false);
    CHECK(m.RetailBanks.size() == 1);
    const CRetailBank& bank = *m.RetailBanks.front();
    CHECK(bank.FilePath == "frontend.big" && bank.DiskFile != 0 && bank.ThreadedOpen && !bank.UseDMA);
    CHECK(bank.ContainedBanks.size() == 2);
    const CContainedBank& main = bank.ContainedBanks.find("GBANK_MAIN")->second;
    CHECK(main.DataVersion == 200 && main.EntryCount == 201 && main.InfoOffset == 202 && main.InfoSize == 203 && main.Alignment == 204);
    CHECK(bank.ContainedBanks.find("GBANK_GUI")->second.Alignment == 104);
    if (g_fail) { std::printf("009a8840_TEST FAIL\n"); return 1; }
    std::printf("009a8840_TEST PASS\n");
    return 0;
}