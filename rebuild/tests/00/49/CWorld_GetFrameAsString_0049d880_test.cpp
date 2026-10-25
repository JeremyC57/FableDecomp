// Behaviour test for CWorld::GetFrameAsString (0x0049D880): the static frame counter formatted by GFIntToCharString.
#include <cstdio>
#include <cstring>

class CCharString {
public:
    char Text[16];
    CCharString() { Text[0] = 0; }
    CCharString(const CCharString& other) { std::strcpy(Text, other.Text); }
    ~CCharString() {}
};

CCharString GFIntToCharString(long value)
{
    CCharString s;
    std::sprintf(s.Text, "%ld", value);
    return s;
}

class CWorld {
public:
    static CCharString GetFrameAsString();
    static long Frame;
};
long CWorld::Frame = 0;

CCharString CWorld::GetFrameAsString()
{
    return GFIntToCharString(Frame);
}

int main()
{
    CWorld::Frame = 1234;
    CCharString a = CWorld::GetFrameAsString();
    CWorld::Frame = -7;
    CCharString b = CWorld::GetFrameAsString();
    if (std::strcmp(a.Text, "1234") != 0 || std::strcmp(b.Text, "-7") != 0) { std::printf("0049d880_TEST FAIL\n"); return 1; }
    std::printf("0049d880_TEST PASS\n");
    return 0;
}