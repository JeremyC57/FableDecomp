// Test for 0x00798C80 CTCForcePushed::GetInterfaceType: the virtual returns the class's interface type 0x8D.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCForcePushed {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCForcePushed::GetInterfaceType() const
{
    return (ETCInterfaceType)0x8D;
}
int main()
{
    CTCForcePushed object;
    const CTCForcePushed* p = &object;
    if ((long)p->GetInterfaceType() != 0x8D) { printf("00798c80_TEST FAIL\n"); return 1; }
    printf("00798c80_TEST PASS\n");
    return 0;
}
