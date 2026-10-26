// Test for 0x004E48C3 CTCSearchableContainer::GetInterfaceType: the virtual returns the class's interface type 0x3C.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCSearchableContainer {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCSearchableContainer::GetInterfaceType() const
{
    return (ETCInterfaceType)0x3C;
}
int main()
{
    CTCSearchableContainer object;
    const CTCSearchableContainer* p = &object;
    if ((long)p->GetInterfaceType() != 0x3C) { printf("004e48c3_TEST FAIL\n"); return 1; }
    printf("004e48c3_TEST PASS\n");
    return 0;
}
