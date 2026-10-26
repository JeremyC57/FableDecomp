#include <cstdio>

class CPlayerManager { public: long main; long GetMainPlayerNumber() { return main; } };
class CPlayer { public: unsigned char pad[0x10]; CPlayerManager* PlayerManager; unsigned char pad2[0x14]; long Number;
    bool IsMainPlayer() { return PlayerManager->GetMainPlayerNumber() == Number; } };
int main()
{
    CPlayerManager m; m.main = 2; CPlayer p; p.PlayerManager = &m; p.Number = 2;
    if (!p.IsMainPlayer()) { std::printf("00487520_TEST FAIL\n"); return 1; }
    p.Number = 3; if (p.IsMainPlayer()) { std::printf("00487520_TEST FAIL\n"); return 1; }
    std::printf("00487520_TEST PASS\n");
    return 0;
}