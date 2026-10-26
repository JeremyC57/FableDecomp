// Test for 0x007BACD0 CTCTavern::GetInterfaceType: the virtual returns the class's interface type 0xAB.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCTavern {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCTavern::GetInterfaceType() const
{
    return (ETCInterfaceType)0xAB;
}
int main()
{
    CTCTavern object;
    const CTCTavern* p = &object;
    if ((long)p->GetInterfaceType() != 0xAB) { printf("007bacd0_TEST FAIL\n"); return 1; }
    printf("007bacd0_TEST PASS\n");
    return 0;
}
