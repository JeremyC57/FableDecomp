// Behaviour test for 0x007C9E70 CTCCreatureGeneratorCreator::GetClassTypeName (NF417 re-form): links the row object (the virtual and the static's pick-any copy come from it) and
// asserts what reaches CCharString(const char*, long): retail's text "CTCCreatureGeneratorCreator", length -1, one constructor call, built straight into
// the caller's own hidden return slot, called through a const reference.
#include <stdio.h>
#include <string.h>

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();
};

static const char* g_seenText = 0;
static long g_seenLength = 0;
static int g_ctorCount = 0;
CCharString::CCharString(const char* text, long length) { PStringData = this; g_seenText = text; g_seenLength = length; ++g_ctorCount; }
CCharString::~CCharString() {}

class CTCCreatureGeneratorCreator
{
public:
    virtual CCharString GetClassTypeName() const;           // definition linked from the row object
};

int main(void)
{
    {
        CTCCreatureGeneratorCreator tc;
        const CTCCreatureGeneratorCreator& ref = tc;
        CCharString r = ref.GetClassTypeName();
        if (g_ctorCount != 1 || r.PStringData != (void*)&r || g_seenLength != -1 || g_seenText == 0 || strcmp(g_seenText, "CTCCreatureGeneratorCreator") != 0) {
            printf("007c9e70_TEST FAIL ctors=%d len=%ld text=%s\n", g_ctorCount, g_seenLength, g_seenText ? g_seenText : "(null)");
            return 1;
        }
    }
    printf("007c9e70_TEST PASS\n");
    return 0;
}
