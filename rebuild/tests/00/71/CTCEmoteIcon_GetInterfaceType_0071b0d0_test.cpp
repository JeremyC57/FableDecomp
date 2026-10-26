// Test for 0x0071B0D0 CTCEmoteIcon::GetInterfaceType: the virtual returns the class's interface type 0x45.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCEmoteIcon {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCEmoteIcon::GetInterfaceType() const
{
    return (ETCInterfaceType)0x45;
}
int main()
{
    CTCEmoteIcon object;
    const CTCEmoteIcon* p = &object;
    if ((long)p->GetInterfaceType() != 0x45) { printf("0071b0d0_TEST FAIL\n"); return 1; }
    printf("0071b0d0_TEST PASS\n");
    return 0;
}
