// Behaviour test for CBankFile::FindIndicesByFilename 0x009CCEF0: a copy of the model's logic over std containers.
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include <cctype>
struct Bank {
    std::vector<std::string> Symbols;
    std::vector<unsigned char> RuntimeType;   // RuntimeData[i].DataType
    std::vector<unsigned long> Packed;        // PackedDataType (index-1), empty = not packed
    std::multimap<std::string, unsigned long> FilenameIndices;
    static std::string ConvertPathToFilename(const std::string& p)
    {
        std::string::size_type s = p.find_last_of("\\/");
        std::string f = s == std::string::npos ? p : p.substr(s + 1);
        std::string::size_type d = f.find('.');
        return d == std::string::npos ? f : f.substr(0, d);
    }
    unsigned long GetEntryDataType(unsigned long i) const { return Packed.size() ? Packed[i - 1] : RuntimeType[i]; }
    std::string GetEntrySymbol(unsigned long i) const { return Symbols.empty() ? std::string("(Unknown)") : Symbols[i]; }
    std::vector<unsigned long> FindIndicesByFilename(const std::string& path, long type, bool excludeUnassigned) const
    {
        std::vector<unsigned long> out;
        std::string name = ConvertPathToFilename(path);
        for (size_t k = 0; k < name.size(); ++k) if ((unsigned char)name[k] < 0x80) name[k] = (char)tolower(name[k]);
        typedef std::multimap<std::string, unsigned long>::const_iterator It;
        std::pair<It, It> r = FilenameIndices.equal_range(name);
        for (It it = r.first; it != r.second; ++it) {
            unsigned long index = it->second;
            if (type == -1 || GetEntryDataType(index) == (unsigned long)type) {
                std::string sym = GetEntrySymbol(index);
                bool skip = excludeUnassigned && sym.find('[') == std::string::npos && strncmp(sym.c_str(), "UNASSIGNED", 10) != 0;
                if (!skip) out.push_back(index);
            }
        }
        return out;
    }
};
int main()
{
    Bank b;
    b.Symbols.push_back("A"); b.Symbols.push_back("TEX_ROCK"); b.Symbols.push_back("UNASSIGNED_3"); b.Symbols.push_back("X[1]");
    b.RuntimeType.push_back(0); b.RuntimeType.push_back(2); b.RuntimeType.push_back(5); b.RuntimeType.push_back(2);
    b.FilenameIndices.insert(std::make_pair(std::string("rock"), 1UL));
    b.FilenameIndices.insert(std::make_pair(std::string("rock"), 2UL));
    b.FilenameIndices.insert(std::make_pair(std::string("rock"), 3UL));
    b.FilenameIndices.insert(std::make_pair(std::string("tree"), 0UL));
    std::vector<unsigned long> v = b.FindIndicesByFilename("C:\\Data\\ROCK.tga", -1, false);
    bool ok = v.size() == 3 && v[0] == 1 && v[1] == 2 && v[2] == 3;
    v = b.FindIndicesByFilename("rock", 2, false);
    ok = ok && v.size() == 2 && v[0] == 1 && v[1] == 3;
    v = b.FindIndicesByFilename("Rock", -1, true);   // keeps '[' or UNASSIGNED symbols only
    ok = ok && v.size() == 2 && v[0] == 2 && v[1] == 3;
    v = b.FindIndicesByFilename("none", -1, false);
    ok = ok && v.empty();
    b.Packed.push_back(7); b.Packed.push_back(9); b.Packed.push_back(7);
    v = b.FindIndicesByFilename("rock", 7, false);   // packed: types of 1,2,3 are Packed[0..2] = 7,9,7
    ok = ok && v.size() == 2 && v[0] == 1 && v[1] == 3;
    if (!ok) { std::printf("009ccef0_TEST FAIL\n"); return 1; }
    std::printf("009ccef0_TEST PASS\n");
    return 0;
}