// Test for 0x007460A0 CTCPerceiveThing::GetInterfaceType: the virtual returns the class's interface type 0x34.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCPerceiveThing {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCPerceiveThing::GetInterfaceType() const
{
    return (ETCInterfaceType)0x34;
}
int main()
{
    CTCPerceiveThing object;
    const CTCPerceiveThing* p = &object;
    if ((long)p->GetInterfaceType() != 0x34) { printf("007460a0_TEST FAIL\n"); return 1; }
    printf("007460a0_TEST PASS\n");
    return 0;
}
