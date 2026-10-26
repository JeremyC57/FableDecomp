// Test for 0x0077C250 NGraphicAppearance::CTCGraphicAppearanceStaticMeshFadeOutOnProximity::GetInterfaceType: the virtual returns the class's interface type 0x106.
#include <stdio.h>
enum ETCInterfaceType { };
namespace NGraphicAppearance {
class CTCGraphicAppearanceStaticMeshFadeOutOnProximity {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
}
ETCInterfaceType NGraphicAppearance::CTCGraphicAppearanceStaticMeshFadeOutOnProximity::GetInterfaceType() const
{
    return (ETCInterfaceType)0x106;
}
int main()
{
    NGraphicAppearance::CTCGraphicAppearanceStaticMeshFadeOutOnProximity object;
    const NGraphicAppearance::CTCGraphicAppearanceStaticMeshFadeOutOnProximity* p = &object;
    if ((long)p->GetInterfaceType() != 0x106) { printf("0077c250_TEST FAIL\n"); return 1; }
    printf("0077c250_TEST PASS\n");
    return 0;
}
