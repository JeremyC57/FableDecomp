// Test for 0x007097B0 CTCHeroAttachableAppearanceModifiers::GetInterfaceType: the virtual returns the class's interface type 0x5E.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCHeroAttachableAppearanceModifiers {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCHeroAttachableAppearanceModifiers::GetInterfaceType() const
{
    return (ETCInterfaceType)0x5E;
}
int main()
{
    CTCHeroAttachableAppearanceModifiers object;
    const CTCHeroAttachableAppearanceModifiers* p = &object;
    if ((long)p->GetInterfaceType() != 0x5E) { printf("007097b0_TEST FAIL\n"); return 1; }
    printf("007097b0_TEST PASS\n");
    return 0;
}
