#include <cstdio>
// Behaviour test for CThingFilter_IsNot::operator() (0x006A76A0), linked against the row's object
// (??RCThingFilter_IsNot@@QBE_NABVCThing@@@Z): the excluded thing's address at +0x00.
class CThing { public: int pad; };
class CThingFilter_IsNot { public:
    const CThing* Excluded;
    bool operator()(const CThing& thing) const;
};
int main(){ CThing m,n; CThingFilter_IsNot o; o.Excluded=&m; if(!o(n)){ std::printf("006a76a0_TEST FAIL\n"); return 1;}
 if(o(m)){ std::printf("006a76a0_TEST FAIL\n"); return 1;}
 std::printf("006a76a0_TEST PASS\n"); return 0;}
