// behaviour test for 0x0042DBD8
#include <stdio.h>
static int g_released = 0;
class CXMVPlayer {
public:
    static void __fastcall StaticReleaseXMVCode();
};
void __fastcall CXMVPlayer::StaticReleaseXMVCode() { ++g_released; }
class CNewFrontendGameComponent {
public:
    unsigned char _pad00[0xb1];
    bool          XMVCodeLoaded;                         // +0xb1
    void ReleaseXMVCode();
};

void CNewFrontendGameComponent::ReleaseXMVCode()
{
    if (XMVCodeLoaded)
    {
        CXMVPlayer::StaticReleaseXMVCode();
        XMVCodeLoaded = false;
    }
}
int main()
{
    CNewFrontendGameComponent c; c.XMVCodeLoaded = true; int ok = 1;
    c.ReleaseXMVCode(); ok &= g_released == 1 && !c.XMVCodeLoaded;
    c.ReleaseXMVCode(); ok &= g_released == 1 && !c.XMVCodeLoaded;
    printf(ok ? "0042dbd8_TEST PASS\n" : "0042dbd8_TEST FAIL\n");
    return ok ? 0 : 1;
}