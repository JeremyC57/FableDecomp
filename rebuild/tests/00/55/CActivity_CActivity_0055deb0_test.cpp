// Behaviour test for the CActivity constructor @ 0x0055DEB0: three empty strings, both byte members cleared.
#include <cstdio>

static int g_stringCtors = 0;
class CCharString { public:
    const char* m_text;
    CCharString() : m_text("") { ++g_stringCtors; }
};
struct CActivity {
    CCharString Name, Description, UnitOfMeasurement;
    unsigned char NumDecPlacesToShow; bool LowerIsBetter;
    CActivity() : NumDecPlacesToShow(0), LowerIsBetter(false) {}
};

int main()
{
    CActivity a;
    if (g_stringCtors != 3 || a.NumDecPlacesToShow != 0 || a.LowerIsBetter || a.Name.m_text[0] != 0) { std::printf("0055deb0_TEST FAIL\n"); return 1; }
    std::printf("0055deb0_TEST PASS\n");
    return 0;
}