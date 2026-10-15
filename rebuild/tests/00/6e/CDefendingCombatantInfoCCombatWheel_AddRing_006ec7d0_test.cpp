// Behaviour test for 0x006EC7D0 CCombatWheel::AddRing: each new ring takes the current ring count as its index and the
// returned reference is the appended ring; the temporary ring's prong storage is released.
#include <cstdio>
#include <cstdlib>
static int g_freed = 0;
struct CCombatTypeDef { float thickness[4]; };
struct CCombatRing {
    int Index; float RingThickness; void* m_prongs;
    CCombatRing(int index, const CCombatTypeDef& def) : Index(index), RingThickness(def.thickness[index]), m_prongs(std::malloc(4)) {}
    CCombatRing(const CCombatRing& o) : Index(o.Index), RingThickness(o.RingThickness), m_prongs(std::malloc(4)) {}
    ~CCombatRing() { if (m_prongs) { std::free(m_prongs); ++g_freed; } }
};
struct CCombatWheel {
    CCombatRing* m_rings[4]; int m_count;
    int size() const { return m_count; }
    void push_back(const CCombatRing& r) { m_rings[m_count++] = new CCombatRing(r); }
    CCombatRing& back() { return *m_rings[m_count - 1]; }
    CCombatRing& AddRing(const CCombatTypeDef& def) { int index = size(); push_back(CCombatRing(index, def)); return back(); }
};
int main()
{
    CCombatTypeDef def = { { 1.0f, 2.0f, 3.0f, 4.0f } };
    CCombatWheel w; w.m_count = 0;
    CCombatRing& a = w.AddRing(def);
    CCombatRing& b = w.AddRing(def);
    if (a.Index != 0 || b.Index != 1 || b.RingThickness != 2.0f || &b != w.m_rings[1] || w.m_count != 2 || g_freed != 2) { std::printf("006ec7d0_TEST FAIL\n"); return 1; }
    std::printf("006ec7d0_TEST PASS\n");
    return 0;
}