// Test for 0x00750A30 CTCDiscovery::GetInterfaceType: the virtual returns the class's interface type 0x40.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDiscovery {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDiscovery::GetInterfaceType() const
{
    return (ETCInterfaceType)0x40;
}
int main()
{
    CTCDiscovery object;
    const CTCDiscovery* p = &object;
    if ((long)p->GetInterfaceType() != 0x40) { printf("00750a30_TEST FAIL\n"); return 1; }
    printf("00750a30_TEST PASS\n");
    return 0;
}
