#include <cstdio>
class CThing { public: int id; };
class CTCBase { public:
    CThing* m_pParent;
    CTCBase(CThing* parent) : m_pParent(parent) { }
    virtual ~CTCBase() { }
};
class CTCDiggingSpot : public CTCBase { public:
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