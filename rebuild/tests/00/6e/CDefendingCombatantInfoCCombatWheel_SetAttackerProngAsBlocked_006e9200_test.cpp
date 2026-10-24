// Behaviour test for 0x006E9200 (CCombatWheel::SetAttackerProngAsBlocked: block the attacker's prong on its ring, then ring index -1).
#include <cstdio>
#include <vector>

struct Creature { int id; };
struct Prong { bool Blocked; const Creature* Occupier; void SetBlocked(bool b) { Blocked = b; } };
struct Ring {
    std::vector<Prong> Prongs;
    Prong* GetAttackerProng(const Creature& c) { for (unsigned i = 0; i < Prongs.size(); ++i) if (Prongs[i].Occupier == &c) return &Prongs[i]; return 0; }
};
struct Info { long CombatWheelRingIndex; };
struct Wheel {
    std::vector<Ring> CombatRings;
    Ring* GetRing(long index) { if (index >= 0 && (unsigned long)index < CombatRings.size()) return &CombatRings[index]; return 0; }
    void SetAttackerProngAsBlocked(const Creature& attacker, Info& info)
    {
        Ring* ring = GetRing(info.CombatWheelRingIndex);
        if (ring) { Prong* p = ring->GetAttackerProng(attacker); if (p) p->SetBlocked(true); }
        info.CombatWheelRingIndex = -1;
    }
};
#define FAIL { std::printf("006e9200_TEST FAIL\n"); return 1; }
int main()
{
    Creature a = { 1 }, b = { 2 };
    Wheel w; w.CombatRings.resize(2);
    Prong pa = { false, &a }, pb = { false, &b };
    w.CombatRings[1].Prongs.push_back(pb); w.CombatRings[1].Prongs.push_back(pa);
    Info info = { 1 };
    w.SetAttackerProngAsBlocked(a, info);
    if (info.CombatWheelRingIndex != -1 || !w.CombatRings[1].Prongs[1].Blocked || w.CombatRings[1].Prongs[0].Blocked) FAIL;
    info.CombatWheelRingIndex = 0;                      // ring without a prong for b
    w.SetAttackerProngAsBlocked(b, info);
    if (info.CombatWheelRingIndex != -1 || w.CombatRings[1].Prongs[0].Blocked) FAIL;
    info.CombatWheelRingIndex = 5;                      // out of range
    w.SetAttackerProngAsBlocked(b, info);
    if (info.CombatWheelRingIndex != -1 || w.CombatRings[1].Prongs[0].Blocked) FAIL;
    w.SetAttackerProngAsBlocked(b, info);               // negative index
    if (info.CombatWheelRingIndex != -1) FAIL;
    std::printf("006e9200_TEST PASS\n");
    return 0;
}