// Behaviour test for 0x009a4fb0: CSystemManager::IsApplicationInputFocus returns the bool at +0x0A and nothing else.
#include <cstdio>
#include <cstring>
struct SM { unsigned char _pad00[0x8]; bool WindowsQuit; bool ApplicationActive; bool ApplicationHasInputFocus;
    bool IsApplicationInputFocus() { return ApplicationHasInputFocus; } };
int main()
{
    SM s; std::memset(&s, 1, sizeof s); s.ApplicationHasInputFocus = false;
    bool ok = !s.IsApplicationInputFocus() && (unsigned char*)&s.ApplicationHasInputFocus - (unsigned char*)&s == 0xa;
    std::memset(&s, 0, sizeof s); s.ApplicationHasInputFocus = true;
    ok = ok && s.IsApplicationInputFocus();
    if (!ok) { std::printf("009a4fb0_TEST FAIL\n"); return 1; }
    std::printf("009a4fb0_TEST PASS\n"); return 0;
}
