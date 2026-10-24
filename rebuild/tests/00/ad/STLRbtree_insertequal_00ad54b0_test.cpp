// Behaviour test for 0x00AD54B0 _Rb_tree::insert_equal on a float-keyed multimap (descent + _M_insert(x, y, v, 0)).
#include <cstdio>
#include <cstdlib>

struct SNodeBase { bool Color; SNodeBase* Parent; SNodeBase* Left; SNodeBase* Right; };
struct SValue { float first; int second; };
struct SNode : SNodeBase { SValue Value; };

struct STree {
    SNodeBase* Header; unsigned int Count;
    int lastX; SNodeBase* lastY;
    // stand-in for _M_insert (0x00AD4540) without rebalancing: x is always 0 from insert_equal
    SNodeBase* _M_insert(SNodeBase* x, SNodeBase* y, const SValue& v, SNodeBase* w)
    {
        lastX = (x != 0); lastY = y;
        SNode* n = (SNode*)std::malloc(sizeof(SNode)); n->Value = v; n->Left = n->Right = 0; n->Parent = y; n->Color = 0;
        if (y == Header || x != 0 || (w == 0 && v.first < ((SNode*)y)->Value.first)) {
            y->Left = n;
            if (y == Header) { Header->Parent = n; Header->Right = n; }
            else if (y == Header->Left) Header->Left = n;
        } else {
            y->Right = n;
            if (y == Header->Right) Header->Right = n;
        }
        ++Count; return n;
    }
    // reconstructed body
    SNodeBase* insert_equal(const SValue& v)
    {
        SNodeBase* y = Header; SNodeBase* x = Header->Parent;
        while (x != 0) { y = x; x = (v.first < ((SNode*)x)->Value.first) ? x->Left : x->Right; }
        return _M_insert(x, y, v, 0);
    }
};

static int InOrder(SNodeBase* n, float* out, int k) { if (!n) return k; k = InOrder(n->Left, out, k); out[k++] = ((SNode*)n)->Value.first; return InOrder(n->Right, out, k); }

int main()
{
    STree t; t.Header = (SNodeBase*)std::malloc(sizeof(SNode)); t.Header->Parent = 0; t.Header->Left = t.Header->Right = t.Header; t.Count = 0;
    float keys[7] = { 3.0f, 1.0f, 4.0f, 1.0f, 5.0f, 9.0f, 2.0f };
    for (int i = 0; i < 7; ++i) {
        SValue v; v.first = keys[i]; v.second = i;
        SNodeBase* r = t.insert_equal(v);
        if (((SNode*)r)->Value.second != i || t.lastX != 0) { std::printf("00ad54b0_TEST FAIL\n"); return 1; }
    }
    float out[7]; int n = InOrder(t.Header->Parent, out, 0);
    float want[7] = { 1.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 9.0f };
    if (n != 7 || t.Count != 7) { std::printf("00ad54b0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 7; ++i) if (out[i] != want[i]) { std::printf("00ad54b0_TEST FAIL\n"); return 1; }
    // equal keys go to the right of existing ones (second 1.0 follows the first)
    if (((SNode*)t.Header->Left)->Value.second != 1) { std::printf("00ad54b0_TEST FAIL\n"); return 1; }
    std::printf("00ad54b0_TEST PASS\n");
    return 0;
}