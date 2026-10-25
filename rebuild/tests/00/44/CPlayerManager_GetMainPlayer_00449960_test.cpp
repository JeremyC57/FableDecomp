// Behaviour test for CPlayerManager::GetMainPlayer() const (0x00449960): forwards MainPlayer to the matching GetPlayer
// overload. The source is copied verbatim; the GetPlayer overloads are stubs that record which one ran.
#include <stdio.h>
class CPlayer { public: long Number; };
static CPlayer g_players[3];
static int g_constCalls = 0, g_mutableCalls = 0;
// ---- source (verbatim) ----
// CPlayerManager::GetMainPlayer() const -- the main player's record: GetPlayer(MainPlayer)
// Leaf: ?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00449960, 10 bytes (`mov eax,[ecx+1Ch] ; push eax ; call ; ret`), thiscall.
// Identity: Ego_r.pdb has both overloads, ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ and
//   ?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ; retail keeps two byte-identical bodies (no identical-code folding):
//   0x00449960 calls the landed const GetPlayer(long) 0x00449910 and 0x00449970 the landed non-const GetPlayer(long)
//   0x004498C0, so each takes the matching overload; this one calls the const GetPlayer(long) 0x00449910 (the twin is 0x00449970).
//   The previous generated Catch_0040d959 stub carried the propagated manifest label "Catch@0040d959".
// Layout: Ego_r.pdb CPlayerManager MainPlayer +0x20 -> retail +0x1c (Players is STLport's 12-byte vector); local model as in
//   the landed GetPlayer rows (engine/CPlayerManager.h carries the PDB offsets).
class CPlayerManager {
public:
    unsigned char _pad00[0x1c];                 // vptr slot, Component, DefinitionManager, Players, PlayerNeutral
    long          MainPlayer;                   // +0x1c
    CPlayer&       GetPlayer(long number);         // 0x004498C0
    const CPlayer& GetPlayer(long number) const;   // 0x00449910
    CPlayer&       GetMainPlayer();
    const CPlayer& GetMainPlayer() const;
};

const CPlayer& CPlayerManager::GetMainPlayer() const
{
    return GetPlayer(MainPlayer);
}
// ---- stubs ----
CPlayer& CPlayerManager::GetPlayer(long number) { ++g_mutableCalls; return g_players[number]; }
const CPlayer& CPlayerManager::GetPlayer(long number) const { ++g_constCalls; return g_players[number]; }
int main()
{
    for (int i = 0; i < 3; ++i) g_players[i].Number = 10 + i;
    const CPlayerManager* manager = 0;
    static CPlayerManager storage;
    storage.MainPlayer = 2;
    manager = &storage;
    const CPlayer& p = manager->GetMainPlayer();
    int ok = &p == &g_players[2] && p.Number == 12 && g_constCalls == 1 && g_mutableCalls == 0;
    printf(ok ? "00449960_TEST PASS\n" : "00449960_TEST FAIL\n");
    return ok ? 0 : 1;
}
