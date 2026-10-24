#include <cstdio>

// Model of 0x0040E1E0 GetDefaultVal<CCharString>: default-construct a local string and return it by value (VC7.1
// copies the named local into the hidden result).
static int g_defaultCtors = 0;
class CCharString {
public:
    const char* Text;
    CCharString() : Text("") { ++g_defaultCtors; }
    CCharString(const CCharString& other) : Text(other.Text) { }
};
template <class T> T GetDefaultVal()
{
    T value;
    return value;
}

int main()
{
    CCharString s = GetDefaultVal<CCharString>();
    if (s.Text == 0 || s.Text[0] != '\0' || g_defaultCtors != 1) {
        std::printf("0040e1e0_TEST FAIL\n");
        return 1;
    }
    std::printf("0040e1e0_TEST PASS\n");
    return 0;
}