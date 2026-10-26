// Behaviour test for CAIStateGroup_RangedCombatUsePreferredFirePoint::IsRunnable (0x0095D590): same logic over mocks.
#include <cstdio>
#include <cstdlib>
class C3DVector { public: float x, y, z; };
class CThing { public: bool dead; C3DVector pos; CThing* reservedBy; CThing() : dead(false), reservedBy(0) { pos.x = pos.y = pos.z = 0; } };
static float Dist2(const C3DVector& a, const C3DVector& b) { float dx = a.x - b.x, dy = a.y - b.y, dz = a.z - b.z; return dx * dx + dy * dy + dz * dz; }
struct CThingVector {
    CThing** _M_start; CThing** _M_finish; CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
};
static void SortFirePoints(CThing** first, CThing** last, C3DVector pos)
{
    for (CThing** i = first; i != last; ++i)
        for (CThing** j = i + 1; j != last; ++j)
            if (Dist2((*j)->pos, pos) < Dist2((*i)->pos, pos)) { CThing* t = *i; *i = *j; *j = t; }
}
struct CState {
    CThing* creature; bool ranged; CThing* enemySource; CThing* points[3]; int nPoints;
    CThing* PEnemy; CThing* PFirePoint; long TimeOutCounter;
    void GetFirePointList(CThingVector& v)
    {
        if (nPoints == 0) return;
        v._M_start = (CThing**)malloc(sizeof(CThing*) * nPoints);
        for (int i = 0; i < nPoints; ++i) v._M_start[i] = points[i];
        v._M_finish = v._M_end_of_storage = v._M_start + nPoints;
    }
    bool IsRunnable()
    {
        if (!ranged) return false;
        PEnemy = enemySource; PFirePoint = 0; TimeOutCounter = 0;
        if (PEnemy != 0 && !PEnemy->dead) {
            CThingVector points;
            GetFirePointList(points);
            SortFirePoints(points._M_start, points._M_finish, creature->pos);
            if (points.size() > 0) {
                PFirePoint = *points._M_start;
                PFirePoint->reservedBy = creature;
                return true;
            }
        }
        return false;
    }
};
int main()
{
    CThing creature, enemy, a, b, c;
    a.pos.x = 10; b.pos.x = 2; c.pos.x = 5;
    CState s; s.creature = &creature; s.ranged = true; s.enemySource = &enemy;
    s.points[0] = &a; s.points[1] = &b; s.points[2] = &c; s.nPoints = 3; s.TimeOutCounter = 9;
    bool ok = s.IsRunnable() && s.PFirePoint == &b && b.reservedBy == &creature && s.TimeOutCounter == 0;
    s.nPoints = 0; ok = ok && !s.IsRunnable() && s.PFirePoint == 0;
    s.nPoints = 3; enemy.dead = true; ok = ok && !s.IsRunnable();
    enemy.dead = false; s.ranged = false; s.PFirePoint = &a; ok = ok && !s.IsRunnable() && s.PFirePoint == &a;
    if (!ok) { std::printf("0095d590_TEST FAIL\n"); return 1; }
    std::printf("0095d590_TEST PASS\n");
    return 0;
}