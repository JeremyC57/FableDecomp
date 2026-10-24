// Behaviour test for 0x00658E38 CTCVillage::GetNearestBuildingWithProperty (model copy of for_each + functor)
#include <cstdio>
struct Building { float x, y, z; int property; };
struct Node { Node* next; Node* prev; Building* b; };
struct Nearest {
    int property; float px, py, pz; Building* nearest; float best;
    Nearest(float x, float y, float z, int prop) : property(prop), px(x), py(y), pz(z), nearest(0), best(-1.0f) { }
    void operator()(Building* b)
    {
        if (b->property != property) return;
        float dx = b->x - px, dy = b->y - py, dz = b->z - pz, d = dx * dx + dy * dy + dz * dz;
        if (best < 0.0f || d < best) { best = d; nearest = b; }
    }
};
template <class F> F for_each_list(Node* first, Node* last, F f) { for (; first != last; first = first->next) f(first->b); return f; }
struct Village { Node* VillageBuildings; Building* GetNearest(float x, float y, float z, int p) { return for_each_list(VillageBuildings->next, VillageBuildings, Nearest(x, y, z, p)).nearest; } };
int main()
{
    Building a = { 10, 0, 0, 1 }, b = { 3, 0, 0, 2 }, c = { 5, 0, 0, 1 };
    Node s, na, nb, nc;
    s.next = &na; na.next = &nb; nb.next = &nc; nc.next = &s;
    na.b = &a; nb.b = &b; nc.b = &c;
    Village v; v.VillageBuildings = &s;
    if (v.GetNearest(0, 0, 0, 1) != &c || v.GetNearest(0, 0, 0, 2) != &b || v.GetNearest(0, 0, 0, 3) != 0) { std::printf("00658e38_TEST FAIL\n"); return 1; }
    s.next = &s;
    if (v.GetNearest(0, 0, 0, 1) != 0) { std::printf("00658e38_TEST FAIL\n"); return 1; }
    std::printf("00658e38_TEST PASS\n");
    return 0;
}