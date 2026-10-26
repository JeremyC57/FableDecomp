// Test for 0x007E5AD0 CTCLifeTimer::GetInterfaceType: the virtual returns the class's interface type 0x103.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCLifeTimer {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCLifeTimer::GetInterfaceType() const
{
    return (ETCInterfaceType)0x103;
}
int main()
{
    CTCLifeTimer object;
    const CTCLifeTimer* p = &object;
    if ((long)p->GetInterfaceType() != 0x103) { printf("007e5ad0_TEST FAIL\n"); return 1; }
    printf("007e5ad0_TEST PASS\n");
    return 0;
}
