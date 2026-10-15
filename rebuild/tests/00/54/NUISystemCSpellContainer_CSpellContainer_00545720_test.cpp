// Behaviour test for 0x00545720: the ctor copies the def's position offset and releases its def reference.
#include <cstdio>
struct V2 { float X, Y; };
struct Def { int count; int destroyed; V2 off; void Release() { --count; if (count == 0) ++destroyed; } };
struct DefPtr { Def* p; explicit DefPtr(Def* d) : p(d) { ++p->count; } ~DefPtr() { p->Release(); } };
struct Spell { void* bs[3]; void* first; unsigned int second; V2 pos; Def* def;
  Spell(Def* d) : first(0), second(0), def(d) { bs[0] = bs[1] = bs[2] = 0; { DefPtr p(def); pos = p.p->off; } } };
int main()
{
    Def d = {1, 0, {3.5f, -2.0f}}; Spell s(&d);
    bool ok = s.pos.X == 3.5f && s.pos.Y == -2.0f && d.count == 1 && d.destroyed == 0 && !s.first && !s.bs[1];
    Def e = {0, 0, {1.0f, 1.0f}}; Spell t(&e); ok = ok && e.count == 0 && e.destroyed == 1 && t.pos.X == 1.0f;
    if (!ok) { std::printf("00545720_TEST FAIL\n"); return 1; }
    std::printf("00545720_TEST PASS\n");
    return 0;
}