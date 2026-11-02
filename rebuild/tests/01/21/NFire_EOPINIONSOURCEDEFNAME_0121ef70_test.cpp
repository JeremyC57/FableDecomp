// Behaviour test for 0x0121EF70: NFire::OPINION_SOURCE_DEF_NAME is a const reference bound to a lifetime-extended temporary
// CCharString built from "OPINION_SOURCE_SCARY_FIRE"; the temporary lives as long as the program and is destroyed once at exit.
#include <cstdio>
#include <cstring>
static int g_Constructed = 0;
class CCharString {
public:
    CCharString(const char* text, long length = -1) : Text(text), Length(length < 0 ? (long)std::strlen(text) : length) { ++g_Constructed; }
    ~CCharString() { Text = 0; }
    const char* Text;
    long Length;
};
namespace NFire {
const CCharString& OPINION_SOURCE_DEF_NAME = "OPINION_SOURCE_SCARY_FIRE";
}
static bool UsesName(const CCharString& name) { return name.Text != 0 && std::strcmp(name.Text, "OPINION_SOURCE_SCARY_FIRE") == 0; }
int main()
{
    if (g_Constructed != 1 || !UsesName(NFire::OPINION_SOURCE_DEF_NAME) || NFire::OPINION_SOURCE_DEF_NAME.Length != 25) { std::printf("0121ef70_TEST FAIL\n"); return 1; }
    std::printf("0121ef70_TEST PASS\n");
    return 0;
}