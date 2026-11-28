// Test for 0x00734880 FixtureCTCCombat::GetBlockAbilityIterator(const_iterator&, EWeaponClass) const: runs a fixture with the same forwarding body; the non-const
// overload (stubbed here) must receive a null-started iterator and the forwarded argument; on success the caller's
// iterator gets the found node and the result is true, on failure the caller's iterator is untouched.
#include <stdio.h>
struct FixtureRb_tree_node_base { long key; };
struct FixtureCCombatAbilityIterator {
    FixtureRb_tree_node_base* _M_node;
    FixtureCCombatAbilityIterator() {}
    FixtureCCombatAbilityIterator(FixtureRb_tree_node_base* __x) { _M_node = __x; }
};
struct FixtureCCombatAbilityConstIterator {
    FixtureRb_tree_node_base* _M_node;
    FixtureCCombatAbilityConstIterator() {}
    FixtureCCombatAbilityConstIterator(const FixtureCCombatAbilityIterator& __it) { _M_node = __it._M_node; }
};
enum EWeaponClass { EWeaponClass_0 };
static FixtureRb_tree_node_base g_nodes[2] = { { 3 }, { 5 } };
static int g_calls, g_nonnull;
static const void* g_this;
class FixtureCTCCombat {
    friend int main();
    int dummy;
    bool GetBlockAbilityIterator(FixtureCCombatAbilityIterator& it, EWeaponClass type);
    bool GetBlockAbilityIterator(FixtureCCombatAbilityConstIterator& it, EWeaponClass type) const;
};
bool FixtureCTCCombat::GetBlockAbilityIterator(FixtureCCombatAbilityIterator& it, EWeaponClass type)
{
    ++g_calls; g_this = this;
    if (it._M_node != 0) ++g_nonnull;
    for (int i = 0; i < 2; ++i)
        if (g_nodes[i].key == (long)type) { it._M_node = &g_nodes[i]; return true; }
    return false;
}
bool FixtureCTCCombat::GetBlockAbilityIterator(FixtureCCombatAbilityConstIterator& it,EWeaponClass weapon_class) const
{
    FixtureCCombatAbilityIterator found(0);
    if (const_cast<FixtureCTCCombat*>(this)->GetBlockAbilityIterator(found,weapon_class)) {
        it=found;
        return true;
    }
    return false;
}

int main()
{
    FixtureCTCCombat t;
    const FixtureCTCCombat& ct = t;
    FixtureCCombatAbilityConstIterator out; out._M_node = 0;
    bool ok = ct.GetBlockAbilityIterator(out, (EWeaponClass)5) && out._M_node == &g_nodes[1] && g_this == &t;
    FixtureCCombatAbilityConstIterator keep; keep._M_node = &g_nodes[0]; out = keep;
    ok = ok && !ct.GetBlockAbilityIterator(out, (EWeaponClass)9) && out._M_node == &g_nodes[0];
    ok = ok && g_calls == 2 && g_nonnull == 0;
    if (!ok) { puts("00734880_TEST FAIL"); return 1; }
    puts("00734880_TEST PASS");
    return 0;
}
