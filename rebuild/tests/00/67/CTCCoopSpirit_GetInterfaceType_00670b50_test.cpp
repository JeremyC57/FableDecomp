// Test for 0x00670B50 CTCCoopSpirit::GetInterfaceType: the virtual returns the class's interface type 0xC7.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCoopSpirit {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCoopSpirit::GetInterfaceType() const
{
    return (ETCInterfaceType)0xC7;
}
int main()
{
    CTCCoopSpirit object;
    const CTCCoopSpirit* p = &object;
    if ((long)p->GetInterfaceType() != 0xC7) { printf("00670b50_TEST FAIL\n"); return 1; }
    printf("00670b50_TEST PASS\n");
    return 0;
}
