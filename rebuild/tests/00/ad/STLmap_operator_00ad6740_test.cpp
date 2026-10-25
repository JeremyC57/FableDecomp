#include <cstdio>
#include <cstdlib>
#include <new>

// Host model of map<pair<long,long>, CCountedPointer<CVertexPairContraction> >::operator[]: lower_bound(k); when it is
// the end or k sorts before its key, insert (k, null counted pointer) at that hint; return the mapped pointer. The tree is
// a plain (unbalanced) binary search tree with a header node, enough to exercise the subscript logic.
struct CCPPointerInfo { long RefCount; };
struct CCountedPointer {
    void*           Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { if (Info) --Info->RefCount; }
private:
    CCountedPointer& operator=(const CCountedPointer&);
};
struct TKey { long first; long second; };
static bool key_less(const TKey& x, const TKey& y)
{
    return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}
struct TValue {
    TKey            first;
    CCountedPointer second;
    TValue(const TKey& k, const CCountedPointer& p) : first(k), second(p) { }
};
struct TNode {
    TNode* Parent; TNode* Left; TNode* Right;
    TValue Value;
    TNode(const TValue& v) : Parent(0), Left(0), Right(0), Value(v) { }
};

struct TMap {
    TNode* Header;                               // Header->Parent is the root; the header is end()
    unsigned int Count;
    TMap() : Count(0)
    {
        Header = (TNode*)std::malloc(sizeof(TNode));
        Header->Parent = Header->Left = Header->Right = 0;
    }
    ~TMap() { destroy(Header->Parent); std::free(Header); }
    static void destroy(TNode* n) { if (!n) return; destroy(n->Left); destroy(n->Right); n->~TNode(); std::free(n); }
    TNode* lower_bound(const TKey& k) const
    {
        TNode* y = Header;
        TNode* x = Header->Parent;
        while (x != 0)
            if (!key_less(x->Value.first, k)) { y = x; x = x->Left; }
            else x = x->Right;
        return y;
    }
    TNode* insert_unique(TNode* hint, const TValue& v)
    {
        (void)hint;
        TNode** link = &Header->Parent;
        TNode* parent = 0;
        while (*link) { parent = *link; link = key_less(v.first, parent->Value.first) ? &parent->Left : &parent->Right; }
        TNode* n = new (std::malloc(sizeof(TNode))) TNode(v);
        n->Parent = parent;
        *link = n;
        ++Count;
        return n;
    }
    CCountedPointer& operator[](const TKey& k)
    {
        TNode* i = lower_bound(k);
        if (i == Header || key_less(k, i->Value.first))
            i = insert_unique(i, TValue(k, CCountedPointer()));
        return i->Value.second;
    }
};

static TKey key_(long a, long b) { TKey k; k.first = a; k.second = b; return k; }

int main()
{
    TMap m;
    CCountedPointer& a = m[key_(1, 2)];
    if (m.Count != 1 || a.Data != 0 || a.Info != 0) { std::printf("00ad6740_TEST FAIL\n"); return 1; }   // a null slot
    int payload = 7;
    a.Data = &payload;
    CCountedPointer& b = m[key_(1, 3)];                       // same first, larger second: a new entry
    CCountedPointer& c = m[key_(0, 9)];                       // smaller first: a new entry
    if (m.Count != 3 || b.Data != 0 || c.Data != 0) { std::printf("00ad6740_TEST FAIL\n"); return 1; }
    CCountedPointer& a2 = m[key_(1, 2)];                      // found: the same slot, nothing inserted
    if (&a2 != &a || m.Count != 3 || a2.Data != &payload) { std::printf("00ad6740_TEST FAIL\n"); return 1; }
    if (&m[key_(0, 9)] != &c || &m[key_(1, 3)] != &b || m.Count != 3) { std::printf("00ad6740_TEST FAIL\n"); return 1; }
    a.Data = 0;
    std::printf("00ad6740_TEST PASS\n");
    return 0;
}