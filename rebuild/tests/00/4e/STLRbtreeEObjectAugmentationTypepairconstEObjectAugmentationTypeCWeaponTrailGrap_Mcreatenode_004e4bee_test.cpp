#include <cstdio>
#include <cstdlib>
#include <new>
struct CWeaponTrailGraphicSet { long Attack; long Knockdown; };
struct SValue {
    const long first; CWeaponTrailGraphicSet second;
    SValue(long k, long a, long b) : first(k) { second.Attack = a; second.Knockdown = b; }
    SValue(const SValue& p) : first(p.first), second(p.second) { }
};
struct SNodeBase { bool color; SNodeBase* parent; SNodeBase* left; SNodeBase* right; };
struct SNode : SNodeBase { SValue value; };
static SNode* CreateNode(const SValue& x)
{
    SNode* tmp = static_cast<SNode*>(std::malloc(sizeof(SNode)));
    new (&tmp->value) SValue(x);
    return tmp;
}
int main()
{
    bool ok = true;
    SValue v(7, 1234, -56);
    SNode* n = CreateNode(v);
    if (!n || n->value.first != 7 || n->value.second.Attack != 1234 || n->value.second.Knockdown != -56) ok = false;
    SNode* m = CreateNode(SValue(-3, 0, 99));
    if (!m || m == n || m->value.first != -3 || m->value.second.Knockdown != 99) ok = false;
    std::free(n); std::free(m);
    if (!ok) { std::printf("004e4bee_TEST FAIL\n"); return 1; }
    std::printf("004e4bee_TEST PASS\n");
    return 0;
}