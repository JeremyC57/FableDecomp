// Test for 0x004D51E0 CTCTavernJug::GetInterfaceType: the virtual returns the class's interface type 0xBB.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCTavernJug {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCTavernJug::GetInterfaceType() const
{
    return (ETCInterfaceType)0xBB;
}
int main()
{
    CTCTavernJug object;
    const CTCTavernJug* p = &object;
    if ((long)p->GetInterfaceType() != 0xBB) { printf("004d51e0_TEST FAIL\n"); return 1; }
    printf("004d51e0_TEST PASS\n");
    return 0;
}
