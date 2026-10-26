// Test for 0x006C43F0 CTCFadeOutAndIn::GetInterfaceType: the virtual returns the class's interface type 0xF9.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCFadeOutAndIn {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCFadeOutAndIn::GetInterfaceType() const
{
    return (ETCInterfaceType)0xF9;
}
int main()
{
    CTCFadeOutAndIn object;
    const CTCFadeOutAndIn* p = &object;
    if ((long)p->GetInterfaceType() != 0xF9) { printf("006c43f0_TEST FAIL\n"); return 1; }
    printf("006c43f0_TEST PASS\n");
    return 0;
}
