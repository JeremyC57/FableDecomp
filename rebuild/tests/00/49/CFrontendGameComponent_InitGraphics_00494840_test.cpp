// Behaviour test for 0x00494840 CFrontendGameComponent::InitGraphics (private) on engine/CFrontendGameComponent.h
// (FABLE_RETAIL_CFRONTENDGAMECOMPONENT_INITGRAPHICS), engine/CCountedPointer.h (the row's three size-body opt-ins) and
// engine/CFontManager.h. The body is the row's; the private member function and MainFont are reached with the explicit-instantiation
// access idiom (C++03 14.7.2/8), as the landed ClearString test 0x004944C0 does. Stubs: CFontManager::GetFont, PFontManager,
// CCharString(const char*, long) / ~CCharString. Cases: MainFont empty -> takes the font (the temporary's reference dropped); MainFont
// holding the last reference to another font -> that font is destroyed and the new one held; the same count block again -> kept, no
// destroy; always returns true; the name is ENG_ARIAL_16 with length -1 and is destroyed once per call.
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_RELEASEDATA_SIZE_BODY
#define FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_SHAREDATA_SIZE_BODY
#define FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_CONVERTING_ASSIGN_SIZE_BODY
#include "engine/CCountedPointer.h"
#include "fable_string.h"
#include "engine/CFontManager.h"
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_INITGRAPHICS
#include "engine/CFrontendGameComponent.h"

bool CFrontendGameComponent::InitGraphics()
{
    MainFont = PFontManager->GetFont("ENG_ARIAL_16");
    return true;
}

typedef bool (CFrontendGameComponent::*InitGraphicsMember)();
typedef CCountedPointer<const CFontBank> CFrontendGameComponent::*MainFontMember;
struct InitGraphicsTag { friend InitGraphicsMember GetInitGraphics(InitGraphicsTag); };
struct MainFontTag { friend MainFontMember GetMainFont(MainFontTag); };
template <InitGraphicsMember M> struct InitGraphicsAccess { friend InitGraphicsMember GetInitGraphics(InitGraphicsTag) { return M; } };
template <MainFontMember M> struct MainFontAccess { friend MainFontMember GetMainFont(MainFontTag) { return M; } };
template struct InitGraphicsAccess<&CFrontendGameComponent::InitGraphics>;
template struct MainFontAccess<&CFrontendGameComponent::MainFont>;

static char g_name[64]; static long g_len = 0; static int g_strDtors = 0, g_destroyed = 0;
static void* g_lastDestroyed = 0;
CCharString::CCharString(const char* text, long length) { strncpy(g_name, text, 63); g_len = length; }
CCharString::~CCharString() { ++g_strDtors; }
static void __fastcall DestroyFont(void* object) { ++g_destroyed; g_lastDestroyed = object; }
static CFontManager* g_managerObject = 0;
CFontManager* PFontManager = 0;
static int g_fontA, g_fontB;
static CCPPointerInfo* g_next = 0;              // the count block GetFont hands out (its reference is the returned temporary's)
CCountedPointer<CFontBank> CFontManager::GetFont(const CCharString&)
{
    CCountedPointer<CFontBank> result;
    result.Data = reinterpret_cast<CFontBank*>(g_next->Data);
    result.Info = g_next;
    ++g_next->RefCount;                         // the temporary's reference
    return result;
}

static int g_fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("00494840 FAIL %s\n", what); g_fail = 1; } }
int main()
{
    static unsigned char managerBytes[16];
    PFontManager = reinterpret_cast<CFontManager*>(managerBytes);
    static unsigned char storage[sizeof(CFrontendGameComponent)];
    memset(storage, 0, sizeof(storage));
    CFrontendGameComponent& c = *reinterpret_cast<CFrontendGameComponent*>(storage);
    CCountedPointer<const CFontBank>& font = c.*GetMainFont(MainFontTag());
    Check(reinterpret_cast<unsigned char*>(&font) - storage == 0x1e04, "MainFont at +0x1E04");
    // case 1: empty MainFont takes font A; the temporary's reference is dropped (one left, held by MainFont)
    CCPPointerInfo* a = new CCPPointerInfo(0, DestroyFont, &g_fontA);
    g_next = a;
    Check((c.*GetInitGraphics(InitGraphicsTag()))() == true, "returns true");
    Check(strcmp(g_name, "ENG_ARIAL_16") == 0 && g_len == -1 && g_strDtors == 1, "font name built and destroyed");
    Check(font.Data == reinterpret_cast<const CFontBank*>(&g_fontA) && font.Info == a && a->RefCount == 1 && g_destroyed == 0, "font A held");
    // case 2: the same count block again: kept, count unchanged, nothing destroyed
    Check((c.*GetInitGraphics(InitGraphicsTag()))() == true && font.Info == a && a->RefCount == 1 && g_destroyed == 0, "same font kept");
    // case 3: font B replaces A, whose last reference goes: A destroyed once, B held
    CCPPointerInfo* b = new CCPPointerInfo(0, DestroyFont, &g_fontB);
    g_next = b;
    Check((c.*GetInitGraphics(InitGraphicsTag()))() == true, "returns true again");
    Check(g_destroyed == 1 && g_lastDestroyed == &g_fontA, "font A destroyed");
    Check(font.Data == reinterpret_cast<const CFontBank*>(&g_fontB) && font.Info == b && b->RefCount == 1 && g_strDtors == 3, "font B held");
    if (g_fail) return 1;
    printf("00494840_TEST PASS\n");
    return 0;
}
