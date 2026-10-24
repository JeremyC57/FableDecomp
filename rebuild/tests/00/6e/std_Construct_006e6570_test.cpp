#include <cstdio>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __cdecl
#endif
static int g_extCopies = 0;   // out-of-line copy constructors the retail body calls
static int g_links = 0;       // CBaseIntelligentPointer::SetPItem calls
namespace N006e6570 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString() { std::memset(this, 0, sizeof(CCharString)); }
    CCharString(const CCharString& other) { std::memcpy(this, &other, sizeof(CCharString)); ++g_extCopies; }
};
struct CConversationLine {                       // PDB sizeof 0x10
    long        PersonSpeaking;                  // +0x00
    long        PersonSpokenTo;                  // +0x04
    CCharString LineToSay;                       // +0x08
    bool        ShowSubtitle;                    // +0x0C
};

void __fastcall _Construct(CConversationLine* p, const CConversationLine& value)
{
    if (p != 0)
        new (p) CConversationLine(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N006e6570;
    CConversationLine src;
    src.PersonSpeaking = 1;
    src.PersonSpokenTo = 2;
    src.LineToSay.m_pData = reinterpret_cast<void*>(17);
    src.ShowSubtitle = true;
    CConversationLine* slot = reinterpret_cast<CConversationLine*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 1 && g_links == 0;
    if (!(slot->PersonSpeaking == src.PersonSpeaking)) ok = false;
    if (!(slot->PersonSpokenTo == src.PersonSpokenTo)) ok = false;
    if (!(slot->LineToSay.m_pData == src.LineToSay.m_pData)) ok = false;
    if (!(slot->ShowSubtitle == src.ShowSubtitle)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 1 || g_links != 0) ok = false;
    if (!ok) { std::printf("006e6570_TEST FAIL\n"); return 1; }
    std::printf("006e6570_TEST PASS\n");
    return 0;
}