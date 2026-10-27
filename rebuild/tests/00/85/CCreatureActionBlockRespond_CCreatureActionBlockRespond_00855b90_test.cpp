// Behaviour test for 0x00855B90 CCreatureAction_BlockRespond::CCreatureAction_BlockRespond(creature, hitPos, blow): the
// base is built with the creature as both owner and target and action code 150; UseHitPos true, HitPos copied,
// FaceTargetAtStart false.
#include <cstdio>

class Creature { public: int id; };
class C3DVector { public: float X, Y, Z; };

class BlockBase {
public:
    Creature* owner; Creature* target; long actionCode; long blowStrength;
    BlockBase(Creature& o, Creature& t, long code, long blow) : owner(&o), target(&t), actionCode(code), blowStrength(blow) { }
    virtual ~BlockBase() { }
};
class BlockRespond : public BlockBase {
public:
    bool UseHitPos; C3DVector HitPos; bool FaceTargetAtStart;
    BlockRespond(Creature& c, const C3DVector& hit, long blow)
        : BlockBase(c, c, 150, blow), UseHitPos(true), HitPos(hit), FaceTargetAtStart(false) { }
};

int main()
{
    Creature c; c.id = 7;
    C3DVector hit; hit.X = 1.5f; hit.Y = -2.0f; hit.Z = 3.25f;
    BlockRespond r(c, hit, 42);
    if (r.owner != &c || r.target != &c || r.actionCode != 150 || r.blowStrength != 42) { std::printf("00855b90_TEST FAIL\n"); return 1; }
    if (!r.UseHitPos || r.FaceTargetAtStart) { std::printf("00855b90_TEST FAIL\n"); return 1; }
    if (r.HitPos.X != 1.5f || r.HitPos.Y != -2.0f || r.HitPos.Z != 3.25f) { std::printf("00855b90_TEST FAIL\n"); return 1; }
    std::printf("00855b90_TEST PASS\n");
    return 0;
}