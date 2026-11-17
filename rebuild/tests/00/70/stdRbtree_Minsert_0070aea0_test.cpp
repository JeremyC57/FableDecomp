// Behaviour test for 0x0070AEA0 _Rb_tree<long, pair<const long, CEngineTextureDecal>>::_M_insert (link + count).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_blendCopies = 0;
static int g_rebalances = 0;
struct CBlendVector { int tag; CBlendVector(int t) : tag(t) {} CBlendVector(const CBlendVector& o) : tag(o.tag) { ++g_blendCopies; } };
class CEngineTextureDecal { public: int OriginalTextureIndex; int DecalID; CBlendVector OverlaidBlends; CEngineTextureDecal(int k, int d) : OriginalTextureIndex(k), DecalID(d), OverlaidBlends(d) {} };
struct SValue { const int first; CEngineTextureDecal second; SValue(int k, int d) : first(k), second(k, d) {}
                SValue(const SValue& o) : first(o.first), second(o.second) {} };
struct SNodeBase { int color; SNodeBase* parent; SNodeBase* left; SNodeBase* right; };
struct SNode : SNodeBase { SValue value; };
static void Rebalance(SNodeBase*, SNodeBase*&) { ++g_rebalances; }

class CMap { public:
    SNodeBase* header; unsigned int count;
    SNode* create(const SValue& v) { SNode* t = (SNode*)std::malloc(sizeof(SNode)); new (&t->value) SValue(v); return t; }
    SNodeBase* insert(SNodeBase* x, SNodeBase* y_, const SValue& v, SNodeBase* w)
    {
        SNode* y = (SNode*)y_; SNode* z;
        if (y == header || (w == 0 && (x != 0 || v.first < y->value.first))) {
            z = create(v); y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            z = create(v); y->right = z;
            if (y == header->right) header->right = z;
        }
        z->parent = y; z->left = 0; z->right = 0;
        Rebalance(z, header->parent);
        ++count;
        return z;
    }
};

int main()
{
    SNodeBase head = { 0, 0, &head, &head };
    CMap m; m.header = &head; m.count = 0;
    SValue a(10, 1), b(20, 2), c(5, 3);
    SNodeBase* ra = m.insert(0, &head, a, 0);                       // first node: root, leftmost, rightmost
    bool ok = head.parent == ra && head.left == ra && head.right == ra && m.count == 1 && ((SNode*)ra)->value.second.DecalID == 1;
    SNodeBase* rb = m.insert(0, ra, b, 0);                          // 20 > 10: right child, new rightmost
    ok = ok && ra->right == rb && head.right == rb && head.left == ra && rb->parent == ra;
    SNodeBase* rc = m.insert(0, ra, c, 0);                          // 5 < 10: left child, new leftmost
    ok = ok && ra->left == rc && head.left == rc && rc->left == 0 && rc->right == 0 && m.count == 3 && g_rebalances == 3;
    ok = ok && g_blendCopies == 3 && ((SNode*)rc)->value.second.OverlaidBlends.tag == 3;
    if (!ok) { std::printf("0070aea0_TEST FAIL\n"); return 1; }
    std::printf("0070aea0_TEST PASS\n");
    return 0;
}