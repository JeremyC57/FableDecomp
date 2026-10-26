// Behaviour test for CPersistContext_TransferableComponent::TransferIn (0x00403FC0): model and body verbatim from
// final.cpp, the traits TransferIn stubbed; checks the default value is false, the context/parser pass through and
// the value pointer is the component's Object.
#include <stdio.h>
#ifndef _MSC_VER
#define _snprintf snprintf
#endif
class CPersistContext;
class CStringParser;

template <class T> struct CPersistTraits {
    T DefaultVal;
    unsigned char Pad[3];
    void TransferIn(CPersistContext* context, CStringParser* parser, T* value);   // 0x00403FF0 for bool
};

struct CPersistContext_TransferableComponent {
    virtual void TransferOut(CPersistContext* context, void* out) {}
    virtual void TransferIn(CPersistContext* context, CStringParser* parser);
    bool* Object;                          // +0x4
};

static int g_calls = 0; static bool g_default = true; static CPersistContext* g_context = 0; static CStringParser* g_parser = 0;
static bool* g_value = 0;
template <> void CPersistTraits<bool>::TransferIn(CPersistContext* context, CStringParser* parser, bool* value)
{
    g_calls++; g_default = DefaultVal; g_context = context; g_parser = parser; g_value = value; *value = true;
}

void CPersistContext_TransferableComponent::TransferIn(CPersistContext* context, CStringParser* parser)
{
    CPersistTraits<bool> traits = CPersistTraits<bool>();
    traits.DefaultVal = bool();
    traits.TransferIn(context, parser, Object);
}

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    bool target = false;
    CPersistContext_TransferableComponent component; component.Object = &target;
    CPersistContext* context = (CPersistContext*)0x1234; CStringParser* parser = (CStringParser*)0x5678;
    component.TransferIn(context, parser);
    CHECK(g_calls == 1 && g_default == false && g_context == context && g_parser == parser && g_value == &target && target == true);
    CPersistContext_TransferableComponent* base = &component;
    base->TransferIn(0, 0);
    CHECK(g_calls == 2 && g_default == false && g_context == 0 && g_parser == 0 && g_value == &target);
    char line[64];
    _snprintf(line, sizeof(line), "00403fc0_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}