// Test for 0x007CBE70 CTCGold::GetInterfaceType: the virtual returns the class's interface type 0xCC.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCGold {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCGold::GetInterfaceType() const
{
    return (ETCInterfaceType)0xCC;
}
int main()
{
    CTCGold object;
    const CTCGold* p = &object;
    if ((long)p->GetInterfaceType() != 0xCC) { printf("007cbe70_TEST FAIL\n"); return 1; }
    printf("007cbe70_TEST PASS\n");
    return 0;
}
