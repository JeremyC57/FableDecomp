#include <cstdio>

struct CTCTavernGame { int id; };
struct CThing { unsigned char Dead : 1; unsigned char _flags : 7; };

static int g_getCalls = 0;
struct CIntelligentPointer_CThing {
    CThing* m_pItem;
    CThing* GetPItem() { ++g_getCalls; return m_pItem; }
};

struct CTCGamePlayer {
    CTCTavernGame*             Game;
    CIntelligentPointer_CThing GameTable;
    CTCTavernGame* GetGame()
    {
        if (GameTable.GetPItem()) {
            if (!GameTable.GetPItem()->Dead)
                return Game;
        }
        return 0;
    }
};

int main()
{
    CTCTavernGame game = { 3 };
    CThing table; table.Dead = 0; table._flags = 0;
    CTCGamePlayer p; p.Game = &game; p.GameTable.m_pItem = 0;
    if (p.GetGame() != 0 || g_getCalls != 1) { std::printf("005edc90_TEST FAIL\n"); return 1; }
    p.GameTable.m_pItem = &table;
    if (p.GetGame() != &game || g_getCalls != 3) { std::printf("005edc90_TEST FAIL\n"); return 1; }
    table.Dead = 1;
    if (p.GetGame() != 0) { std::printf("005edc90_TEST FAIL\n"); return 1; }
    std::printf("005edc90_TEST PASS\n");
    return 0;
}