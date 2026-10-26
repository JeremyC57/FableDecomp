// Test for 0x004DB3B5 CTCDRumble::GetInterfaceType: the virtual returns the class's interface type 0xEC.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDRumble {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDRumble::GetInterfaceType() const
{
    return (ETCInterfaceType)0xEC;
}
int main()
{
    CTCDRumble object;
    const CTCDRumble* p = &object;
    if ((long)p->GetInterfaceType() != 0xEC) { printf("004db3b5_TEST FAIL\n"); return 1; }
    printf("004db3b5_TEST PASS\n");
    return 0;
}
