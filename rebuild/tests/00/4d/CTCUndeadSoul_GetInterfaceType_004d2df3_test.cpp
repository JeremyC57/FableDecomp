// Test for 0x004D2DF3 CTCUndeadSoul::GetInterfaceType: the virtual returns the class's interface type 0x7E.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCUndeadSoul {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCUndeadSoul::GetInterfaceType() const
{
    return (ETCInterfaceType)0x7E;
}
int main()
{
    CTCUndeadSoul object;
    const CTCUndeadSoul* p = &object;
    if ((long)p->GetInterfaceType() != 0x7E) { printf("004d2df3_TEST FAIL\n"); return 1; }
    printf("004d2df3_TEST PASS\n");
    return 0;
}
