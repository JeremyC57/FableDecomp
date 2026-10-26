// Test for 0x006285E0 CTCQuickAccessMenu::GetInterfaceType: the virtual returns the class's interface type 0xF6.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCQuickAccessMenu {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCQuickAccessMenu::GetInterfaceType() const
{
    return (ETCInterfaceType)0xF6;
}
int main()
{
    CTCQuickAccessMenu object;
    const CTCQuickAccessMenu* p = &object;
    if ((long)p->GetInterfaceType() != 0xF6) { printf("006285e0_TEST FAIL\n"); return 1; }
    printf("006285e0_TEST PASS\n");
    return 0;
}
