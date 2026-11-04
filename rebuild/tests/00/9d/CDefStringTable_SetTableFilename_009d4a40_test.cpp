// Behaviour test for CDefStringTable::SetTableFilename 0x009D4A40: the table keeps a copy of the given file name (later changes to the
// caller's string do not leak in), and a second call replaces it.
#include <cstdio>
#include <string>
struct CDefStringTable {
    unsigned char StringData[0x30]; std::wstring TableFilename;
    void SetTableFilename(const std::wstring& filename) { TableFilename = filename; }
};
int main()
{
    CDefStringTable t;
    std::wstring name = L"Data\\Defs\\names.bin";
    t.SetTableFilename(name);
    name = L"changed";
    if (t.TableFilename != L"Data\\Defs\\names.bin") { std::printf("009d4a40_TEST FAIL\n"); return 1; }
    t.SetTableFilename(L"other.bin");
    if (t.TableFilename != L"other.bin") { std::printf("009d4a40_TEST FAIL\n"); return 1; }
    std::printf("009d4a40_TEST PASS\n");
    return 0;
}