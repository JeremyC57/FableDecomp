// Test for 0x0077BCF0 NGraphicAppearance::CTCGraphicAppearanceStaticMesh::GetInterfaceType: the virtual returns the class's interface type 0x5C.
#include <stdio.h>
enum ETCInterfaceType { };
namespace NGraphicAppearance {
class CTCGraphicAppearanceStaticMesh {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
}
ETCInterfaceType NGraphicAppearance::CTCGraphicAppearanceStaticMesh::GetInterfaceType() const
{
    return (ETCInterfaceType)0x5C;
}
int main()
{
    NGraphicAppearance::CTCGraphicAppearanceStaticMesh object;
    const NGraphicAppearance::CTCGraphicAppearanceStaticMesh* p = &object;
    if ((long)p->GetInterfaceType() != 0x5C) { printf("0077bcf0_TEST FAIL\n"); return 1; }
    printf("0077bcf0_TEST PASS\n");
    return 0;
}
