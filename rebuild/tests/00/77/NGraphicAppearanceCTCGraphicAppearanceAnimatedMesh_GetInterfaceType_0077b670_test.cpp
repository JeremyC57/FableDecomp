// Test for 0x0077B670 NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh::GetInterfaceType: the virtual returns the class's interface type 0x5C.
#include <stdio.h>
enum ETCInterfaceType { };
namespace NGraphicAppearance {
class CTCGraphicAppearanceAnimatedMesh {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
}
ETCInterfaceType NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh::GetInterfaceType() const
{
    return (ETCInterfaceType)0x5C;
}
int main()
{
    NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh object;
    const NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh* p = &object;
    if ((long)p->GetInterfaceType() != 0x5C) { printf("0077b670_TEST FAIL\n"); return 1; }
    printf("0077b670_TEST PASS\n");
    return 0;
}
