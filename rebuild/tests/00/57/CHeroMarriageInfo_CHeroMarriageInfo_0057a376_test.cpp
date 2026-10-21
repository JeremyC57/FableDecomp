// test for 0057a376 CHeroMarriageInfo ctor
#include <stdio.h>
#include <stddef.h>
enum EMarriageState { MARRIAGE_STATE_NONE };
class CHeroMarriageInfo {
public:
    unsigned __int64 WifeUID;                            // +0x00
    unsigned __int64 MaritalHomeUID;                     // +0x08
    EMarriageState   State;                              // +0x10
    bool             GayMarriage;                        // +0x14
    CHeroMarriageInfo(unsigned __int64 wife_uid, unsigned __int64 marital_home_uid, EMarriageState state, bool gay_marriage);
};
CHeroMarriageInfo::CHeroMarriageInfo(unsigned __int64 wife_uid, unsigned __int64 marital_home_uid, EMarriageState state, bool gay_marriage)
    : WifeUID(wife_uid)
    , MaritalHomeUID(marital_home_uid)
    , State(state)
    , GayMarriage(gay_marriage)
{
}

int main()
{
    CHeroMarriageInfo m(0x1122334455667788ui64, 0x99aabbccddeeff00ui64, (EMarriageState)3, true);
    int ok = sizeof m == 0x18 && offsetof(CHeroMarriageInfo, GayMarriage) == 0x14 && m.WifeUID == 0x1122334455667788ui64
          && m.MaritalHomeUID == 0x99aabbccddeeff00ui64 && m.State == 3 && m.GayMarriage;
    printf(ok ? "0057a376_TEST PASS\n" : "0057a376_TEST FAIL\n");
    return 0;
}