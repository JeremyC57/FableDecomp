// Behaviour test for CPlayer::PrepareForLoad 0x00487550: clear the player's hero log book, then reset its modes (in that order)
#include <cstdio>
static int g_order[4];
static int g_count = 0;
class CHeroLogBook {
public:
    CHeroLogBook() : Entries(5) {}
    void Clear() { Entries = 0; g_order[g_count++] = 1; }
    int Entries;
};
class CPlayer {
public:
    CPlayer() : PHeroLogBook_Data(0), ModeCount(3) {}
    void PrepareForLoad()
    {
        PHeroLogBook_Data->Clear();
        ResetModes();
    }
    void ResetModes() { ModeCount = 0; g_order[g_count++] = 2; }
    CHeroLogBook* PHeroLogBook_Data;
    int ModeCount;
};
int main()
{
    CHeroLogBook book;
    CPlayer player;
    player.PHeroLogBook_Data = &book;
    player.PrepareForLoad();
    if (g_count != 2 || g_order[0] != 1 || g_order[1] != 2 || book.Entries != 0 || player.ModeCount != 0) {
        std::printf("00487550_TEST FAIL\n");
        return 1;
    }
    std::printf("00487550_TEST PASS\n");
    return 0;
}