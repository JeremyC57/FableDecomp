// Test for 0x007B26C0 CTCFishing::GetInterfaceType: the virtual returns the class's interface type 0xB1.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCFishing {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCFishing::GetInterfaceType() const
{
    return (ETCInterfaceType)0xB1;
}
int main()
{
    CTCFishing object;
    const CTCFishing* p = &object;
    if ((long)p->GetInterfaceType() != 0xB1) { printf("007b26c0_TEST FAIL\n"); return 1; }
    printf("007b26c0_TEST PASS\n");
    return 0;
}
