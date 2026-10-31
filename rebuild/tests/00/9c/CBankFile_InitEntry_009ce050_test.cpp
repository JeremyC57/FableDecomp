// Test: CBankFile::InitEntry -- by the open flags: 0x40 maps the symbol to the index, 0x100 adds (CRC(symbol), index) to
// the CRC index, 0x80 with a non-empty filename adds (lower-cased bare filename, index) to the filename multimap.
#include <cstdio>
#include <string>
#include <map>
#include <vector>
#include <cctype>
struct Bank {
    unsigned long OpenFlags;
    std::map<std::string, unsigned long> SymbolIndices;
    std::vector<std::pair<unsigned long, unsigned long> > SymbolCRCIndices;
    std::multimap<std::string, unsigned long> FilenameIndices;
    static unsigned long Crc(const std::string& s) { unsigned long c = 0; for (size_t i = 0; i < s.size(); ++i) c = c * 31 + (unsigned char)s[i]; return c; }
    static std::string ConvertPathToFilename(const std::string& p) { size_t k = p.find_last_of("\\/"); return k == std::string::npos ? p : p.substr(k + 1); }
    static std::string Lower(const std::string& s) { std::string r = s; for (size_t i = 0; i < r.size(); ++i) r[i] = (char)tolower(r[i]); return r; }
    void InitEntry(unsigned long index, const std::string& symbol, const std::string& filename)
    {
        if (OpenFlags & 0x40) SymbolIndices[symbol] = index;
        if (OpenFlags & 0x100) SymbolCRCIndices.push_back(std::make_pair(Crc(symbol), index));
        if ((OpenFlags & 0x80) && !filename.empty()) {
            std::string lower = Lower(ConvertPathToFilename(filename));
            FilenameIndices.insert(std::make_pair(lower, index));
        }
    }
};
int main()
{
    bool ok = true;
    Bank b; b.OpenFlags = 0x1c0;
    b.InitEntry(7, "SYM", "Data\\Graphics\\Tex.DDS");
    b.InitEntry(8, "SYM2", "");
    if (b.SymbolIndices["SYM"] != 7 || b.SymbolIndices["SYM2"] != 8 || b.SymbolCRCIndices.size() != 2) ok = false;
    if (b.SymbolCRCIndices[0].first != Bank::Crc("SYM") || b.SymbolCRCIndices[1].second != 8) ok = false;
    if (b.FilenameIndices.size() != 1 || b.FilenameIndices.find("tex.dds") == b.FilenameIndices.end()) ok = false;
    Bank c; c.OpenFlags = 0x80; c.InitEntry(1, "A", "x.bin"); c.InitEntry(2, "B", "X.BIN");
    if (!c.SymbolIndices.empty() || !c.SymbolCRCIndices.empty() || c.FilenameIndices.count("x.bin") != 2) ok = false;
    printf(ok ? "009ce050_TEST PASS\n" : "009ce050_TEST FAIL\n");
    return ok ? 0 : 1;
}