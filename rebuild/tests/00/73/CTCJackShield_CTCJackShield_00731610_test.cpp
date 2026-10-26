// Behaviour test for 0x00731610 CTCJackShield::CTCJackShield
#include <cstdio>
class CThing { public: int id; };
struct CPtr {
    CThing* item;
    CPtr() : item((CThing*)1) {}
    CPtr(CThing* p) : item(p) {}
    CPtr& operator=(const CPtr& o) { item = o.item; return *this; }
};
class CTCBase { public: CThing* ParentThing; CTCBase(CThing* o) : ParentThing(o) {} };
class CTCJackShield : public CTCBase { public:
    CPtr PEmitter;
    CTCJackShield(CThing* owner) : CTCBase(owner) { PEmitter = CPtr(0); }
};
int main()
{
    CThing t = { 3 };
    CTCJackShield s(&t);
    if (s.ParentThing != &t || s.PEmitter.item != 0) { std::printf("00731610_TEST FAIL\n"); return 1; }
    std::printf("00731610_TEST PASS\n");
    return 0;
}