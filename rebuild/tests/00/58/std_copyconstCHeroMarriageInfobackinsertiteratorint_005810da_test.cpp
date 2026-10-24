// Behaviour test for 0x005810DA std::__copy<const CHeroMarriageInfo*, back_insert_iterator<list>, int> (model copy)
#include <cstdio>
struct Rec { unsigned int WifeLo, WifeHi, HomeLo, HomeHi; long State; bool Gay; };
struct FakeList {
    Rec items[8]; int count;
    void push_back(const Rec& x) { items[count++] = x; }
};
struct BackInserter {
    FakeList* container;
    explicit BackInserter(FakeList& x) : container(&x) {}
    BackInserter& operator=(const Rec& v) { container->push_back(v); return *this; }
    BackInserter& operator*() { return *this; }
    BackInserter& operator++() { return *this; }
};
struct Tag {};
template <class It, class Out, class D> Out copy_ra(It first, It last, Out result, const Tag&, D*)
{
    for (D n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    Rec src[3];
    for (int i = 0; i < 3; ++i) { src[i].WifeLo = 10 + i; src[i].WifeHi = 0; src[i].HomeLo = 20 + i; src[i].HomeHi = 0; src[i].State = i; src[i].Gay = (i == 1); }
    FakeList l; l.count = 0;
    BackInserter r = copy_ra(src, src + 3, BackInserter(l), Tag(), (int*)0);
    if (l.count != 3 || r.container != &l || l.items[2].WifeLo != 12 || !l.items[1].Gay || l.items[0].HomeLo != 20) { std::printf("005810da_TEST FAIL\n"); return 1; }
    copy_ra(src, src, BackInserter(l), Tag(), (int*)0);
    if (l.count != 3) { std::printf("005810da_TEST FAIL\n"); return 1; }
    std::printf("005810da_TEST PASS\n");
    return 0;
}