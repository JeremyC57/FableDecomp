#include <cstdio>

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);
    ~CCharString();
};
static const char* g_seenText = 0;
static long g_seenLength = 0;
static int g_ctorCount = 0;
CCharString::CCharString(const char* text, long length) { PStringData = this; g_seenText = text; g_seenLength = length; ++g_ctorCount; }
CCharString::~CCharString() {}

namespace NCreatureAction {
class CActionEventAddObject
{
public:
    virtual CCharString GetHook() const;
};
}

int main()
{
    {
        NCreatureAction::CActionEventAddObject* event = 0;   // `this` is never read
        CCharString hook = event->NCreatureAction::CActionEventAddObject::GetHook();
        if (g_ctorCount != 1 || hook.PStringData != &hook) { std::printf("BAD_IDENTITY\n"); return 1; }
        if (g_seenLength != -1) { std::printf("BAD_LEN\n"); return 1; }
        if (g_seenText != (const char*)0x0125C18C) { std::printf("BAD_TEXT\n"); return 1; }
    }
    std::printf("006922a0_TEST PASS\n");
    return 0;
}
