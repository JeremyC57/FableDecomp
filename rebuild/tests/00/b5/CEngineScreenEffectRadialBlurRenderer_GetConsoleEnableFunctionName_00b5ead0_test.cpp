// Behaviour test for 0x00B5EAD0 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "EnableScreenEffectRadialBlurRenderer", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// CEngineScreenEffectRadialBlurRenderer::GetConsoleEnableFunctionName() -- returns "EnableScreenEffectRadialBlurRenderer" as a CCharString
// Leaf: ?GetConsoleEnableFunctionName@CEngineScreenEffectRadialBlurRenderer@@UAE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5EAD0: the hidden result is constructed through CCharString(const char*, long) 0x0099EBF0
//   with length -1, its address returned in eax.
// Body port (NF321, 2026-11-23): defined on the real class of engine/CEngineScreenEffectRadialBlurRenderer.h
//   (FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER; was ?GetConsoleEnableFunctionName@CEngineScreenEffectRadialBlurRenderer_Methods@@...
//   over a local CCharString model); the override is declared by the add-only opt-in
//   FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_GETCONSOLEENABLEFUNCTIONNAME_DECL (both PDB name sets; the RTTI placement names it
//   here), CCharString is fable_string.h's (through engine/CEngineComponent.h), as the landed water / outline-glow overrides.
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_GETCONSOLEENABLEFUNCTIONNAME_DECL
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"



static const char* g_text = 0;
static long g_len = 0;
static int g_calls = 0;
static const void* g_slot = 0;
static char g_storage[1024];
CCharString::CCharString(const char* s, long n) { g_text = s; g_len = n; ++g_calls; g_slot = this; }
CCharString::~CCharString() {}
int main(void)
{
    const void* where = 0;
    {
        CCharString r = ((CEngineScreenEffectRadialBlurRenderer*)g_storage)->CEngineScreenEffectRadialBlurRenderer::GetConsoleEnableFunctionName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "EnableScreenEffectRadialBlurRenderer") != 0 || g_slot != where) {
            printf("FAIL_00b5ead0 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("RADIALBLUR_GETNAME_OK\n");
    return 0;
}
