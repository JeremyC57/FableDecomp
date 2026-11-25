// Behaviour test for NParticleEngine::CPSCLight::GetClassTypeName() const @ 0x00B1AE00 (linked with the row object).
// Asserts the string the row hands to CCharString::CCharString(char const*, int): retail pushes 0x0129F5E8 = "CPSCLight"
// with length -1, once, into the hidden return slot. (NF388: the HEAD row passed "Bounding Sphere" and its test only printed PASS.)
#include <stdio.h>
#include <string.h>
class CCharString {
public:
    CCharString(const char* s, long n);
    const char* Text() const { return m_p; }
private:
    char* m_p;
};
namespace NParticleEngine {
class CPSCLight {
public:
    virtual CCharString GetClassTypeName() const;
};
}
static const char* g_text = 0;
static long g_len = 0;
static int g_calls = 0;
static const void* g_slot = 0;
CCharString::CCharString(const char* s, long n) : m_p((char*)s) { g_text = s; g_len = n; ++g_calls; g_slot = this; }
int main(void)
{
    NParticleEngine::CPSCLight light;
    const NParticleEngine::CPSCLight& c = light;
    CCharString r = c.GetClassTypeName();
    if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "CPSCLight") != 0 || r.Text() != g_text || g_slot != (const void*)&r) {
        printf("FAIL_00b1ae00 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
        return 1;
    }
    printf("PASS_00b1ae00\n");
    return 0;
}
