// Behaviour test for CTCBuyableHouse::GetSignPostPos (0x006BFCD0): a copy of the model's logic on stand-alone types.
#include <stdio.h>
#include <math.h>
struct V3 { float X,Y,Z; V3(){} V3(float x,float y,float z):X(x),Y(y),Z(z){} V3(const V3&p){X=p.X;Y=p.Y;Z=p.Z;}
  const V3 operator+(const V3& o) const { return V3(X+o.X,Y+o.Y,Z+o.Z); }
  const V3 operator*(float s) const { return V3(X*s,Y*s,Z*s); } };
static inline V3 AddOffset(const V3& offset, V3 position) { return position + offset; }
struct RH { V3 Up, Forward;
  void Rotate(const V3&, float turns) { if (turns == 0.5f) { Forward.X = -Forward.X; Forward.Y = -Forward.Y; } }
  const V3& GetForward() const { return Forward; } };
struct Thing { int defIndex; bool dead; bool door; V3 pos; RH rh; };
static float Ground(const V3& p) { return p.X * 0.0f + 7.0f; }
static float g_insideMinX;      // building occupies X >= g_insideMinX
static bool IsPointInside(const V3& p) { return p.X >= g_insideMinX; }
static bool GetSignPostPos(Thing* things, int n, int mainIndex, int dressIndex, bool main, V3& pos, RH* orientation)
{
    float side = main ? 2.0f : 4.0f;
    Thing* door = 0; Thing* signPost = 0;
    int index = main ? mainIndex : dressIndex;
    for (int k = 0; k < n; ++k) {
        Thing* t = &things[k];
        if (t && !t->dead) { if (t->door) door = t; else if (t->defIndex == index) signPost = t; }
    }
    if (signPost) { pos = signPost->pos; if (orientation) *orientation = signPost->rh; return true; }
    if (door) {
        for (long i = 0; i < 2; ++i) {
            RH set = door->rh;
            if (i) set.Rotate(V3(0.0f, 0.0f, 1.0f), 0.5f);
            V3 forward = set.GetForward();
            V3 candidate = AddOffset(V3(-forward.Y, forward.X, 0.0f) * side, set.GetForward() * 3.0f + door->pos);
            candidate.Z = Ground(candidate);
            if (orientation) *orientation = set;
            pos = candidate;
            if (!IsPointInside(pos)) return true;
        }
    }
    return false;
}
static bool near(float a, float b) { return fabs(a - b) < 1e-4f; }
int main()
{
    int fails = 0;
    Thing t[3];
    t[0].defIndex = 5; t[0].dead = false; t[0].door = true;  t[0].pos = V3(10,20,1); t[0].rh.Up = V3(0,0,1); t[0].rh.Forward = V3(1,0,0);
    t[1].defIndex = 9; t[1].dead = false; t[1].door = false; t[1].pos = V3(1,2,3);   t[1].rh.Up = V3(0,0,1); t[1].rh.Forward = V3(0,1,0);
    t[2].defIndex = 8; t[2].dead = true;  t[2].door = false; t[2].pos = V3(4,4,4);   t[2].rh = t[1].rh;
    V3 p; RH o;
    // existing main sign post wins
    if (!GetSignPostPos(t, 3, 9, 8, true, p, &o) || !near(p.X,1) || !near(p.Y,2) || !near(o.Forward.Y,1)) ++fails;
    // dressing post is dead -> door placement: 3 ahead (+X) and 4 to the left (+Y), ground height 7
    g_insideMinX = 100.0f;
    if (!GetSignPostPos(t, 3, 9, 8, false, p, &o) || !near(p.X,13) || !near(p.Y,24) || !near(p.Z,7) || !near(o.Forward.X,1)) ++fails;
    // first spot inside the building -> half turn: 3 behind, 4 to the other side
    g_insideMinX = 12.0f;
    if (!GetSignPostPos(t, 3, 9, 8, false, p, &o) || !near(p.X,7) || !near(p.Y,16) || !near(o.Forward.X,-1)) ++fails;
    // both spots inside -> false
    g_insideMinX = 0.0f;
    if (GetSignPostPos(t, 3, 9, 8, false, p, 0)) ++fails;
    // no door and no post -> false
    t[0].door = false;
    if (GetSignPostPos(t, 3, 9, 8, false, p, 0)) ++fails;
    if (fails) { printf("006bfcd0_TEST FAIL\n"); return 1; }
    printf("006bfcd0_TEST PASS\n");
    return 0;
}