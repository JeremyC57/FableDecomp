// behaviour test for 0x0042F70B
#include <stdio.h>
bool g_bCompileFrontendDefinitions;
static int g_calls[4]; static int g_n;
class CNewFrontendGameComponent {
public:
    static void __fastcall CompileDefs();
    static void __fastcall CompileDefsPlatformSpecific(bool frontend);   // 0x0042F5A9
};
void __fastcall CNewFrontendGameComponent::CompileDefs()
{
    if (g_bCompileFrontendDefinitions)
        CompileDefsPlatformSpecific(true);
    CompileDefsPlatformSpecific(false);
}
void __fastcall CNewFrontendGameComponent::CompileDefsPlatformSpecific(bool frontend) { g_calls[g_n++] = frontend ? 1 : 0; }
int main()
{
    int ok = 1;
    g_bCompileFrontendDefinitions = false; g_n = 0; CNewFrontendGameComponent::CompileDefs();
    ok &= g_n == 1 && g_calls[0] == 0;
    g_bCompileFrontendDefinitions = true; g_n = 0; CNewFrontendGameComponent::CompileDefs();
    ok &= g_n == 2 && g_calls[0] == 1 && g_calls[1] == 0;
    printf(ok ? "0042f70b_TEST PASS\n" : "0042f70b_TEST FAIL\n");
    return ok ? 0 : 1;
}