#include <stdio.h>
// Behaviour test for 0x009BD090 CXMVPlayer::LoadXMVCode: links against the source object; the static loader is stubbed.
static int g_loads = 0;
class CXMVPlayer {
public:
    unsigned char _pad00[0x54];
    long LocalXMVPlayerRefCount;
    static void __fastcall StaticLoadXMVCode();
    void LoadXMVCode();
};
void __fastcall CXMVPlayer::StaticLoadXMVCode() { ++g_loads; }
int main()
{
    static CXMVPlayer player; player.LocalXMVPlayerRefCount = 4;
    player.LoadXMVCode();
    if (player.LocalXMVPlayerRefCount != 5 || g_loads != 1) { printf("009bd090_TEST FAIL first\n"); return 1; }
    player.LoadXMVCode();
    if (player.LocalXMVPlayerRefCount != 6 || g_loads != 2) { printf("009bd090_TEST FAIL second\n"); return 1; }
    printf("009bd090_TEST PASS\n");
    return 0;
}
