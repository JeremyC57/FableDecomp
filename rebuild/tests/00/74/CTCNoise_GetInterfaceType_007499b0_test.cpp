// Test for 0x007499B0 CTCNoise::GetInterfaceType: the virtual returns the class's interface type 0x37.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCNoise {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCNoise::GetInterfaceType() const
{
    return (ETCInterfaceType)0x37;
}
int main()
{
    CTCNoise object;
    const CTCNoise* p = &object;
    if ((long)p->GetInterfaceType() != 0x37) { printf("007499b0_TEST FAIL\n"); return 1; }
    printf("007499b0_TEST PASS\n");
    return 0;
}
