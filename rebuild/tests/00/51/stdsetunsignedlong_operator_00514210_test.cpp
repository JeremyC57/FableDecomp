// Behaviour test for 00514210 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
// set<unsigned long>::operator= delegates to the tree's operator= and returns *this
struct Tree { unsigned int Items[8]; unsigned int Count;
    Tree& operator=(const Tree& x) { if (this != &x) { Count = x.Count; for (unsigned int i = 0; i < x.Count; ++i) Items[i] = x.Items[i]; } return *this; } };
class Set { public: Tree _M_t; Set& operator=(const Set& x) { _M_t = x._M_t; return *this; } };
int main() {
    Set a, b; a._M_t.Count = 0; b._M_t.Count = 3; b._M_t.Items[0] = 1; b._M_t.Items[1] = 5; b._M_t.Items[2] = 9;
    Set& r = (a = b);
    if (&r != &a || a._M_t.Count != 3 || a._M_t.Items[2] != 9) { std::printf("00514210_TEST FAIL\n"); return 1; }
    { std::printf("00514210_TEST PASS\n"); return 0; }
}