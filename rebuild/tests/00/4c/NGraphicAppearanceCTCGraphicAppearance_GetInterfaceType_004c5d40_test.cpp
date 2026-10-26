// Test for 0x004C5D40 NGraphicAppearance::CTCGraphicAppearance::GetInterfaceType: the virtual returns the class's interface type 0x5B.
#include <stdio.h>
enum ETCInterfaceType { };
namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
}
ETCInterfaceType NGraphicAppearance::CTCGraphicAppearance::GetInterfaceType() const
{
    return (ETCInterfaceType)0x5B;
}
int main()
{
    NGraphicAppearance::CTCGraphicAppearance object;
    const NGraphicAppearance::CTCGraphicAppearance* p = &object;
    if ((long)p->GetInterfaceType() != 0x5B) { printf("004c5d40_TEST FAIL\n"); return 1; }
    printf("004c5d40_TEST PASS\n");
    return 0;
}
