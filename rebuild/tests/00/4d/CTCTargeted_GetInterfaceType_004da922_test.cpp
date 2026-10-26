// Test for 0x004DA922 CTCTargeted::GetInterfaceType: the virtual returns the class's interface type 0x9.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCTargeted {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCTargeted::GetInterfaceType() const
{
    return (ETCInterfaceType)0x9;
}
int main()
{
    CTCTargeted object;
    const CTCTargeted* p = &object;
    if ((long)p->GetInterfaceType() != 0x9) { printf("004da922_TEST FAIL\n"); return 1; }
    printf("004da922_TEST PASS\n");
    return 0;
}
