// Test for 0x004D610C CTCSleep::GetInterfaceType: the virtual returns the class's interface type 0xD4.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCSleep {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCSleep::GetInterfaceType() const
{
    return (ETCInterfaceType)0xD4;
}
int main()
{
    CTCSleep object;
    const CTCSleep* p = &object;
    if ((long)p->GetInterfaceType() != 0xD4) { printf("004d610c_TEST FAIL\n"); return 1; }
    printf("004d610c_TEST PASS\n");
    return 0;
}
