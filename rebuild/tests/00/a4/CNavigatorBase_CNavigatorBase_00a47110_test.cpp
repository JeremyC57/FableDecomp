#include <cstdio>
struct Mgr { int registered; };
struct Iface { int id; Iface* Clone() const { Iface* c = new Iface; c->id = id + 100; return c; } };
struct Agent { const Iface* iface; bool b; Agent(const Iface* i, bool f) : iface(i), b(f) { } };
class CClass { public: Mgr* factory; unsigned int type; CClass(Mgr& m, unsigned int t, bool reg) : factory(&m), type(t) { if (reg) ++m.registered; } };
struct Nav : CClass {
    Iface* PInterface; bool OnRoute; bool Interrupted; Mgr* NavigationManager; Agent* SteeringAgent;
    Nav(Mgr& m, unsigned int type, const Iface& i) : CClass(m, type, true), OnRoute(false), Interrupted(false), NavigationManager(&m)
    {
        PInterface = i.Clone();
        SteeringAgent = new Agent(PInterface, false);
    }
};
int main()
{
    Mgr m; m.registered = 0; Iface i; i.id = 5;
    Nav n(m, 7, i);
    if (m.registered != 1 || n.type != 7 || n.NavigationManager != &m || n.OnRoute || n.Interrupted || !n.PInterface || n.PInterface->id != 105 ||
        n.SteeringAgent->iface != n.PInterface || n.SteeringAgent->b) { std::printf("00a47110_TEST FAIL\n"); return 1; }
    delete n.SteeringAgent; delete n.PInterface;
    std::printf("00a47110_TEST PASS\n");
    return 0;
}