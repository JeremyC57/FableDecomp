// Behaviour test for 0x009207A0 CAIStateGroup_CreatureSeekEnemyUnderground::IsRunnable (model copy over stubs).
#include <cstdio>
#include <math.h>
class C3DVector { public: float X, Y, Z; };
class CThing;
struct CTCEnemySource { CThing* enemy; CThing* GetEnemy() { return enemy; } };
struct CTCUndergroundMover { float range; float GetSurfaceRange() { return range; } };
class CThing { public:
    C3DVector pos; unsigned char m_flags91; CTCEnemySource* source; CTCUndergroundMover* mover;
    const C3DVector& GetPosition() const { return pos; }
    template<class T> bool GetInterface(T** out, long type)
    {
        if (type == 8) { *out = (T*)source; return true; }
        if (type == 0x34 && mover) { *out = (T*)mover; return true; }
        return false;
    }
};
struct CIntelligentPointerThing { CThing* item; CIntelligentPointerThing() : item(0) {} CIntelligentPointerThing& operator=(CThing* t) { item = t; return *this; } };
class CAIBrain { public: CThing* PCreature; };
class CAIStateGroupBase { public: CAIBrain* PBrain; };
struct CAIStateGroup_CreatureSeekEnemyUnderground : CAIStateGroupBase {
    CIntelligentPointerThing PEnemy;
    bool IsRunnable();
};
static inline float XYDistance(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    return (float)sqrt(dx * dx + dy * dy);
}

bool CAIStateGroup_CreatureSeekEnemyUnderground::IsRunnable()
{
    CTCEnemySource* source;
    PBrain->PCreature->GetInterface(&source, 8);
    CThing* enemy = source->GetEnemy();
    if (enemy != 0 && !(enemy->m_flags91 & 1)) {
        CThing* creature = PBrain->PCreature;
        CTCUndergroundMover* mover;
        if (creature->GetInterface(&mover, 0x34)) {
            if (XYDistance(enemy->GetPosition(), creature->GetPosition()) > mover->GetSurfaceRange() * 0.5f) {
                PEnemy = enemy;
                return true;
            }
            return false;
        }
    }
    return true;
}

static int fail(int n) { std::printf("009207a0_TEST FAIL (%d)\n", n); return 1; }
int main()
{
    CThing enemy; enemy.pos.X = 3; enemy.pos.Y = 4; enemy.pos.Z = 50; enemy.m_flags91 = 0;
    CTCEnemySource source; source.enemy = &enemy;
    CTCUndergroundMover mover; mover.range = 8.0f;
    CThing me; me.pos.X = 0; me.pos.Y = 0; me.pos.Z = 0; me.source = &source; me.mover = &mover;
    CAIBrain brain; brain.PCreature = &me;
    CAIStateGroup_CreatureSeekEnemyUnderground g; g.PBrain = &brain;
    if (!g.IsRunnable() || g.PEnemy.item != &enemy) return fail(1);          // 5 > 8 * 0.5
    mover.range = 12.0f; g.PEnemy.item = 0;
    if (g.IsRunnable() || g.PEnemy.item != 0) return fail(2);               // 5 < 6
    source.enemy = 0;
    if (!g.IsRunnable()) return fail(3);                                     // nothing to seek: true
    source.enemy = &enemy; enemy.m_flags91 = 1;
    if (!g.IsRunnable()) return fail(4);
    enemy.m_flags91 = 0; me.mover = 0;
    if (!g.IsRunnable()) return fail(5);
    std::printf("009207a0_TEST PASS\n");
    return 0;
}