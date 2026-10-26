// Test for 0x004DF303 CTCAISummon::GetInterfaceType: the virtual returns the class's interface type 0xFA.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCAISummon {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCAISummon::GetInterfaceType() const
{
    return (ETCInterfaceType)0xFA;
}
int main()
{
    CTCAISummon object;
    const CTCAISummon* p = &object;
    if ((long)p->GetInterfaceType() != 0xFA) { printf("004df303_TEST FAIL\n"); return 1; }
    printf("004df303_TEST PASS\n");
    return 0;
}
