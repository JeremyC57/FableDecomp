// Test for 0x007DF2B0 CTCWallMount::GetInterfaceType: the virtual returns the class's interface type 0xDA.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCWallMount {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCWallMount::GetInterfaceType() const
{
    return (ETCInterfaceType)0xDA;
}
int main()
{
    CTCWallMount object;
    const CTCWallMount* p = &object;
    if ((long)p->GetInterfaceType() != 0xDA) { printf("007df2b0_TEST FAIL\n"); return 1; }
    printf("007df2b0_TEST PASS\n");
    return 0;
}
