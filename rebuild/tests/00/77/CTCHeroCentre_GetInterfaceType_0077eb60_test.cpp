// Test for 0x0077EB60 CTCHeroCentre::GetInterfaceType: the virtual returns the class's interface type 0x6A.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCHeroCentre {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCHeroCentre::GetInterfaceType() const
{
    return (ETCInterfaceType)0x6A;
}
int main()
{
    CTCHeroCentre object;
    const CTCHeroCentre* p = &object;
    if ((long)p->GetInterfaceType() != 0x6A) { printf("0077eb60_TEST FAIL\n"); return 1; }
    printf("0077eb60_TEST PASS\n");
    return 0;
}
