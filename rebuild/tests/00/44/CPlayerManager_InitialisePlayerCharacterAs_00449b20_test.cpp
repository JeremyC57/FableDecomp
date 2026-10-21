// behaviour test for 0x00449B20
#include <stdio.h>
class CCharString { public: int id; };
class CPlayer {
public:
    int as;
    void InitCharacterAs(const CCharString& name) { as = name.id; }
};
class CPlayerManager {
public:
    CPlayer players[4];
    CPlayer& GetPlayer(long number) { return players[number]; }
    void InitialisePlayerCharacterAs(long number, const CCharString& name);
};
void CPlayerManager::InitialisePlayerCharacterAs(long number, const CCharString& name)
{
    GetPlayer(number).InitCharacterAs(name);
}
int main()
{
    CPlayerManager m; for (int i = 0; i < 4; ++i) m.players[i].as = 0;
    CCharString n; n.id = 77; m.InitialisePlayerCharacterAs(2, n);
    int ok = m.players[2].as == 77 && m.players[0].as == 0 && m.players[1].as == 0;
    printf(ok ? "00449b20_TEST PASS\n" : "00449b20_TEST FAIL\n");
    return ok ? 0 : 1;
}