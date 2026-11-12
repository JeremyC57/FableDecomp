// Behaviour test for 0x00489D40 CPlayer::CreateCharacter: a standalone model of the decision flow (holy site / debug start
// position, co-op offset, empty def name falls back to the player def's CharacterDef, main player sets the spawn status).
#include <cstdio>
#include <cstring>

struct Vec { float X, Y, Z; };
struct Model {
    bool siteFound; Vec sitePos;
    bool overrideSet; bool overrideOnMap; Vec overridePos;
    bool isMain; bool mainValid; bool mainHasCreature; Vec mainPos;
    char defName[16]; long playerDefCharacter;
    // outputs
    Vec placed; char usedName[16]; int spawnStatus; bool created; bool upright;
};

static bool CreateCharacter(Model& m)
{
    if (!m.siteFound && !m.overrideSet) return false;
    Vec pos; bool upright = false;
    if (m.overrideSet && m.overrideOnMap) { pos = m.overridePos; upright = true; }
    else pos = m.sitePos;
    if (!m.isMain) {
        const Vec& p = (m.mainValid && m.mainHasCreature) ? m.mainPos : m.sitePos;
        Vec q = { p.X + 1.0f, p.Y + 1.0f, p.Z };
        pos = q;
    }
    char name[16];
    std::strcpy(name, m.defName);
    if (name[0] == 0) { name[0] = (char)m.playerDefCharacter; name[1] = 0; }
    std::strcpy(m.usedName, name);
    m.placed = pos; m.upright = upright; m.created = true;
    if (m.isMain) m.spawnStatus = 1;
    return true;
}

int main()
{
    Model a; std::memset(&a, 0, sizeof a);
    if (CreateCharacter(a)) { std::printf("00489d40_TEST FAIL\n"); return 1; }
    Model b; std::memset(&b, 0, sizeof b);
    b.siteFound = true; b.sitePos.X = 5; b.sitePos.Y = 6; b.sitePos.Z = 7; b.isMain = true; std::strcpy(b.defName, "HERO");
    if (!CreateCharacter(b) || b.placed.X != 5 || b.spawnStatus != 1 || std::strcmp(b.usedName, "HERO") != 0 || b.upright) {
        std::printf("00489d40_TEST FAIL\n"); return 1; }
    Model c; std::memset(&c, 0, sizeof c);
    c.overrideSet = true; c.overrideOnMap = true; c.overridePos.X = 1; c.mainValid = true; c.mainHasCreature = true;
    c.mainPos.X = 10; c.mainPos.Y = 20; c.mainPos.Z = 30; c.playerDefCharacter = 'A';
    if (!CreateCharacter(c) || c.placed.X != 11 || c.placed.Y != 21 || c.placed.Z != 30 || c.spawnStatus != 0 ||
        std::strcmp(c.usedName, "A") != 0 || !c.upright) { std::printf("00489d40_TEST FAIL\n"); return 1; }
    std::printf("00489d40_TEST PASS\n");
    return 0;
}