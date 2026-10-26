// Test for 0x007E1A70 NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes::GetInterfaceType: the virtual returns the class's interface type 0x5C.
#include <stdio.h>
enum ETCInterfaceType { };
namespace NGraphicAppearance {
class CTCGraphicAppearanceMultipleStaticMeshes {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
}
ETCInterfaceType NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes::GetInterfaceType() const
{
    return (ETCInterfaceType)0x5C;
}
int main()
{
    NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes object;
    const NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes* p = &object;
    if ((long)p->GetInterfaceType() != 0x5C) { printf("007e1a70_TEST FAIL\n"); return 1; }
    printf("007e1a70_TEST PASS\n");
    return 0;
}
