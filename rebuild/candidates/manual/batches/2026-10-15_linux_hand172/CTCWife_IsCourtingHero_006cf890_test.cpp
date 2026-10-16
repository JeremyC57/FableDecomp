#include <cstdio>
struct COp { bool courting; };
struct CThing { unsigned int bits[9]; COp* op; };
struct CTCWife {
    CThing* parent; bool courtable;
    bool IsCourtable() const { return courtable; }
    bool Get() const { if (IsCourtable() && (parent->bits[2] & 0x8000u)) { if (parent->op->courting) return true; } return false; }
};
int main()
{
    COp y = { true }, n = { false };
    CThing t1 = { {0, 0, 0x8000u}, &y }, t2 = { {0, 0, 0x8000u}, &n }, t3 = { {0}, &y };
    CTCWife a = { &t1, true }, b = { &t2, true }, c = { &t3, true }, d = { &t1, false };
    if (!a.Get() || b.Get() || c.Get() || d.Get()) { std::printf("006cf890_TEST FAIL\n"); return 1; }
    std::printf("006cf890_TEST PASS\n");
    return 0;
}
