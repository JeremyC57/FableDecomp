// Behaviour test for 0x0054f370 (NF232, q12632) on the shared headers: NUISystem::CText::SetValue(const CCharString&, bool, bool), the
// row's body on fable_string.h (CCHARSTRING + GETCRC_INLINE), engine/NGameText_CDataBank.h (FindIndexBySymbolCRC), engine/CMainGameComponent.h
// (TEXTBANK), engine/CNewFrontendGameComponent.h and engine/NUISystem_CText.h. The text, the CText object, the two components and the banks
// are raw storage (constructors out of line; the row is called non-virtually; the protected statics are set through never-constructed subclasses); the forwarded (tag, text, keepSize) overload is the CText
// vtable's slot 143, here a test table whose slot 143 is a recording member of a test struct (same thiscall shape). The out-of-line
// callees (ToWideString, the CWideString constructors / destructor, CCRC::Calc, CBankFile::FindIndexBySymbolCRC) are stubs. `#define private public` (test only)
// around the front-end component header alone reaches its private static. One case per condition.
// Replaces the HEAD host model (local CCharString / CWideString / CDataBank / CMainGameComponent / CText and the ?g_ stand-in).
// The narrow text is an empty CCharString (null data): its inline GetCRC hashes the NullChar text, length 0.
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#include "fable_string.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGameComponent.h"
#include "engine/CCountedPointer.h"
#define private public
#include "engine/CNewFrontendGameComponent.h"
#undef private
#include "engine/NUISystem_CText.h"

namespace NUISystem {
void CText::SetValue(const CCharString& text, bool lookUpTag, bool keepSize)
{
    unsigned long tag = 0;
    if (lookUpTag) {
        if (CMainGameComponent::Get()) {
            tag = CMainGameComponent::Get()->GetTextBank().FindIndexBySymbolCRC(text.GetCRC());
        }
        else if (CNewFrontendGameComponent::Get()) {
            tag = CNewFrontendGameComponent::Get()->GetTextBank().FindIndexBySymbolCRC(text.GetCRC());
        }
    }
    SetValue(tag, text.ToWideString(), keepSize);
}
}

static int g_conversions = 0, g_wideDtors = 0, g_calcs = 0, g_finds = 0, g_forwards = 0;
static const void* g_calcData = 0; static unsigned long g_calcLength = 99, g_calcSeed = 99;
static const void* g_findBank = 0; static unsigned long g_findCrc = 0;
static unsigned long g_tag = 0xFFFFFFFF; static const CWideString* g_text = 0; static bool g_keep = false; static const void* g_self = 0;
const char CCharString::NullChar = 0;
CWideString::CWideString() : PStringData(0) {}
CWideString::CWideString(const CWideString&) : PStringData(0) {}
CWideString::~CWideString() { ++g_wideDtors; }
CWideString CCharString::ToWideString() const { ++g_conversions; return CWideString(); }
unsigned long FABLE_FASTCALL CCRC::Calc(unsigned long seed, const void* data, unsigned long byteCount)
{ ++g_calcs; g_calcSeed = seed; g_calcData = data; g_calcLength = byteCount; return 0x1234; }
unsigned long CBankFile::FindIndexBySymbolCRC(unsigned long crc) const { ++g_finds; g_findBank = this; g_findCrc = crc; return 0x40 + g_finds; }
CMainGameComponent* CMainGameComponent::PMainGameComponent = 0;
CNewFrontendGameComponent* CNewFrontendGameComponent::m_pNewFrontendGameComponent = 0;
// Protected statics reached through never-constructed subclasses.
struct MainAccess : CMainGameComponent { static void Set(CMainGameComponent* game) { PMainGameComponent = game; } };
struct NullAccess : CCharString { static const char* Null() { return &NullChar; } };

// Slot 143 of the CText vtable, SetValue(unsigned long, const CWideString&, bool): a recording member with the same thiscall shape.
struct Slot143 { void SetValue(unsigned long tag, const CWideString& text, bool keepSize)
    { ++g_forwards; g_self = this; g_tag = tag; g_text = &text; g_keep = keepSize; } };
static void* g_vtable[160];
static void* g_text_store[16];
static double g_main_store[4], g_front_store[(0x148 + 7) / 8], g_mainBank[8], g_frontBank[8], g_narrow_store[1];
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }
static void Reset() { g_conversions = g_wideDtors = g_calcs = g_finds = g_forwards = 0; g_tag = 0xFFFFFFFF; g_findBank = 0; g_text = 0; }

int main()
{
    void (Slot143::*slot)(unsigned long, const CWideString&, bool) = &Slot143::SetValue;
    memcpy(&g_vtable[143], &slot, sizeof(void*));
    g_text_store[0] = g_vtable;
    NUISystem::CText* ui = (NUISystem::CText*)g_text_store;
    const CCharString& text = *(const CCharString*)g_narrow_store;   // PStringData 0: the empty string
    *(void**)((unsigned char*)g_main_store + 0x14) = g_mainBank;      // PTextBank's object word
    *(void**)((unsigned char*)g_front_store + 0x60) = g_frontBank;    // m_pTextBank's object word
    // no look-up: tag 0, the text and keepSize forwarded, no conversion
    Reset(); MainAccess::Set((CMainGameComponent*)g_main_store);
    ui->NUISystem::CText::SetValue(text, false, true);
    Check(g_forwards == 1 && g_self == ui && g_tag == 0 && g_text != 0 && g_keep && g_conversions == 1 && g_wideDtors >= 1 && g_calcs == 0 && g_finds == 0, "no look-up: the wide copy forwarded, then destroyed");
    // look-up through the main game's bank: the narrow copy's CRC (seed 0, the empty text), its tag, the temporary destroyed
    Reset();
    ui->NUISystem::CText::SetValue(text, true, false);
    Check(g_calcs == 1 && g_calcSeed == 0 && g_calcData == NullAccess::Null() && g_calcLength == 0, "main: CRC of the text (seed 0, empty)");
    Check(g_finds == 1 && g_findBank == g_mainBank && g_findCrc == 0x1234, "main: the game's text bank");
    Check(g_forwards == 1 && g_tag == 0x41 && g_text != 0 && !g_keep && g_conversions == 1 && g_wideDtors >= 1, "main: tag and the wide copy forwarded");
    // no main game, front end running: the front end's bank
    Reset(); MainAccess::Set(0);
    CNewFrontendGameComponent::m_pNewFrontendGameComponent = (CNewFrontendGameComponent*)g_front_store;
    ui->NUISystem::CText::SetValue(text, true, true);
    Check(g_calcs == 1 && g_finds == 1 && g_findBank == g_frontBank && g_forwards == 1 && g_tag == 0x41 && g_keep, "front end: its text bank");
    // neither component: tag 0, no conversion
    Reset(); CNewFrontendGameComponent::m_pNewFrontendGameComponent = 0;
    ui->NUISystem::CText::SetValue(text, true, false);
    Check(g_calcs == 0 && g_finds == 0 && g_forwards == 1 && g_tag == 0 && g_conversions == 1, "no component: tag 0");
    printf(g_fails ? "0054f370_TEST FAIL\n" : "0054f370_TEST PASS\n");
    return g_fails ? 1 : 0;
}
