// Test for 0x00778BD0 CTCFamilyMember::GetInterfaceType: the virtual returns the class's interface type 0x57.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCFamilyMember {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCFamilyMember::GetInterfaceType() const
{
    return (ETCInterfaceType)0x57;
}
int main()
{
    CTCFamilyMember object;
    const CTCFamilyMember* p = &object;
    if ((long)p->GetInterfaceType() != 0x57) { printf("00778bd0_TEST FAIL\n"); return 1; }
    printf("00778bd0_TEST PASS\n");
    return 0;
}
