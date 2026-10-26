extern unsigned char g_aggressiveEnabled;

class CPlayer;
extern "C" void __fastcall CPlayer_OnAggressiveChanged(CPlayer* player);

class CPlayer { public:
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