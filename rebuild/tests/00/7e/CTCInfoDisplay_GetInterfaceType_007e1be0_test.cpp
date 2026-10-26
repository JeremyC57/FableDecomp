// Test for 0x007E1BE0 CTCInfoDisplay::GetInterfaceType: the virtual returns the class's interface type 0xE5.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCInfoDisplay {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCInfoDisplay::GetInterfaceType() const
{
    return (ETCInterfaceType)0xE5;
}
int main()
{
    CTCInfoDisplay object;
    const CTCInfoDisplay* p = &object;
    if ((long)p->GetInterfaceType() != 0xE5) { printf("007e1be0_TEST FAIL\n"); return 1; }
    printf("007e1be0_TEST PASS\n");
    return 0;
}
