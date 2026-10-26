// Test for 0x004D2B63 CTCRotate::GetInterfaceType: the virtual returns the class's interface type 0x2A.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCRotate {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCRotate::GetInterfaceType() const
{
    return (ETCInterfaceType)0x2A;
}
int main()
{
    CTCRotate object;
    const CTCRotate* p = &object;
    if ((long)p->GetInterfaceType() != 0x2A) { printf("004d2b63_TEST FAIL\n"); return 1; }
    printf("004d2b63_TEST PASS\n");
    return 0;
}
