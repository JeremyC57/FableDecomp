// Test for 0x00787490 CTCSkeletalMorph::GetInterfaceType: the virtual returns the class's interface type 0x78.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCSkeletalMorph {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCSkeletalMorph::GetInterfaceType() const
{
    return (ETCInterfaceType)0x78;
}
int main()
{
    CTCSkeletalMorph object;
    const CTCSkeletalMorph* p = &object;
    if ((long)p->GetInterfaceType() != 0x78) { printf("00787490_TEST FAIL\n"); return 1; }
    printf("00787490_TEST PASS\n");
    return 0;
}
