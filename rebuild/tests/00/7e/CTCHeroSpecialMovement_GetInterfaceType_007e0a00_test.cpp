// Test for 0x007E0A00 CTCHeroSpecialMovement::GetInterfaceType: the virtual returns the class's interface type 0xDF.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCHeroSpecialMovement {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCHeroSpecialMovement::GetInterfaceType() const
{
    return (ETCInterfaceType)0xDF;
}
int main()
{
    CTCHeroSpecialMovement object;
    const CTCHeroSpecialMovement* p = &object;
    if ((long)p->GetInterfaceType() != 0xDF) { printf("007e0a00_TEST FAIL\n"); return 1; }
    printf("007e0a00_TEST PASS\n");
    return 0;
}
