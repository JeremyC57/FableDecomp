// Test for 0x007DFC50 CTCFish::GetInterfaceType: the virtual returns the class's interface type 0xDC.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCFish {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCFish::GetInterfaceType() const
{
    return (ETCInterfaceType)0xDC;
}
int main()
{
    CTCFish object;
    const CTCFish* p = &object;
    if ((long)p->GetInterfaceType() != 0xDC) { printf("007dfc50_TEST FAIL\n"); return 1; }
    printf("007dfc50_TEST PASS\n");
    return 0;
}
