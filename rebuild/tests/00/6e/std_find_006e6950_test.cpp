// Behaviour test for std::find<CIntelligentPointer<CThing>*, CThing*> (0x006E6950): first pointer whose item equals the value, else last.
#include <cstdio>
class CThing { public: int Id; };
struct CIntelligentPointer { CThing* PItem; void* PNext; };
static CIntelligentPointer* find_ptr(CIntelligentPointer* first, CIntelligentPointer* last, CThing* const& val)
{
    for (; first != last; ++first)
        if (first->PItem == val) return first;
    return last;
}
int main()
{
    CThing a, b, c;
    CIntelligentPointer v[3];
    v[0].PItem = &a; v[1].PItem = &b; v[2].PItem = &b;
    CThing* pb = &b; CThing* pc = &c;
    if (find_ptr(v, v + 3, pb) != v + 1) { std::printf("006e6950_TEST FAIL\n"); return 1; }
    if (find_ptr(v, v + 3, pc) != v + 3) { std::printf("006e6950_TEST FAIL\n"); return 1; }
    if (find_ptr(v, v, pb) != v) { std::printf("006e6950_TEST FAIL\n"); return 1; }
    std::printf("006e6950_TEST PASS\n");
    return 0;
}