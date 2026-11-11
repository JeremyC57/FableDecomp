#include <stdio.h>
#include <string.h>

#define FABLE_RETAIL_CWIDESTRING_NOSTRINGS
#include "fable_string.h"

long CWideString::NoStrings = 0;                               // 0x013BCA20 ?NoStrings@CWideString@@1JA (protected static)
struct NoStringsProbe : CWideString { static long Count() { return NoStrings; } };

namespace
{
    CWideStringData g_storage;
    int g_allocationCalls = 0;
    const wchar_t* g_observedText = 0;
    long g_observedLength = 0;
}

CWideStringData* CWideString::AllocStringData(
    const wchar_t* text,
    long length)
{
    ++g_allocationCalls;
    g_observedText = text;
    g_observedLength = length;
    return &g_storage;
}

CWideString::~CWideString()
{
    --NoStrings;
}

static CWideStringData* Storage(const CWideString& value)
{
    return *reinterpret_cast<CWideStringData* const*>(&value);
}

int main()
{
    memset(&g_storage, 0, sizeof(g_storage));

    {
        CWideString nullValue(static_cast<const wchar_t*>(0));
        if (Storage(nullValue) != 0 || g_allocationCalls != 0)
            return 1;
    }

    {
        CWideString emptyValue(L"");
        if (Storage(emptyValue) != 0 || g_allocationCalls != 0)
            return 2;
    }

    {
        CWideString value(L"names.bin");
        if (Storage(value) != &g_storage || g_allocationCalls != 1)
            return 3;
        if (
            g_observedText == 0 ||
            wcscmp(g_observedText, L"names.bin") != 0 ||
            g_observedLength != -1)
        {
            return 4;
        }
    }

    if (NoStringsProbe::Count() != 0)
        return 5;

    puts("FABLETLC_WIDE_LITERAL_CONSTRUCTOR_BEHAVIOR PASS");
    return 0;
}
