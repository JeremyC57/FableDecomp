#include <cstdio>
#include <math.h>
struct V2 { float X, Y; };
static float Distance2D(const V2& a, const V2& b) { float dx = a.X - b.X, dy = a.Y - b.Y; return (float)sqrt(dx * dx + dy * dy); }
struct Node { V2 Position; unsigned char pref; unsigned char GetPreferabilityCost() const { return pref; } };
struct Iface { float GetPreferredNodeCost() const { return 2.0f; } float GetUnpreferredNodeCost() const { return 6.0f; } };
struct AStar {
    Iface* PInterface;
    float GetCost(const Node* from, const Node* to, bool ignore) const
    {
        float distance = Distance2D(from->Position, to->Position);
        float factor = 1.0f;
        if (!ignore) {
            switch (to->GetPreferabilityCost()) {
            case 0x00: factor = PInterface->GetPreferredNodeCost(); break;
            case 0x40: factor = (PInterface->GetUnpreferredNodeCost() + PInterface->GetPreferredNodeCost()) * 0.5f; break;
            case 0x80: factor = PInterface->GetUnpreferredNodeCost(); break;
            default: factor = PInterface->GetUnpreferredNodeCost(); break;
            }
        }
        return factor * distance;
    }
};
int main()
{
    Iface i; AStar a; a.PInterface = &i;
    Node n1 = { { 0, 0 }, 0 }, n2 = { { 3, 4 }, 0 };
    bool fail = false;
    if (a.GetCost(&n1, &n2, true) != 5.0f) fail = true;
    if (a.GetCost(&n1, &n2, false) != 10.0f) fail = true;
    n2.pref = 0x40; if (a.GetCost(&n1, &n2, false) != 20.0f) fail = true;
    n2.pref = 0x80; if (a.GetCost(&n1, &n2, false) != 30.0f) fail = true;
    n2.pref = 0x11; if (a.GetCost(&n1, &n2, false) != 30.0f) fail = true;
    if (fail) { std::printf("00a1a0a0_TEST FAIL\n"); return 1; }
    std::printf("00a1a0a0_TEST PASS\n");
    return 0;
}