// Test for 0x00733750 CTCCombat::GetCombatAbilityData(const_iterator&, ECombatAbilityInterfaceType) const: links the source object; the non-const
// overload (stubbed here) must receive a null-started iterator and the forwarded argument; on success the caller's
// iterator gets the found node and the result is true, on failure the caller's iterator is untouched.
#include <stdio.h>
struct _Rb_tree_node_base { long key; };
struct CCombatAbilityIterator {
    _Rb_tree_node_base* _M_node;
    CCombatAbilityIterator() {}
    CCombatAbilityIterator(_Rb_tree_node_base* __x) { _M_node = __x; }
};
struct CCombatAbilityConstIterator {
    _Rb_tree_node_base* _M_node;
    CCombatAbilityConstIterator() {}
    CCombatAbilityConstIterator(const CCombatAbilityIterator& __it) { _M_node = __it._M_node; }
};
enum ECombatAbilityInterfaceType { ECombatAbilityInterfaceType_0 };
static _Rb_tree_node_base g_nodes[2] = { { 3 }, { 5 } };
static int g_calls, g_nonnull;
static const void* g_this;
class CTCCombat {
    friend int main();
    int dummy;
    bool GetCombatAbilityData(CCombatAbilityIterator& it, ECombatAbilityInterfaceType type);
    bool GetCombatAbilityData(CCombatAbilityConstIterator& it, ECombatAbilityInterfaceType type) const;
};
bool CTCCombat::GetCombatAbilityData(CCombatAbilityIterator& it, ECombatAbilityInterfaceType type)
{
    ++g_calls; g_this = this;
    if (it._M_node != 0) ++g_nonnull;
    for (int i = 0; i < 2; ++i)
        if (g_nodes[i].key == (long)type) { it._M_node = &g_nodes[i]; return true; }
    return false;
}
int main()
{
    CTCCombat t;
    const CTCCombat& ct = t;
    CCombatAbilityConstIterator out; out._M_node = 0;
    bool ok = ct.GetCombatAbilityData(out, (ECombatAbilityInterfaceType)5) && out._M_node == &g_nodes[1] && g_this == &t;
    CCombatAbilityConstIterator keep; keep._M_node = &g_nodes[0]; out = keep;
    ok = ok && !ct.GetCombatAbilityData(out, (ECombatAbilityInterfaceType)9) && out._M_node == &g_nodes[0];
    ok = ok && g_calls == 2 && g_nonnull == 0;
    if (!ok) { puts("00733750_TEST FAIL"); return 1; }
    puts("00733750_TEST PASS");
    return 0;
}
