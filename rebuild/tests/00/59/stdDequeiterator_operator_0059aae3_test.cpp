// Behaviour test for 0x0059AAE3: the traits-flavoured deque iterator operator- forwards to _M_subtract.
#include <cstdio>
#include <cstddef>
struct Elem;
template <class T> struct IterBase {
    T* cur; T* first; T* last; T** node;
    ptrdiff_t Subtract(const IterBase& x) const
    {
        return ptrdiff_t(32) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
    }
};
template <class T> struct Iter : public IterBase<T> {
    ptrdiff_t operator-(const Iter& x) const { return this->Subtract(x); }
};
int main()
{
    Elem* blockA[32]; Elem* blockB[32]; Elem* blockC[32];
    Elem** map[3] = { blockA, blockB, blockC };
    Iter<Elem*> b, e;
    b.first = blockA; b.last = blockA + 32; b.cur = blockA + 5;  b.node = &map[0];
    e.first = blockC; e.last = blockC + 32; e.cur = blockC + 7;  e.node = &map[2];
    // elements: 27 left in A + 32 in B + 7 in C
    if (e - b != 66) { std::printf("0059aae3_TEST FAIL\n"); return 1; }
    Iter<Elem*> s = b; s.cur = blockA + 9;
    if (s - b != 4 || b - b != 0) { std::printf("0059aae3_TEST FAIL\n"); return 1; }
    std::printf("0059aae3_TEST PASS\n");
    return 0;
}
