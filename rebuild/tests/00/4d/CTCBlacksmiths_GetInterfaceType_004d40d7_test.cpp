// Test for 0x004D40D7 CTCBlacksmiths::GetInterfaceType: the virtual returns the class's interface type 0x55.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCBlacksmiths {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCBlacksmiths::GetInterfaceType() const
{
    return (ETCInterfaceType)0x55;
}
int main()
{
    CTCBlacksmiths object;
    const CTCBlacksmiths* p = &object;
    if ((long)p->GetInterfaceType() != 0x55) { printf("004d40d7_TEST FAIL\n"); return 1; }
    printf("004d40d7_TEST PASS\n");
    return 0;
}
