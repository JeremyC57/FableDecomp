// Behaviour test for std::__destroy_aux<CShopItemDef*> (0x004DA5BD): every element of [first, last) is destroyed once, in order,
// through its virtual destructor; an empty range destroys nothing.
#include <cstdio>
#include <new>

static int g_count = 0;
static int g_order[8];

class CShopItemDef {
public:
    CShopItemDef(int tag) : Tag(tag) { }
    virtual ~CShopItemDef();
    int Tag;
    unsigned char _body[0x40];
};
CShopItemDef::~CShopItemDef() { if (g_count < 8) g_order[g_count] = Tag; ++g_count; }

namespace std {
struct __false_type { };
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);
}

int main()
{
    static unsigned char raw[3 * sizeof(CShopItemDef)];
    CShopItemDef* p = (CShopItemDef*)raw;
    for (int i = 0; i < 3; ++i) new (p + i) CShopItemDef(10 + i);
    std::__false_type tag;
    std::__destroy_aux(p, p + 3, tag);
    int ok = sizeof(CShopItemDef) == 0x48 && g_count == 3 && g_order[0] == 10 && g_order[1] == 11 && g_order[2] == 12;
    std::__destroy_aux(p, p, tag);
    if (g_count != 3) ok = 0;
    std::printf(ok ? "004da5bd_TEST PASS\n" : "004da5bd_TEST FAIL\n");
    return ok ? 0 : 1;
}
