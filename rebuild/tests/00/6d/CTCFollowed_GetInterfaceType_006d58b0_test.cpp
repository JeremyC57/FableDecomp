// Test for 0x006D58B0 CTCFollowed::GetInterfaceType: the virtual returns the class's interface type 0xBC.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCFollowed {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCFollowed::GetInterfaceType() const
{
    return (ETCInterfaceType)0xBC;
}
int main()
{
    CTCFollowed object;
    const CTCFollowed* p = &object;
    if ((long)p->GetInterfaceType() != 0xBC) { printf("006d58b0_TEST FAIL\n"); return 1; }
    printf("006d58b0_TEST PASS\n");
    return 0;
}
