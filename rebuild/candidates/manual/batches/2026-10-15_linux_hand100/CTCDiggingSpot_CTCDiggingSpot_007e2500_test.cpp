#include <cstdio>
struct CThing { int id; };
struct CTCBase {
    CThing* m_pParent;
    CTCBase(CThing* parent) : m_pParent(parent) { }
    virtual ~CTCBase() { }
};
struct CTCDiggingSpot : CTCBase {
    bool m_canBeDug; bool m_dug;
    CTCDiggingSpot(CThing* parent) : CTCBase(parent), m_canBeDug(true), m_dug(false) { }
};
int main()
{
    CThing t;
    CTCDiggingSpot d(&t);
    if (d.m_pParent != &t || !d.m_canBeDug || d.m_dug) { std::printf("007e2500_TEST FAIL\n"); return 1; }
    std::printf("007e2500_TEST PASS\n");
    return 0;
}
