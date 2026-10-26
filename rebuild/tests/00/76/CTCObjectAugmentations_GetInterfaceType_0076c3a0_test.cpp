// Test for 0x0076C3A0 CTCObjectAugmentations::GetInterfaceType: the virtual returns the class's interface type 0xBF.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCObjectAugmentations {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCObjectAugmentations::GetInterfaceType() const
{
    return (ETCInterfaceType)0xBF;
}
int main()
{
    CTCObjectAugmentations object;
    const CTCObjectAugmentations* p = &object;
    if ((long)p->GetInterfaceType() != 0xBF) { printf("0076c3a0_TEST FAIL\n"); return 1; }
    printf("0076c3a0_TEST PASS\n");
    return 0;
}
