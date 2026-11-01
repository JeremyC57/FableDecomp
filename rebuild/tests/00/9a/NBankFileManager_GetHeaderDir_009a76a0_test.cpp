// Behaviour test for 0x009A76A0 NBankFileManager::GetHeaderDir(): returns a copy of SymbolHeaderDir.
#include <cstdio>
#include <cstring>
class CWideString {
public:
    char Text[16];
    CWideString() { Text[0] = 0; }
    CWideString(const CWideString& o) { std::memcpy(Text, o.Text, sizeof(Text)); }
};
namespace NBankFileManager {
    CWideString SymbolHeaderDir;
    CWideString GetHeaderDir() { return SymbolHeaderDir; }
}
int main()
{
    std::strcpy(NBankFileManager::SymbolHeaderDir.Text, "Data\\Bank\\");
    CWideString d = NBankFileManager::GetHeaderDir();
    NBankFileManager::SymbolHeaderDir.Text[0] = 'X';
    if (std::strcmp(d.Text, "Data\\Bank\\") != 0) { std::printf("B1"); return 1; }
    std::printf("RB_9a76a0_OK\n"); return 0;
}
