#include <cstdio>

class C2DCoordI { public: int x, y; };

struct Target {
    void* got_a; C2DCoordI* got_b; unsigned long got_c, got_d; int called;
    void Apply(void* a, C2DCoordI* b, unsigned long c, unsigned long d) {
        got_a = a; got_b = b; got_c = c; got_d = d; called = 1;
    }
};

class CEditTransactionBase { public:
    void*   m0;
    Target* m4;
    void*   m8;
};

void ActionSetEngineTheme(CEditTransactionBase* transaction, C2DCoordI* coord, unsigned long a, unsigned long b)
{
    transaction->m4->Apply(transaction->m8, coord, a, b);
}

int main() {
    Target t; t.called = 0;
    CEditTransactionBase tx;
    tx.m0 = 0; tx.m4 = &t; tx.m8 = (void*)0xDEADBEEF;
    C2DCoordI coord; coord.x = 3; coord.y = 7;

    ActionSetEngineTheme(&tx, &coord, 11u, 22u);

    if (t.called == 1 &&
        t.got_a == (void*)0xDEADBEEF &&
        t.got_b == &coord &&
        t.got_c == 11u &&
        t.got_d == 22u) {
        printf("PARITY_OK\n");
    } else {
        printf("PARITY_FAIL\n");
    }
    return 0;
}