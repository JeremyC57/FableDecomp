// Behaviour test for 0x006D8D00 CTCHeroExperience::AddSuccessfulStrikeWithHitParams: one AddSuccessfulStrikeOfType per
// damage-map entry in key order, with special = BlockCounter || KnockDown || Flourish.
#include <cstdio>

struct NodeBase { unsigned char color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { long first; float second; };
static NodeBase* Increment(NodeBase* x)
{
    if (x->right) { x = x->right; while (x->left) x = x->left; }
    else { NodeBase* y = x->parent; while (x == y->right) { x = y; y = y->parent; } if (x->right != y) x = y; }
    return x;
}
struct HitParams { bool KnockDown, Flourish, BlockCounter; NodeBase* DamageMapHeader; };

struct Experience {
    long types[8]; float damages[8]; bool specials[8]; int n;
    void AddSuccessfulStrikeOfType(long type, float damage, bool special) { types[n] = type; damages[n] = damage; specials[n] = special; ++n; }
    void AddSuccessfulStrikeWithHitParams(const HitParams& p)
    {
        for (NodeBase* it = p.DamageMapHeader->left; it != p.DamageMapHeader; it = Increment(it))
            AddSuccessfulStrikeOfType(((Node*)it)->first, ((Node*)it)->second, p.BlockCounter || p.KnockDown || p.Flourish);
    }
};

int main()
{
    Node header, a, b;                               // a root, b right child
    header.color = 0; header.parent = &a; header.left = &a; header.right = &b;
    a.parent = &header; a.left = 0; a.right = &b; a.first = 1; a.second = 10.0f;
    b.parent = &a; b.left = b.right = 0; b.first = 4; b.second = 2.5f;
    HitParams p; p.KnockDown = false; p.Flourish = false; p.BlockCounter = false; p.DamageMapHeader = &header;
    Experience e; e.n = 0;
    e.AddSuccessfulStrikeWithHitParams(p);
    if (e.n != 2 || e.types[0] != 1 || e.damages[0] != 10.0f || e.types[1] != 4 || e.damages[1] != 2.5f || e.specials[0] || e.specials[1]) { std::printf("006d8d00_TEST FAIL\n"); return 1; }
    p.Flourish = true; e.n = 0;
    e.AddSuccessfulStrikeWithHitParams(p);
    if (e.n != 2 || !e.specials[0] || !e.specials[1]) { std::printf("006d8d00_TEST FAIL\n"); return 1; }
    Node empty; empty.color = 0; empty.parent = 0; empty.left = empty.right = &empty;
    p.DamageMapHeader = &empty; e.n = 0;
    e.AddSuccessfulStrikeWithHitParams(p);
    if (e.n != 0) { std::printf("006d8d00_TEST FAIL\n"); return 1; }
    std::printf("006d8d00_TEST PASS\n");
    return 0;
}