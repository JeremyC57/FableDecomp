// Behaviour test for 0x007BF510 NThingShot::CTCShotDefinitionBase::OnCollision: a copy of the model's logic over plain stand-ins.
#include <cstdio>
#include <vector>
#include <utility>
#include <algorithm>
struct V { float X, Y, Z; V() {} V(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float GetSquaredMagnitude() const { return Z * Z + Y * Y + X * X; } };
static V operator-(const V& a, const V& b) { return V(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
struct Thing { bool Dead; V Pos; };
struct Trail { std::vector<std::pair<V, V> > pts; bool HasWeaponTrailPoints() const { return !pts.empty(); }
    bool GetLastProjectileWeaponTrailPoints(std::vector<std::pair<V, V> >& out) const { out = pts; return pts.size() >= 2; } };
struct Shot {
    Thing* Parent; Trail* trail; std::vector<Thing*> HitThingsList;
    V BestCollisionPos, BestCollisionNormal; Thing* PBestCollisionThing; int BestCollisionType;
    void OnCollision(int type, const V& position, const V& normal, Thing* thing)
    {
        if (thing && !thing->Dead && std::find(HitThingsList.begin(), HitThingsList.end(), thing) != HitThingsList.end())
            return;
        if (trail) {
            std::vector<std::pair<V, V> > points;
            if (trail->HasWeaponTrailPoints()) {
                if (trail->GetLastProjectileWeaponTrailPoints(points)) {
                    V tip = (points.end() - 2)->first;
                    if (BestCollisionType == 0 || (position - tip).GetSquaredMagnitude() < (BestCollisionPos - tip).GetSquaredMagnitude()) {
                        BestCollisionPos = position; BestCollisionNormal = normal; PBestCollisionThing = thing; BestCollisionType = type;
                    }
                }
            } else {
                PBestCollisionThing = 0; BestCollisionType = type; BestCollisionPos = Parent->Pos; BestCollisionNormal = V(0.0f, 0.0f, 1.0f);
            }
        }
    }
};
int main()
{
    Thing parent = { false, V(5, 6, 7) }, a = { false, V() }, b = { false, V() };
    Trail tr; tr.pts.push_back(std::make_pair(V(0, 0, 0), V())); tr.pts.push_back(std::make_pair(V(1, 1, 1), V()));
    Shot s; s.Parent = &parent; s.trail = &tr; s.PBestCollisionThing = 0; s.BestCollisionType = 0; s.BestCollisionPos = V(0, 0, 0);
    bool ok = true;
    s.OnCollision(2, V(10, 0, 0), V(0, 1, 0), &a);                 // first collision always taken
    ok = ok && s.PBestCollisionThing == &a && s.BestCollisionType == 2 && s.BestCollisionPos.X == 10.0f;
    s.OnCollision(3, V(2, 0, 0), V(1, 0, 0), &b);                  // nearer the tip (0,0,0): replaces
    ok = ok && s.PBestCollisionThing == &b && s.BestCollisionType == 3 && s.BestCollisionNormal.X == 1.0f;
    s.OnCollision(4, V(9, 0, 0), V(0, 0, 1), &a);                  // farther: kept
    ok = ok && s.PBestCollisionThing == &b && s.BestCollisionType == 3;
    s.HitThingsList.push_back(&a);
    s.OnCollision(5, V(0, 0, 0), V(0, 0, 1), &a);                  // already hit: ignored
    ok = ok && s.BestCollisionType == 3;
    a.Dead = true;
    s.OnCollision(5, V(0.5f, 0, 0), V(0, 0, 1), &a);               // dead thing skips the hit-list test
    ok = ok && s.BestCollisionType == 5 && s.PBestCollisionThing == &a;
    tr.pts.clear();
    s.OnCollision(6, V(1, 2, 3), V(1, 0, 0), &b);                  // no trail points: shooter position, up normal
    ok = ok && s.PBestCollisionThing == 0 && s.BestCollisionType == 6 && s.BestCollisionPos.Y == 6.0f && s.BestCollisionNormal.Z == 1.0f;
    if (!ok) { std::printf("007bf510_TEST FAIL\n"); return 1; }
    std::printf("007bf510_TEST PASS\n");
    return 0;
}