// Behaviour test for 0x007FF8A0 CTCDHeroSuit::OnCreate: GetSubDef into PDef, then push (part, false) for each def map entry in order.
#include <cstdio>
#include <cstdlib>
#include <new>
enum EClothingSuitPart { PART_0, PART_1, PART_2, PART_3, PART_4, PART_5 };
struct SPair { EClothingSuitPart first; bool second; SPair(const EClothingSuitPart& a, const bool& b) : first(a), second(b) {} SPair(const SPair& o) : first(o.first), second(o.second) {} };
struct CVector {
    SPair* _M_start; SPair* _M_finish; SPair* _M_end_of_storage;
    void _M_insert_overflow(SPair* pos, const SPair& x, unsigned long fill, bool atend)
    {
        unsigned long old = _M_finish - _M_start, len = old + (old < fill ? fill : old);
        SPair* ns = len ? (SPair*)std::malloc(len * sizeof(SPair)) : 0; SPair* nf = ns;
        for (SPair* p = _M_start; p != pos; ++p, ++nf) new (nf) SPair(*p);
        for (unsigned long n = fill; n > 0; --n, ++nf) new (nf) SPair(x);
        if (!atend) for (SPair* q = pos; q != _M_finish; ++q, ++nf) new (nf) SPair(*q);
        if (_M_start) std::free(_M_start);
        _M_start = ns; _M_finish = nf; _M_end_of_storage = ns + len;
    }
    void push_back(const SPair& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) SPair(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, 1UL, true);
    }
};
struct SNode { long c; SNode* _M_parent; SNode* _M_left; SNode* _M_right; EClothingSuitPart first; long second; };
static const SNode* Increment(const SNode* node)
{
    if (node->_M_right != 0) { node = node->_M_right; while (node->_M_left != 0) node = node->_M_left; }
    else { const SNode* y = node->_M_parent; while (node == y->_M_right) { node = y; y = y->_M_parent; } if (node->_M_right != y) node = y; }
    return node;
}
struct CHeroSuitDef { SNode* header; };
struct CDefClassBase { const CHeroSuitDef* sub; bool GetSubDef(const CHeroSuitDef** out) const { *out = sub; return true; } };
struct CThing { CDefClassBase* PDef; };
struct CTCDHeroSuit {
    CThing* ParentThing; const CHeroSuitDef* PDef; CVector SuitParts;
    void OnCreate()
    {
        ParentThing->PDef->GetSubDef(&PDef);
        for (const SNode* it = PDef->header->_M_left; it != PDef->header; it = Increment(it))
            SuitParts.push_back(SPair(it->first, false));
    }
};
int main()
{
    SNode h, a, b, c;
    h._M_parent = &b; h._M_left = &a; h._M_right = &c;
    b._M_parent = &h; b._M_left = &a; b._M_right = &c; b.first = PART_2;
    a._M_parent = &b; a._M_left = 0; a._M_right = 0; a.first = PART_1;
    c._M_parent = &b; c._M_left = 0; c._M_right = 0; c.first = PART_5;
    CHeroSuitDef def; def.header = &h;
    CDefClassBase thingDef; thingDef.sub = &def;
    CThing thing; thing.PDef = &thingDef;
    CTCDHeroSuit suit; suit.ParentThing = &thing; suit.PDef = 0;
    suit.SuitParts._M_start = suit.SuitParts._M_finish = suit.SuitParts._M_end_of_storage = 0;
    suit.OnCreate();
    SPair* p = suit.SuitParts._M_start;
    if (suit.PDef != &def || suit.SuitParts._M_finish - p != 3 || p[0].first != PART_1 || p[1].first != PART_2
        || p[2].first != PART_5 || p[0].second || p[1].second || p[2].second) {
        std::printf("007ff8a0_TEST FAIL\n"); return 1;
    }
    std::free(p);
    std::printf("007ff8a0_TEST PASS\n");
    return 0;
}