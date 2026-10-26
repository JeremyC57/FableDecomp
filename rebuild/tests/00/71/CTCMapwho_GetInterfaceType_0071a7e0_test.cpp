// Test for 0x0071A7E0 CTCMapwho::GetInterfaceType: the virtual returns the class's interface type 0x3A.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCMapwho {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCMapwho::GetInterfaceType() const
{
    return (ETCInterfaceType)0x3A;
}
int main()
{
    CTCMapwho object;
    const CTCMapwho* p = &object;
    if ((long)p->GetInterfaceType() != 0x3A) { printf("0071a7e0_TEST FAIL\n"); return 1; }
    printf("0071a7e0_TEST PASS\n");
    return 0;
}
