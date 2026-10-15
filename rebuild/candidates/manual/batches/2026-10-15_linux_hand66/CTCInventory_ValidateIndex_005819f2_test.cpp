#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
typedef long long Int64;
typedef unsigned long long UInt64;
#else
typedef __int64 Int64;
typedef unsigned __int64 UInt64;
#endif
// Behaviour test for 005819f2: ValidateIndex evaluates the world -> main player -> item list count once and returns the index unchanged.
static int g_counted = 0;
struct CItemList { unsigned long GetCount() { ++g_counted; return 8; } };
struct CPlayer { long _pad[3]; CItemList* m_pItems; };
static CItemList g_items;
static CPlayer g_player;
struct CWorld { CPlayer* GetMainPlayer() { return &g_player; } };
static CWorld g_world;
static CWorld* GetWorld() { return &g_world; }
struct CTCInventory { long ValidateIndex(long index); };
long CTCInventory::ValidateIndex(long index)
{
    GetWorld()->GetMainPlayer()->m_pItems->GetCount();
    return index;
}
int main()
{
    g_player.m_pItems = &g_items;
    CTCInventory inv;
    if (inv.ValidateIndex(17) != 17 || g_counted != 1 || inv.ValidateIndex(-1) != -1 || g_counted != 2) { std::printf("005819f2_TEST FAIL\n"); return 1; }
    std::printf("005819f2_TEST PASS\n");
    return 0;
}
