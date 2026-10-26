// Test for 0x004DB227 CTCDecapitation::GetInterfaceType: the virtual returns the class's interface type 0xD1.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDecapitation {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDecapitation::GetInterfaceType() const
{
    return (ETCInterfaceType)0xD1;
}
int main()
{
    CTCDecapitation object;
    const CTCDecapitation* p = &object;
    if ((long)p->GetInterfaceType() != 0xD1) { printf("004db227_TEST FAIL\n"); return 1; }
    printf("004db227_TEST PASS\n");
    return 0;
}
