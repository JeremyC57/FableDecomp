// Test for 0x00758E80 CTCWound::GetInterfaceType: the virtual returns the class's interface type 0x5.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCWound {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCWound::GetInterfaceType() const
{
    return (ETCInterfaceType)0x5;
}
int main()
{
    CTCWound object;
    const CTCWound* p = &object;
    if ((long)p->GetInterfaceType() != 0x5) { printf("00758e80_TEST FAIL\n"); return 1; }
    printf("00758e80_TEST PASS\n");
    return 0;
}
