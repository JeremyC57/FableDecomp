extern unsigned char g_aggressiveEnabled;

struct CPlayer;
extern "C" void __fastcall CPlayer_OnAggressiveChanged(CPlayer* player);

struct CPlayer {
    char pad[0x217];
    unsigned char aggressive;
};

void __fastcall ToggleAggressiveMode(CPlayer* player)
{
    if (g_aggressiveEnabled) {
        player->aggressive = (player->aggressive == 0);
        CPlayer_OnAggressiveChanged(player);
    }
}