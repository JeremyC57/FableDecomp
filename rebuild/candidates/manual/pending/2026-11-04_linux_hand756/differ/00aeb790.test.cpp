// Behaviour test for CParticleEmitterManager::Create2DEmitter (0x00AEB790): a model of the row -- a 2D template is cloned into the
// active list at the screen position on the layer (oriented when a set is given); a non-2D template gives null.
#include <stdio.h>
#include <list>
struct Emitter { bool Is2D; float X, Y; int Layer; bool Oriented; };
static Emitter* Create2D(std::list<Emitter>& active, const Emitter& tmpl, float x, float y, int layer, bool orient)
{
    if (!tmpl.Is2D) return 0;
    active.push_back(tmpl); Emitter& e = active.back();
    e.X = x; e.Y = y; e.Layer = layer; e.Oriented = orient;
    return &e;
}
int main()
{
    std::list<Emitter> active; Emitter t2 = { true, 0, 0, 0, false }, t3 = { false, 0, 0, 0, false };
    Emitter* e = Create2D(active, t2, 10, 20, 3, true);
    int ok = e && e->X == 10 && e->Y == 20 && e->Layer == 3 && e->Oriented && active.size() == 1 && !Create2D(active, t3, 1, 1, 1, false);
    printf(ok ? "00aeb790_TEST PASS\n" : "00aeb790_TEST FAIL\n");
    return ok ? 0 : 1;
}
