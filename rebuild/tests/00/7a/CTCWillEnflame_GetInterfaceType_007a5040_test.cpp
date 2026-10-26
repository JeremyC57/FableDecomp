// Test for 0x007A5040 CTCWillEnflame::GetInterfaceType: the virtual returns the class's interface type 0x95.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCWillEnflame {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCWillEnflame::GetInterfaceType() const
{
    return (ETCInterfaceType)0x95;
}
int main()
{
    CTCWillEnflame object;
    const CTCWillEnflame* p = &object;
    if ((long)p->GetInterfaceType() != 0x95) { printf("007a5040_TEST FAIL\n"); return 1; }
    printf("007a5040_TEST PASS\n");
    return 0;
}
