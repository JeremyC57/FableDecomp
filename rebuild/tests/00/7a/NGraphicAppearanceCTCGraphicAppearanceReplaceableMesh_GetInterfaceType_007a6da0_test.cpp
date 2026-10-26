// Test for 0x007A6DA0 NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh::GetInterfaceType: the virtual returns the class's interface type 0x5C.
#include <stdio.h>
enum ETCInterfaceType { };
namespace NGraphicAppearance {
class CTCGraphicAppearanceReplaceableMesh {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
}
ETCInterfaceType NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh::GetInterfaceType() const
{
    return (ETCInterfaceType)0x5C;
}
int main()
{
    NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh object;
    const NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh* p = &object;
    if ((long)p->GetInterfaceType() != 0x5C) { printf("007a6da0_TEST FAIL\n"); return 1; }
    printf("007a6da0_TEST PASS\n");
    return 0;
}
