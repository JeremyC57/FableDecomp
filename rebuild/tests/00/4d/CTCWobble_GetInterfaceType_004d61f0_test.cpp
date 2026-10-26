// Test for 0x004D61F0 CTCWobble::GetInterfaceType: the virtual returns the class's interface type 0xF7.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCWobble {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCWobble::GetInterfaceType() const
{
    return (ETCInterfaceType)0xF7;
}
int main()
{
    CTCWobble object;
    const CTCWobble* p = &object;
    if ((long)p->GetInterfaceType() != 0xF7) { printf("004d61f0_TEST FAIL\n"); return 1; }
    printf("004d61f0_TEST PASS\n");
    return 0;
}
