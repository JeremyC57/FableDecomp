// Test for 0x00751250 CTCExplodingObject::GetInterfaceType: the virtual returns the class's interface type 0xDE.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCExplodingObject {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCExplodingObject::GetInterfaceType() const
{
    return (ETCInterfaceType)0xDE;
}
int main()
{
    CTCExplodingObject object;
    const CTCExplodingObject* p = &object;
    if ((long)p->GetInterfaceType() != 0xDE) { printf("00751250_TEST FAIL\n"); return 1; }
    printf("00751250_TEST PASS\n");
    return 0;
}
