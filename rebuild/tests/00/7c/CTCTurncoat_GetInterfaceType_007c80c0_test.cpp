// Test for 0x007C80C0 CTCTurncoat::GetInterfaceType: the virtual returns the class's interface type 0x9F.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCTurncoat {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCTurncoat::GetInterfaceType() const
{
    return (ETCInterfaceType)0x9F;
}
int main()
{
    CTCTurncoat object;
    const CTCTurncoat* p = &object;
    if ((long)p->GetInterfaceType() != 0x9F) { printf("007c80c0_TEST FAIL\n"); return 1; }
    printf("007c80c0_TEST PASS\n");
    return 0;
}
